#include "QUCleanUpTask.h"
#include "QUSmartCheckBox.h"
#include "QUSmartInputCheck.h"
#include "QUSongSupport.h"
#include <QFileInfo>
#include <QMimeDatabase>
#include <QImage>
#include <QBuffer>
#include <QDebug>

#include "fileref.h"
#include "mpegfile.h"
#include "id3v1tag.h"
#include "id3v2tag.h"
#include "commentsframe.h"
#include "unsynchronizedlyricsframe.h"
#include "synchronizedlyricsframe.h"
#include "textidentificationframe.h"
#include "attachedpictureframe.h"

QUCleanUpTask::QUCleanUpTask(TaskModes mode, QObject *parent):
	QUSimpleTask(parent),
	_mode(mode)
{
	switch(_mode) {
	case UnusedFiles:
		this->setIcon(QIcon(":/types/file_delete.png"));
		this->setDescription(tr("Delete unused files"));
		this->setToolTip(tr("Every file which is not used by UltraStar will be deleted.<br><br><b>This cannot be undone!</b>"));
		break;
	case InvalidFileTags:
		this->setIcon(QIcon(":/types/tag_delete.png"));
		this->setDescription(tr("Clear invalid file-related tags"));
		this->setToolTip(tr("Removes the value of invalid file-related tags so that they will not be saved into the song text file.<br>"
				"<br>"
				"This includes <b>#VIDEOGAP</b> for invalid video files as well as <b>#START</b> and <b>#END</b> for invalid audio files."));
		break;
	case RemoveEndTag:
		this->setIcon(QIcon(":/types/no_end.png"));
		this->setDescription(tr("Remove #END tag"));
		break;
	case SetID3Tags:
		this->setIcon(QIcon(":/types/writeID3.png"));
		this->setDescription(tr("Write song info to ID3 of the audio file. This will overwrite existing info!"));
		break;
	}
}

