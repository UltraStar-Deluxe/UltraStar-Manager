#include "QUMediaPlayer.h"

#include "QU.h"
#include "QULogService.h"

QUMediaPlayer::QUMediaPlayer(QWidget *parent): QWidget(parent) {
	setupUi(this);

	connect(playBtn, SIGNAL(clicked()), this, SIGNAL(currentSongRequested()));
	connect(stopBtn, SIGNAL(clicked()), this, SLOT(stop()));

	if(!BASS_Init(-1, 44100, 0, 0, NULL))
		logSrv->add("BASS was NOT loaded.", QU::warning);

	_mediaStream = 0;
}

QUMediaPlayer::~QUMediaPlayer() {
	stop();

	if(!BASS_Free())
		logSrv->add("BASS was NOT unloaded.", QU::warning);
}

void QUMediaPlayer::setCurrentSong(QUSongFile &song) {
	if(_mediaStream)
		stop();

	_currentArtist = song.artist();
	_currentTitle = song.title();
	_currentMp3FilePath = song.mp3FileInfo().filePath();
	_mp3Length = song.lengthMp3();

	autocue->reset(song);

	play();
}

void QUMediaPlayer::play() {
	_mediaStream = BASS_StreamCreateFile(FALSE, _currentMp3FilePath.toLocal8Bit().data(), 0, 0, BASS_STREAM_PRESCAN);

	if(!_mediaStream) {
		logSrv->add(QString("BASS ERROR: %1").arg(BASS_ErrorGetCode()), QU::warning);
		return;
	}

	if(!BASS_ChannelPlay(_mediaStream, TRUE)) {
		logSrv->add(QString("BASS ERROR: %1").arg(BASS_ErrorGetCode()), QU::warning);
		return;
	}

	autocue->play();
	artistLbl->setText(_currentArtist);
	titleLbl->setText(_currentTitle);

	updateTime();
}

void QUMediaPlayer::stop() {
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

	_mediaStream = 0;

	artistLbl->setText("-");
	titleLbl->setText("-");
	timeLbl->setText("-");

	autocue->stop();
}

void QUMediaPlayer::updateTime() {
	if(!_mediaStream)
		return;

	QWORD len = BASS_ChannelGetLength(_mediaStream, BASS_POS_BYTE);
	QWORD pos = BASS_ChannelGetPosition(_mediaStream, BASS_POS_BYTE);
	int posSec = (_mp3Length * pos) / len;
	timeLbl->setText(QString("%1:%2 / %3:%4")
			.arg(posSec / 60)
			.arg(posSec % 60, 2, 10, QChar('0'))
			.arg(_mp3Length / 60)
			.arg(_mp3Length % 60, 2, 10, QChar('0')));

	QTimer::singleShot(1000, this, SLOT(updateTime()));
}
