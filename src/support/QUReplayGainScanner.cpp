#include <QUrl>
#include <QObject>
#include <QAudioFormat>
#include <QEventLoop>
#include <ebur128.h>
#include <fileref.h>
#include <tfile.h>
#include <tstring.h>
#include <tpropertymap.h>
#include <mp4file.h>
#include <mpegfile.h>
#include <opusfile.h>
#include <cmath>

#include "QUReplayGainScanner.h"
#include "QUSongItem.h"
#include "QULogService.h"

inline TagLib::String formatGain(double gain) { return TagLib::String(QString("%1 dB").arg(gain, 0, 'f', 2).toStdString()); }
inline TagLib::String formatPeak(double peak) { return TagLib::String(QString("%1").arg(peak, 0, 'f', 6).toStdString()); }
inline TagLib::String formatGainR128(double gain) { return TagLib::String(QString("%1").arg(std::round(gain * 256.0)).toStdString()); }

QUReplayGainFile::QUReplayGainFile(const QString &path, QObject *parent):
    QObject(parent),
    _path(path),
    _ebur128_state(nullptr),
    _loudness(0.0),
    _peak(0.0),
    _error(false),
    _cancelled(false)
{
    _decoder.setSource(QUrl(path));
}

QUReplayGainFile::~QUReplayGainFile()
{
    if (_ebur128_state)
        ebur128_destroy(&_ebur128_state);
}

void QUReplayGainFile::readBuffer()
{
    QAudioBuffer buffer = _decoder.read();
    QAudioFormat format = buffer.format();
    if (!_ebur128_state && !initializeEbur128(format)) {
        setDecodingError();
        return;
    }

    switch(format.sampleFormat()) {
        case QAudioFormat::Int16:
            ebur128_add_frames_short(_ebur128_state, buffer.constData<short>(), buffer.frameCount());
            break;

        case QAudioFormat::Int32:
            ebur128_add_frames_int(_ebur128_state, buffer.constData<int>(), buffer.frameCount());
            break;

        case QAudioFormat::Float:
            ebur128_add_frames_float(_ebur128_state, buffer.constData<float>(), buffer.frameCount());
            break;

        default:
            break;
    }
}

bool QUReplayGainFile::initializeEbur128(const QAudioFormat &format)
{
    QAudioFormat::SampleFormat sFormat = format.sampleFormat();
    if (!(sFormat == QAudioFormat::Int16 || sFormat == QAudioFormat::Int32 || sFormat == QAudioFormat::Float)) {
        logSrv->add(tr("[ReplayGain Scanner] File '%1' has unsupported sample format").arg(_path), QU::Error);
        return false;
    }
    _ebur128_state = ebur128_init(format.channelCount(), format.sampleRate(), EBUR128_MODE_I | EBUR128_MODE_TRUE_PEAK);
    if (!_ebur128_state) {
        logSrv->add(tr("[ReplayGain Scanner] Failed to initialize loudness meter for file '%1").arg(_path), QU::Error);
        return false;
    }
    return true;
}

bool QUReplayGainFile::scan()
{
    connect(&_decoder, &QAudioDecoder::bufferReady, this, &QUReplayGainFile::readBuffer);
    connect(&_decoder, &QAudioDecoder::finished, this, &QUReplayGainFile::decodingFinished);
    connect(&_decoder, qOverload<QAudioDecoder::Error>(&QAudioDecoder::error), this, &QUReplayGainFile::decodingError);

    // Scan the file
    _decoder.start();
    if (!(_error || _cancelled))
        _loop.exec();
    if (_error || _cancelled)
        return false;

    // Calculate loudness
    if (ebur128_loudness_global(_ebur128_state, &_loudness) != EBUR128_SUCCESS) {
        logSrv->add(tr("[ReplayGain Scanner] Failed to calculate loudness for file '%'").arg(_path), QU::Error);
        return false;
    }
    else if (_loudness == -HUGE_VAL) {
        logSrv->add(tr("[ReplayGain Scanner] File '%1' is completely silent").arg(_path), QU::Error);
        return false;
    }

    // Calculate peak
    double channel_peak;
    for (unsigned int i = 0; i < _ebur128_state->channels; i++) {
        if (ebur128_true_peak(_ebur128_state, i, &channel_peak) != EBUR128_SUCCESS) {
            logSrv->add(tr("[ReplayGain Scanner] Failed to calculate peak for file '%1'").arg(_path), QU::Error);
            return false;
        }
        _peak = std::max(_peak, channel_peak);
    }

    return true;
}

