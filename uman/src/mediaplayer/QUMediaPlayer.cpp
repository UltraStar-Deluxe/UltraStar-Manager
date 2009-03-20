#include "QUMediaPlayer.h"

#include "QU.h"
#include "QULogService.h"

#include "audioproperties.h"
#include "fileref.h"
#include "tag.h"
#include "tstring.h"

QUSongInfo::QUSongInfo(QUSongFile *song) {
	melody = song->melody();
	bpm = QVariant(song->bpm().replace(",", ".")).toDouble() * 4;
	gap = QVariant(song->gap().replace(",", ".")).toDouble();
	isRelative = song->relative() != N_A;

	filePath = song->mp3FileInfo().filePath();
	artist = song->artist();
	title = song->title();
	length = song->lengthMp3();

	bitrate = 0;
	channels = 0;
	sampleRate = 0;

	TagLib::FileRef ref(song->mp3FileInfo().absoluteFilePath().toLocal8Bit().data());
	if(!ref.isNull()) {
		album = TStringToQString(ref.tag()->album());
		if(ref.audioProperties()) {
			bitrate = ref.audioProperties()->bitrate();
			channels = ref.audioProperties()->channels();
			sampleRate = ref.audioProperties()->sampleRate();
		}
	}
}

QUMediaPlayer::QUMediaPlayer(QWidget *parent): QWidget(parent) {
	setupUi(this);

	connect(stopBtn, SIGNAL(clicked()), this, SLOT(stop()));
	connect(prevBtn, SIGNAL(clicked()), this, SLOT(prev()));
	connect(nextBtn, SIGNAL(clicked()), this, SLOT(next()));

	connect(this, SIGNAL(stateChanged(QUMediaPlayer::States)), this, SLOT(updatePlayerControls(QUMediaPlayer::States)));
	connect(this, SIGNAL(stateChanged(QUMediaPlayer::States)), this, SLOT(updateInfoLabel(QUMediaPlayer::States)));

	if(!BASS_Init(-1, 44100, 0, 0, NULL))
		logSrv->add("BASS was NOT loaded.", QU::warning);

	_mediaStream = 0;
	_currentSongIndex = -1;

	setState(QUMediaPlayer::stopped);
}

QUMediaPlayer::~QUMediaPlayer() {
	stop();

	if(!BASS_Free())
		logSrv->add("BASS was NOT unloaded.", QU::warning);
}

void QUMediaPlayer::setSongs(const QList<QUSongFile*> &songs) {
	_songs.clear();
	foreach(QUSongFile *song, songs) {
		_songs << QUSongInfo(song);
	}
	_currentSongIndex = 0;
}

void QUMediaPlayer::play() {	
	if(_mediaStream)
		return;	// already playing

	if(_currentSongIndex < 0 || _currentSongIndex >= _songs.size())
		return; // invalid index

	QUSongInfo info = _songs.at(_currentSongIndex);

	_mediaStream = BASS_StreamCreateFile(FALSE, info.filePath.toLocal8Bit().data(), 0, 0, BASS_STREAM_PRESCAN);
	autocue->reset(info.melody, info.bpm, info.gap, info.isRelative);

	BASS_Play();
	autocue->play();
	setState(QUMediaPlayer::playing);
}

void QUMediaPlayer::stop() {
	BASS_StopAndFree();
	_mediaStream = 0;
	autocue->stop();
	setState(QUMediaPlayer::stopped);
}

void QUMediaPlayer::pause() {
	setState(QUMediaPlayer::paused);
}

void QUMediaPlayer::prev() {
	this->stop();
	this->_currentSongIndex = qMax(_currentSongIndex - 1, 0);
	this->play();
}

void QUMediaPlayer::next() {
	this->stop();
	this->_currentSongIndex = qMin(_currentSongIndex + 1, _songs.size() - 1);
	this->play();
}