void QUCleanUpTask::startOn(QUSongInterface *song) {
	_currentSong = song;
	
	QStringList filter;
	switch(_mode) {
	case UnusedFiles:
		if(smartSettings().at(0)->value().toBool()) filter << QUSongSupport::allowedAudioFiles();
		if(smartSettings().at(1)->value().toBool()) filter << QUSongSupport::allowedImageFiles();
		if(smartSettings().at(2)->value().toBool()) filter << QUSongSupport::allowedVideoFiles();
		if(smartSettings().at(3)->value().isValid())
			_currentSong->deleteUnusedFiles(filter, smartSettings().at(3)->value().toString(), true);
		else
			_currentSong->deleteUnusedFiles(filter, "", false);
		break;
	case InvalidFileTags:
		_currentSong->clearInvalidFileTags();
		break;
	case RemoveEndTag:
		_currentSong->removeEndTag();
		break;
	case SetID3Tags:
		bool artistToID3    = smartSettings().at(0)->value().toBool();
		bool titleToID3     = smartSettings().at(1)->value().toBool();
		bool genreToID3     = smartSettings().at(2)->value().toBool();
		bool yearToID3      = smartSettings().at(3)->value().toBool();
		bool commentToID3   = smartSettings().at(4)->value().toBool();
		bool languageToID3  = smartSettings().at(5)->value().toBool();
		bool bpmToID3       = smartSettings().at(6)->value().toBool();
		bool ulyricsToID3   = smartSettings().at(7)->value().toBool();
		bool slyricsToID3   = smartSettings().at(8)->value().toBool();
		bool artworkCOToID3 = smartSettings().at(9)->value().toBool();
		bool artworkBGToID3 = smartSettings().at(10)->value().toBool();
		
		// currently only support MP3 files (FIXME/TODO)
		if(_currentSong->audio().toLower().endsWith(".mp3")) {
			/*
			// with FileRef --> fails (FIXME/TODO)
			qDebug() << "ID3 song artist (ref) before: " << this->ref().tag()->artist().toCString() << Qt::endl;
			this->ref().tag()->setArtist(QStringToTString(this->_currentSong->artist()));
			bool saveOK = this->ref().save();
			
			qDebug() << "fref().save() returns " << saveOK << Qt::endl;
			qDebug() << "ID3 song artist (ref) after: " << this->ref().tag()->artist().toCString() << Qt::endl;
			*/
			
			//with MPEG::File --> seems to work
			TagLib::MPEG::File f(_currentSong->audioFileInfo().absoluteFilePath().toLocal8Bit().data());
			
			if(artistToID3) {
				if(f.hasID3v1Tag()) { f.ID3v1Tag()->setArtist(_currentSong->artist() == N_A ? TagLib::String() : QStringToTString(_currentSong->artist())); }
				f.ID3v2Tag(true)->setArtist(_currentSong->artist() == N_A ? TagLib::String() : QStringToTString(_currentSong->artist()));
				_currentSong->log(QString(tr("#ARTIST written to ID3 tag of song file: \"%1\"")).arg(_currentSong->filePath()), QU::Information);
			}
			if(titleToID3) {
				if(f.hasID3v1Tag()) { f.ID3v1Tag()->setTitle(_currentSong->title() == N_A ? TagLib::String() : QStringToTString(_currentSong->title())); }
				f.ID3v2Tag(true)->setTitle(_currentSong->title() == N_A ? TagLib::String() : QStringToTString(_currentSong->title()));
				_currentSong->log(QString(tr("#TITLE written to ID3 tag of song file: \"%1\"")).arg(_currentSong->filePath()), QU::Information);
			}
			if(genreToID3) {
				if(f.hasID3v1Tag()) { f.ID3v1Tag()->setGenre(_currentSong->genre() == N_A ? TagLib::String() : QStringToTString(_currentSong->genre())); }
				f.ID3v2Tag(true)->setGenre(_currentSong->genre() == N_A ? TagLib::String() : QStringToTString(_currentSong->genre()));
				_currentSong->log(QString(tr("#GENRE written to ID3 tag of song file: \"%1\"")).arg(_currentSong->filePath()), QU::Information);
			}
			if(yearToID3) {
				if(f.hasID3v1Tag()) { f.ID3v1Tag()->setYear(_currentSong->year() == N_A ? 0 : _currentSong->year().toUInt()); }
				f.ID3v2Tag(true)->setYear(_currentSong->year() == N_A ? 0 : _currentSong->year().toUInt());
				_currentSong->log(QString(tr("#YEAR written to ID3 tag of song file: \"%1\"")).arg(_currentSong->filePath()), QU::Information);
			}
			if(commentToID3) {
				if(f.hasID3v1Tag()) { f.ID3v1Tag()->setComment(_currentSong->comment() == N_A ? TagLib::String() : QStringToTString(_currentSong->comment())); }
				f.ID3v2Tag(true)->setComment(_currentSong->comment() == N_A ? TagLib::String() : QStringToTString(_currentSong->comment()));
				_currentSong->log(QString(tr("#COMMENT written to ID3 tag of song file: \"%1\"")).arg(_currentSong->filePath()), QU::Information);
			}
			if(languageToID3) {
				if(_currentSong->language() != N_A) {
					f.ID3v2Tag(true)->removeFrames("TLAN");
					auto languageFrame = new TagLib::ID3v2::TextIdentificationFrame(TagLib::ByteVector("TLAN"), TagLib::String::UTF8);
					languageFrame->setText(QStringToTString(QUSongSupport::langToLangCode(_currentSong->language())).data(TagLib::String::UTF8));
					f.ID3v2Tag()->addFrame(languageFrame);
					_currentSong->log(QString(tr("#LANGUAGE written to ID3 tag of song file: \"%1\"")).arg(_currentSong->filePath()), QU::Information);
				}
			}
			if(bpmToID3) {
				f.ID3v2Tag(true)->removeFrames("TBPM");
				auto bpmFrame = new TagLib::ID3v2::TextIdentificationFrame(TagLib::ByteVector("TBPM"), TagLib::String::UTF8);
				bpmFrame->setText(QStringToTString(QString::number(QVariant(_currentSong->bpm().toFloat()).toInt())));
				f.ID3v2Tag()->addFrame(bpmFrame);
				_currentSong->log(QString(tr("#BPM written to ID3 tag of song file: \"%1\"")).arg(_currentSong->filePath()), QU::Information);
			}
			if(ulyricsToID3) {
				f.ID3v2Tag(true)->removeFrames("USLT");
				
				QList<QUSongLineInterface*> melody1 = (!_currentSong->isDuet()) ? song->loadMelody() : song->melodyForSinger(QUSongLineInterface::first);
				
				QString ulyrics1;
				
				foreach(QUSongLineInterface *line, melody1) {
					ulyrics1.append(line->syllables().replaceInStrings("~", "").join("")).append("\n");
				}
				
				auto ulyricsFrame1 = new TagLib::ID3v2::UnsynchronizedLyricsFrame;
				ulyricsFrame1->setTextEncoding(TagLib::String::UTF8);
				ulyricsFrame1->setLanguage(QStringToTString(QUSongSupport::langToLangCode(_currentSong->language())).data(TagLib::String::UTF8));
				ulyricsFrame1->setText(QStringToTString(ulyrics1));
				ulyricsFrame1->setDescription(QStringToTString(QString("Lyrics for singer 1")));
				f.ID3v2Tag()->addFrame(ulyricsFrame1);
				
				if(_currentSong->isDuet()) {
					QList<QUSongLineInterface*> melody2 = song->melodyForSinger(QUSongLineInterface::second);
					
					QString ulyrics2;
					
					foreach(QUSongLineInterface *line, melody2) {
						ulyrics2.append(line->syllables().replaceInStrings("~", "").join("")).append("\n");
					}
					
					auto ulyricsFrame2 = new TagLib::ID3v2::UnsynchronizedLyricsFrame;
					ulyricsFrame2->setTextEncoding(TagLib::String::UTF8);
					ulyricsFrame2->setLanguage(QStringToTString(QUSongSupport::langToLangCode(_currentSong->language())).data(TagLib::String::UTF8));
					ulyricsFrame2->setText(QStringToTString(ulyrics2));
					ulyricsFrame2->setDescription(QStringToTString(QString("Lyrics for singer 2")));
					f.ID3v2Tag()->addFrame(ulyricsFrame2);
				}
				
				_currentSong->log(QString(tr("Unsychronized lyrics written to ID3 tag of song file: \"%1\"")).arg(_currentSong->filePath()), QU::Information);
			}
			if(slyricsToID3) {
				f.ID3v2Tag(true)->removeFrames("SYLT");
				
				QList<QUSongLineInterface*> melody1 = (!_currentSong->isDuet()) ? song->loadMelody() : song->melodyForSinger(QUSongLineInterface::first);
				
				auto slyricsFrame1 = new TagLib::ID3v2::SynchronizedLyricsFrame;
				slyricsFrame1->setTextEncoding(TagLib::String::UTF8);
				slyricsFrame1->setLanguage(QStringToTString(QUSongSupport::langToLangCode(_currentSong->language())).data(TagLib::String::UTF8));
				slyricsFrame1->setTimestampFormat(TagLib::ID3v2::SynchronizedLyricsFrame::TimestampFormat::AbsoluteMilliseconds);
				slyricsFrame1->setType(TagLib::ID3v2::SynchronizedLyricsFrame::Type::Lyrics);
				slyricsFrame1->setDescription("Song lyrics for singer 1 retrieved from UltraStar song file via UltraStar Manager.");
				
				TagLib::ID3v2::SynchronizedLyricsFrame::SynchedTextList stl1;
				
				/* each lyrics line separately
				foreach(QUSongLineInterface *line, _currentSong->melodyForSinger(QUSongLineInterface::first)) {
					QString lyricsLine = line->syllables().join("");
					float timestamp = _currentSong->gap().toFloat() + (line->notes().first()->timestamp() * 15.0 * 1000.0 / _currentSong->bpm().toFloat());
					stl1.append(TagLib::ID3v2::SynchronizedLyricsFrame::SynchedText(qRound(timestamp), QStringToTString(lyricsLine)));
				} */
				
				// each syllable separately
				for(int i = 0; i < melody1.size(); ++i) {
					QUSongLineInterface *line = melody1.at(i);
					
					// according to ID3 standard (https://id3.org/id3v2.3.0#Synchronised_lyrics.2Ftext), whitespace and newline characters should go before the respective syllable
					bool shiftSpaceToNextLine = false;
					for(int j = 0; j < line->notes().size(); ++j) {
						QUSongNoteInterface *note = line->notes().at(j);
						QString syllable = note->syllable();
						if(shiftSpaceToNextLine) {
							syllable.prepend(" ");
						}
						if(line != melody1.first() && note == line->notes().first()) {
							syllable.prepend("\n");
						}
						if(syllable.endsWith(" ")) {
							syllable.chop(1);
							shiftSpaceToNextLine = true;
						} else {
							shiftSpaceToNextLine = false;
						}
						
						float timestamp = _currentSong->gap().toFloat() + (note->timestamp() * 15.0 * 1000.0 / _currentSong->bpm().toFloat());
						stl1.append(TagLib::ID3v2::SynchronizedLyricsFrame::SynchedText(qRound(timestamp), QStringToTString(syllable)));
						qDebug() << "Adding (" << QString::number(timestamp) << ", " << syllable << ")" << Qt::endl;
					}
				}
				
				slyricsFrame1->setSynchedText(stl1);
				f.ID3v2Tag()->addFrame(slyricsFrame1);
				
				if(_currentSong->isDuet()) {
					QList<QUSongLineInterface*> melody2 = song->melodyForSinger(QUSongLineInterface::second);
					
					auto slyricsFrame2 = new TagLib::ID3v2::SynchronizedLyricsFrame;
					slyricsFrame2->setTextEncoding(TagLib::String::UTF8);
					slyricsFrame2->setLanguage(QStringToTString(QUSongSupport::langToLangCode(_currentSong->language())).data(TagLib::String::UTF8));
					slyricsFrame2->setTimestampFormat(TagLib::ID3v2::SynchronizedLyricsFrame::TimestampFormat::AbsoluteMilliseconds);
					slyricsFrame2->setType(TagLib::ID3v2::SynchronizedLyricsFrame::Type::Lyrics);
					slyricsFrame2->setDescription("Song lyrics for singer 2 retrieved from UltraStar song file via UltraStar Manager.");
					
					TagLib::ID3v2::SynchronizedLyricsFrame::SynchedTextList stl2;
					
					/* each lyrics line separately
					foreach(QUSongLineInterface *line, melody2) {
						QString lyricsLine = line->syllables().join("");
						float timestamp = _currentSong->gap().toFloat() + (line->notes().first()->timestamp() * 15.0 * 1000.0 / _currentSong->bpm().toFloat());
						stl2.append(TagLib::ID3v2::SynchronizedLyricsFrame::SynchedText(qRound(timestamp), QStringToTString(lyricsLine)));
					} */
					
					// each syllable separately
					foreach(QUSongLineInterface *line, melody2) {
						foreach(QUSongNoteInterface *note, line->notes()) {
							QString syllable = note->syllable();
							float timestamp = _currentSong->gap().toFloat() + (note->timestamp() * 15.0 * 1000.0 / _currentSong->bpm().toFloat());
							stl2.append(TagLib::ID3v2::SynchronizedLyricsFrame::SynchedText(qRound(timestamp), QStringToTString(syllable)));
							//qDebug() << "Adding (" << QString::number(timestamp) << ", " << syllable << ")" << endl;
						}
					}
					
					slyricsFrame2->setSynchedText(stl2);
					f.ID3v2Tag()->addFrame(slyricsFrame2);
				}
				
				//TagLib::ID3v2::FrameList l = f.ID3v2Tag()->frameListMap()["SYLT"];
				//qDebug() << "Number of SYLT frames: " << l.size() << endl;
				
				_currentSong->log(QString(tr("Sychronized lyrics written to ID3 tag of song file: \"%1\"")).arg(_currentSong->filePath()), QU::Information);
			}
			if(artworkCOToID3) {
				if(_currentSong->hasCover()) {
					f.ID3v2Tag(true)->removeFrames("APIC");
					
					TagLib::ID3v2::AttachedPictureFrame *coverframe = new TagLib::ID3v2::AttachedPictureFrame;
					
					coverframe->setType(TagLib::ID3v2::AttachedPictureFrame::FrontCover);
					coverframe->setTextEncoding(TagLib::String::UTF8);
					coverframe->setDescription("Cover image from UltraStar song file embedded via UltraStar Manager.");
					
					QString mimeType = QMimeDatabase().mimeTypeForFile(_currentSong->coverFileInfo().absoluteFilePath()).name();
					if(QString::compare(mimeType, "image/jpeg") == 0 || QString::compare(mimeType, "image/png") == 0) {
						coverframe->setMimeType(QStringToTString(mimeType));
					}
					
					QImage cover(_currentSong->coverFileInfo().absoluteFilePath());
					
					QByteArray bytes;
					QBuffer buffer( &bytes );
					buffer.open( QIODevice::WriteOnly );
					if( !cover.save( &buffer, "JPEG" ) ) {
						buffer.close();
						return;
					}
					buffer.close();
					
					//TagLib::ByteVector bv(cover.byteCount(), cover.constBits());
					//coverframe->setPicture(bv);
					coverframe->setPicture( TagLib::ByteVector( bytes.data(), bytes.length() ) );
					
					f.ID3v2Tag(true)->addFrame(coverframe);
					
					_currentSong->log(QString(tr("Cover image written to ID3 tag of song file: \"%1\"")).arg(_currentSong->filePath()), QU::Information);
				} else {
					_currentSong->log(QString(tr("Song has no cover image to be written to ID3 tag of song file: \"%1\"")).arg(_currentSong->filePath()), QU::Warning);
				}
			}
			if(artworkBGToID3) {
				if(_currentSong->hasBackground()) {
					if(!artworkCOToID3) {
						f.ID3v2Tag(true)->removeFrames("APIC");
					}
					
					TagLib::ID3v2::AttachedPictureFrame *artistframe = new TagLib::ID3v2::AttachedPictureFrame;
					
					artistframe->setType(TagLib::ID3v2::AttachedPictureFrame::Artist);
					artistframe->setTextEncoding(TagLib::String::UTF8);
					artistframe->setDescription("Background image from UltraStar song file embedded via UltraStar Manager.");
					
					QString mimeType = QMimeDatabase().mimeTypeForFile(_currentSong->coverFileInfo().absoluteFilePath()).name();
					if(QString::compare(mimeType, "image/jpeg") == 0 || QString::compare(mimeType, "image/png") == 0) {
						artistframe->setMimeType(QStringToTString(mimeType));
					}
					
					QImage background(_currentSong->backgroundFileInfo().absoluteFilePath());
					
					QByteArray bytes;
					QBuffer buffer( &bytes );
					buffer.open( QIODevice::WriteOnly );
					if( !background.save( &buffer, "JPEG" ) ) {
						buffer.close();
						return;
					}
					buffer.close();
					
					//TagLib::ByteVector bv(cover.byteCount(), cover.constBits());
					//coverframe->setPicture(bv);
					artistframe->setPicture( TagLib::ByteVector( bytes.data(), bytes.length() ) );
					
					f.ID3v2Tag(true)->addFrame(artistframe);
					
					_currentSong->log(QString(tr("Background image written to ID3 tag of song file: \"%1\"")).arg(_currentSong->filePath()), QU::Information);
				} else {
					_currentSong->log(QString(tr("Song has no background image to be written to ID3 tag of song file: \"%1\"")).arg(_currentSong->filePath()), QU::Information);
				}
			}
			f.save();
		} else {
			_currentSong->log(QString(tr("Cannot write song infos to ID3 tags. The audio file type is not yet supported.")), QU::Warning);
		}
		
		break;
	}
}