bool QUReplayGainFile::tag()
{
    bool ret = false;
    TagLib::FileRef file(_path.toLocal8Bit().data(), false);
    TagLib::File *f = file.file();
    if (!f)
        return false;
    TagLib::MP4::File *mp4File = dynamic_cast<TagLib::MP4::File*>(f);
    TagLib::MPEG::File *mp3File = dynamic_cast<TagLib::MPEG::File*>(f);

    // MP4 files need special handling - PropertyMap API doesn't work well
    if (mp4File) {
        TagLib::MP4::Tag *tag = mp4File->tag();
        const TagLib::MP4::ItemMap map = tag->itemMap();

        // Remove existing ReplayGain tags (if any)
        for (auto it = map.begin(); it != map.end(); ++it) {
            TagLib::String key = it->first.upper();
            if (key == "----:COM.APPLE.ITUNES:REPLAYGAIN_TRACK_GAIN" ||
                key == "----:COM.APPLE.ITUNES:REPLAYGAIN_TRACK_PEAK" ||
                key == "----:COM.APPLE.ITUNES:REPLAYGAIN_ALBUM_GAIN" ||
                key == "----:COM.APPLE.ITUNES:REPLAYGAIN_ALBUM_PEAK")
                tag->removeItem(it->first);
        }

        // Write ReplayGain tags
        tag->setItem("----:com.apple.iTunes:REPLAYGAIN_TRACK_GAIN", TagLib::MP4::Item(formatGain(-18 - _loudness)));
        tag->setItem("----:com.apple.iTunes:REPLAYGAIN_TRACK_PEAK", TagLib::MP4::Item(formatPeak(_peak)));
    }

    // For other formats, PropertyMap API works fine
    else {
        TagLib::Ogg::Opus::File *opusFile = dynamic_cast<TagLib::Ogg::Opus::File*>(f);

        // Remove existing ReplayGain tags (if any)
        TagLib::PropertyMap props = f->properties();
        props.erase("REPLAYGAIN_TRACK_GAIN");
        props.erase("REPLAYGAIN_TRACK_PEAK");
        props.erase("REPLAYGAIN_ALBUM_GAIN");
        props.erase("REPLAYGAIN_ALBUM_PEAK");
        if (opusFile)
            props.erase("R128_TRACK_GAIN");
        f->setProperties(props);

        // Write ReplayGain tags
        props = f->properties();
        if (opusFile)
            props["R128_TRACK_GAIN"] = formatGainR128(-23 - _loudness);
        else {
            props["REPLAYGAIN_TRACK_GAIN"] = formatGain(-18 - _loudness);
            props["REPLAYGAIN_TRACK_PEAK"] = formatPeak(_peak);
        }
        f->setProperties(props);
    }

    // For MP3 files, TagLib will duplicate the information into a ID3v1 tag
    // by default...need to call the overloaded save function
    if (mp3File)
        ret = mp3File->save(
#if TAGLIB_MAJOR_VERSION > 1
            TagLib::MPEG::File::TagTypes::ID3v2,
            TagLib::File::StripTags::StripNone,
            TagLib::ID3v2::Version::v4,
            TagLib::File::DuplicateTags::DoNotDuplicate
#else
            static_cast<int>(TagLib::MPEG::File::TagTypes::ID3v2),
            false,
            4,
            false
#endif
        );
    else
        ret = f->save();

    if (!ret)
        logSrv->add(tr("[ReplayGain Scanner] failed to write tags to audio file '%1'").arg(_path), QU::Error);
    return ret;
}

void QUReplayGainFile::cancel()
{
    _cancelled = true;
    _decoder.stop();
    _loop.quit();
}

// Called by this
void QUReplayGainFile::setDecodingError()
{
    _error = true;
    _decoder.stop();
    _loop.quit();
}

// Called by QAudioDecoder
void QUReplayGainFile::decodingError([[maybe_unused]] QAudioDecoder::Error error)
{
    logSrv->add(tr("[ReplayGain Scanner] Error occured while decoding file '%1'").arg(_path), QU::Error);
    _error = true;
    _loop.quit();
}

QUReplayGainScanner::QUReplayGainScanner(const QList<QUSongItem*> &selectedItems, QObject *parent):
    QThread(parent),
    _selectedItems(selectedItems),
    _cancelled(false)
{

}

void QUReplayGainScanner::run()
{
    foreach(QUSongItem *songItem, _selectedItems) {
        QUSongFile *song = songItem->song();
		emit updateProgress(QString("%1 - %2").arg(song->artist(), song->title()));
        if (!song->hasAudio()) {
            logSrv->add(tr("[ReplayGain Scanner] Song '%1 - %2' has no valid audio file")
                            .arg(song->artist()).arg(song->title()),
                        QU::Error);
            continue;
        }

        QUReplayGainFile file(song->audioFileInfo().absoluteFilePath());
        connect(this, &QUReplayGainScanner::userCancelled, &file, &QUReplayGainFile::cancel);
        if (file.scan() && file.tag()) {
            song->updateReplayGain();
            emit updateItem(song);
        }
        if (_cancelled)
            break;
    }
    emit scanFinished(0);
}

void QUReplayGainScanner::cancel()
{
    _cancelled = true;
    emit userCancelled();
}