void QUMediaPlayer::updateTime() {
	if(!_mediaStream)
		return;

	if(_currentSongIndex < 0 || _currentSongIndex >= _songs.size())
		return; // invalid index

	QUSongInfo info = _songs.at(_currentSongIndex);

	QWORD len = BASS_ChannelGetLength(_mediaStream, BASS_POS_BYTE);
	QWORD pos = BASS_ChannelGetPosition(_mediaStream, BASS_POS_BYTE);
	int posSec = (info.length * pos) / len;
	timeLbl->setText(QString("%1:%2 / %3:%4")
			.arg(posSec / 60)
			.arg(posSec % 60, 2, 10, QChar('0'))
			.arg(info.length / 60)
			.arg(info.length % 60, 2, 10, QChar('0')));

	QTimer::singleShot(1000, this, SLOT(updateTime()));
}

void QUMediaPlayer::requestSongs() {
	if(_mediaStream)
		stop();

	if(sourceCombo->currentIndex() == 0) { // selected songs
		emit selectedSongsRequested();
	} else if(sourceCombo->currentIndex() == 1) { // all songs
		emit allSongsRequested();
	} else if(sourceCombo->currentIndex() == 2) { // visible songs
		emit visibleSongsRequested();
	} else if(sourceCombo->currentIndex() == 3) { // current playlist
		emit currentPlaylistRequested();
	}
}

void QUMediaPlayer::updatePlayerControls(QUMediaPlayer::States state) {
	if(state.testFlag(QUMediaPlayer::paused) || state.testFlag(QUMediaPlayer::playing)) {
		prevBtn->setEnabled(_songs.size() > 0 && _currentSongIndex > 0);
		nextBtn->setEnabled(_songs.size() > 0 && _currentSongIndex < _songs.size() - 1);
		stopBtn->setEnabled(true);
	} else if(state.testFlag(QUMediaPlayer::stopped)) {
		prevBtn->setEnabled(false);
		nextBtn->setEnabled(false);
		stopBtn->setEnabled(false);
	}

	if(state.testFlag(QUMediaPlayer::playing)) {
		playBtn->setIcon(QIcon(":/player/pause.png"));
		disconnect(playBtn, 0, 0, 0);
		connect(playBtn, SIGNAL(clicked()), this, SLOT(pause()));
	} else {
		playBtn->setIcon(QIcon(":/player/play.png"));
		disconnect(playBtn, 0, 0, 0);
		connect(playBtn, SIGNAL(clicked()), this, SLOT(requestSongs()));
	}
}

void QUMediaPlayer::updateInfoLabel(QUMediaPlayer::States state) {
	if(state.testFlag(QUMediaPlayer::playing)) {
		if(_currentSongIndex < 0 || _currentSongIndex >= _songs.size())
			return; // invalid index

		QUSongInfo info = _songs.at(_currentSongIndex);
		currentSongLbl->setText(QString("%1<br><b>%2</b>").arg(info.artist).arg(info.title));
		infoIconLbl->setToolTip(
				QString("Bitrate: <b>%1</b> kbps<br>Channels: <b>%2</b><br>Sample Rate: <b>%3</b> kHz")
				.arg(info.bitrate)
				.arg(info.channels)
				.arg(info.sampleRate));
		updateTime();
	} else if(state.testFlag(QUMediaPlayer::stopped)) {
		currentSongLbl->setText(N_A);
		timeLbl->setText("-:--");
		infoIconLbl->setToolTip("");
		autocue->setText(tr("<i>Hit the play-button to fetch all songs of the selected list below. Then the first song will start playing.</i>"));
	}
}

void QUMediaPlayer::setState(States newState) {
	_state = newState;
	emit stateChanged(newState);
}

void QUMediaPlayer::BASS_StopAndFree() {
	if(!_mediaStream)
		return;

	if(!BASS_ChannelStop(_mediaStream)) {
		logSrv->add(QString("BASS ERROR: %1").arg(BASS_ErrorGetCode()), QU::warning);
		return;
	}

	if(!BASS_StreamFree(_mediaStream)) {
		logSrv->add(QString("BASS ERROR: %1").arg(BASS_ErrorGetCode()), QU::warning);
		return;
	}
}

void QUMediaPlayer::BASS_Play() {
	if(!_mediaStream) {
		logSrv->add(QString("BASS ERROR: %1").arg(BASS_ErrorGetCode()), QU::warning);
		return;
	}

	if(!BASS_ChannelPlay(_mediaStream, TRUE)) {
		logSrv->add(QString("BASS ERROR: %1").arg(BASS_ErrorGetCode()), QU::warning);
		return;
	}
}