QList<QUSmartSettingInterface*> QUCleanUpTask::smartSettings() const {
	if(_smartSettings.isEmpty()) {
		switch(_mode) {
		case UnusedFiles:
			_smartSettings.append(new QUSmartCheckBox("cleanup/unusedFiles_audio", tr("Audio files"), true));
			_smartSettings.append(new QUSmartCheckBox("cleanup/unusedFiles_picture", tr("Picture files"), true));
			_smartSettings.append(new QUSmartCheckBox("cleanup/unusedFiles_video", tr("Video files"), true));
			_smartSettings.append(new QUSmartInputCheck("cleanup/unusedFiles_pattern", "\\.xml|\\.sco", nullptr, tr("Pattern:")));
			break;
		case SetID3Tags:
			_smartSettings.append(new QUSmartCheckBox("cleanup/setID3Tags_artist", tr("Write #ARTIST to ID3 artist tag"), false));
			_smartSettings.append(new QUSmartCheckBox("cleanup/setID3Tags_title", tr("Write #TITLE to ID3 title tag"), false));
			_smartSettings.append(new QUSmartCheckBox("cleanup/setID3Tags_genre", tr("Write #GENRE to ID3 genre tag"), false));
			_smartSettings.append(new QUSmartCheckBox("cleanup/setID3Tags_year", tr("Write #YEAR to ID3 year tag"), false));
			_smartSettings.append(new QUSmartCheckBox("cleanup/setID3Tags_comment", tr("Write #COMMENT to ID3 comment tag"), false));
			_smartSettings.append(new QUSmartCheckBox("cleanup/setID3Tags_language", tr("Write #LANGUAGE to ID3 language tag"), false));
			_smartSettings.append(new QUSmartCheckBox("cleanup/setID3Tags_bpm", tr("Write #BPM to ID3 BPM tag"), false));
			_smartSettings.append(new QUSmartCheckBox("cleanup/setID3Tags_ulyrics", tr("Write song lyrics to ID3 unsychronized lyrics tag"), false));
			_smartSettings.append(new QUSmartCheckBox("cleanup/setID3Tags_slyrics", tr("Write song lyrics to ID3 sychronized lyrics tag"), false));
			_smartSettings.append(new QUSmartCheckBox("cleanup/setID3Tags_artwork_co", tr("Write #COVER artwork to ID3 (FrontCover) artwork tag"), false));
			_smartSettings.append(new QUSmartCheckBox("cleanup/setID3Tags_artwork_bg", tr("Write #BACKGROUD artwork to ID3 (Artist) artwork tag"), false));
			break;
		}
	}
	
	return _smartSettings;
}

TagLib::FileRef QUCleanUpTask::ref() {
	if(!_currentSong)
		return TagLib::FileRef();

	return TagLib::FileRef(_currentSong->audioFileInfo().absoluteFilePath().toLocal8Bit().data());
}
