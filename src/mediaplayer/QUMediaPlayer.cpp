#include "QUMediaPlayer.h"

#include "QU.h"
#include "QULogService.h"
#include "QUSongDatabase.h"

#include <QRandomGenerator>

#include "audioproperties.h"
#include "fileref.h"
#include "tag.h"
#include "tstring.h"

QUSongInfo::QUSongInfo(QUSongFile *song) {
	_isValid = song->hasMp3();

	gap1 = QVariant(song->gap().replace(",", ".")).toDouble();
	gap2 = gap1;

	bpm = QVariant(song->bpm().replace(",", ".")).toDouble() * 4;
	isRelative = song->isRelative();

	filePath = song->mp3FileInfo().filePath();
	artist = song->artist();
	title = song->title();
	length = song->lengthMp3();
	lengthAudio = song->lengthAudioFile();

	if(song->isDuet()) {
		melody1 = song->melodyForSinger(QUSongLineInterface::first);
		melody2 = song->melodyForSinger(QUSongLineInterface::second);
	} else {
		melody1 = song->loadMelody();
	}

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
	_player = new QMediaPlayer;

	setupUi(this);

	connect(stopBtn, SIGNAL(clicked()), this, SLOT(stop()));
	connect(prevBtn, SIGNAL(clicked()), this, SLOT(prev()));
	connect(nextBtn, SIGNAL(clicked()), this, SLOT(next()));

	connect(this, SIGNAL(stateChanged(QUMediaPlayer::States)), this, SLOT(updatePlayerControls(QUMediaPlayer::States)));
	connect(this, SIGNAL(stateChanged(QUMediaPlayer::States)), this, SLOT(updateInfoLabel(QUMediaPlayer::States)));
	connect(this, SIGNAL(stateChanged(QUMediaPlayer::States)), this, SLOT(updateTimeSlider(QUMediaPlayer::States)));

	connect(shuffleBtn, SIGNAL(toggled(bool)), this, SLOT(loopShuffleToggled()));
	connect(loopBtn, SIGNAL(toggled(bool)), this, SLOT(loopShuffleToggled()));

	connect(timeSlider, SIGNAL(sliderReleased()), this, SLOT(seek()));

	connect(autocue, SIGNAL(editSongRequested(int)), this, SLOT(requestSongEdit(int)));
	connect(autocue2, SIGNAL(editSongRequested(int)), this, SLOT(requestSongEdit(int)));

	timeSlider->setMinimum(0);
	timeSlider->setSingleStep(1);

	_currentSongIndex = -1;

	setState(QUMediaPlayer::stopped);
}

QUMediaPlayer::~QUMediaPlayer() {
	stop();
}

void QUMediaPlayer::setSongs(const QList<QUSongFile*> &songs, Source src) {
	if(src != NoChange) {
		sourceCombo->setCurrentIndex(src);
	}

	_songs.clear();
	foreach(QUSongFile *song, songs) {
		_songs << QUSongInfo(song);
	}
	_currentSongIndex = -1;
	_lastIndices.clear();
	_freeIndices.clear();
	for(int i = 0; i < _songs.size(); ++i) {
		_freeIndices << i;
	}
}

void QUMediaPlayer::play() {
	if(_player->playbackState() == QMediaPlayer::PlayingState)
		return;	// already playing

	if(_currentSongIndex == -1) {
		next();
		return;
	}

	if(_currentSongIndex < 0 || _currentSongIndex >= _songs.size())
		return; // invalid index

	QUSongInfo info = _songs.at(_currentSongIndex);
	_freeIndices.removeAll(_currentSongIndex);

	if(!info.isValid()) {
		logSrv->add(tr("[Media Player] The song \"%1 - %2\" has no valid audio file.").arg(info.artist, info.title), QU::Warning);
		loopBtn->setChecked(false); // avoid endless-loops
		next();
		return;
	}

	_player->setSource(QUrl::fromLocalFile(info.filePath));

	autocue2->setVisible(!info.melody2.isEmpty());

	autocue->reset(info.melody1, info.bpm, info.gap1, info.isRelative);
	autocue2->reset(info.melody2, info.bpm, info.gap2, info.isRelative);

	_player->play();
	autocue->play(); autocue2->play();
	setState(QUMediaPlayer::playing);
}

void QUMediaPlayer::stop() {
	_player->stop();
	autocue->stop(); autocue2->stop();
	setState(QUMediaPlayer::stopped);
}

void QUMediaPlayer::pause() {
	if(state() == QUMediaPlayer::paused)
		return;

	_player->pause();
	autocue->pause(); autocue2->pause();
	setState(QUMediaPlayer::paused);
}

void QUMediaPlayer::resume() {
	if(state() != QUMediaPlayer::paused)
		return;

	autocue->resume(_player->position());
	autocue2->resume(_player->position());
	_player->play();
	setState(QUMediaPlayer::playing);
}

void QUMediaPlayer::prev() {
	if(_songs.isEmpty())
		return;

	this->stop();

	// any song before?
	if(_lastIndices.isEmpty())
		return;

	// which song now?
	_freeIndices.prepend(_lastIndices.last());
	_lastIndices.removeLast();
	_freeIndices.prepend(_lastIndices.last());
	//qStableSort(_freeIndices); // deprecated
	std::stable_sort(_freeIndices.begin(), _freeIndices.end());
	this->_currentSongIndex = _lastIndices.last();

	this->play();
}

void QUMediaPlayer::next() {
	if(_songs.isEmpty())
		return;

	this->stop();

	// any song left?
	if(_freeIndices.isEmpty()) {
		if(loopBtn->isChecked()) {
			for(int i = 0; i < _songs.size(); ++i)
				_freeIndices << i;
		} else {
			this->stop();
			return;
		}
	}

	// which song next?
	if(shuffleBtn->isChecked()) {
		_lastIndices << _freeIndices.at(QRandomGenerator::global()->generate() % _freeIndices.size());
	} else {
		_lastIndices << _freeIndices.first();
	}
	this->_currentSongIndex = _lastIndices.last();
	this->play();
}

void QUMediaPlayer::seek() {
	int pos = timeSlider->sliderPosition();

	if(state() == QUMediaPlayer::stopped)
		return;

	pause();

	_player->setPosition(pos);
	_player->play();
	autocue->seek(_player->position());
	autocue2->seek(_player->position());
	setState(QUMediaPlayer::playing);
}

void QUMediaPlayer::updateTime() {
	if(state() == QUMediaPlayer::stopped)
		return;

	if(_currentSongIndex < 0 || _currentSongIndex >= _songs.size())
		return; // invalid index

	QUSongInfo info = _songs.at(_currentSongIndex);

	int pos = int(_player->position());
	timeLbl->setText(QString("%1:%2 / %3:%4")
			.arg(pos / 1000 / 60)
			.arg(pos / 1000 % 60, 2, 10, QChar('0'))
			.arg(info.length / 60)
			.arg(info.length % 60, 2, 10, QChar('0')));

	if(!timeSlider->isSliderDown())
		timeSlider->setValue(pos);

	if(pos / 1000 < info.length && pos != -1)
		QTimer::singleShot(1000, this, SLOT(updateTime()));
	else
		next();
}

void QUMediaPlayer::requestSongs() {
	//if(_mediaStream)
	//	stop();
	if(_player->playbackState() == QMediaPlayer::PlayingState)
		_player->stop();

	if(sourceCombo->currentIndex() == SelectedSongs) {
		emit selectedSongsRequested();
	} else if(sourceCombo->currentIndex() == AllSongs) {
		emit allSongsRequested();
	} else if(sourceCombo->currentIndex() == VisibleSongs) {
		emit visibleSongsRequested();
	} else if(sourceCombo->currentIndex() == CurrentPlaylist) {
		emit currentPlaylistRequested();
	}
}

void QUMediaPlayer::updatePlayerControls(QUMediaPlayer::States state) {
	if(state.testFlag(QUMediaPlayer::paused) || state.testFlag(QUMediaPlayer::playing)) {
		prevBtn->setEnabled(_lastIndices.size() > 1);
		nextBtn->setEnabled(loopBtn->isChecked() || (!_freeIndices.isEmpty()));
		stopBtn->setEnabled(true);
		sourceCombo->setEnabled(false);
	} else if(state.testFlag(QUMediaPlayer::stopped)) {
		prevBtn->setEnabled(false);
		nextBtn->setEnabled(false);
		stopBtn->setEnabled(false);
		sourceCombo->setEnabled(true);
	}

	if(state.testFlag(QUMediaPlayer::playing)) {
		playBtn->setIcon(QIcon(":/player/pause.png"));
		disconnect(playBtn, nullptr, nullptr, nullptr);
		connect(playBtn, SIGNAL(clicked()), this, SLOT(pause()));
	} else if(state.testFlag(QUMediaPlayer::paused)) {
		playBtn->setIcon(QIcon(":/player/play.png"));
		disconnect(playBtn, nullptr, nullptr, nullptr);
		connect(playBtn, SIGNAL(clicked()), this, SLOT(resume()));
	} else {
		playBtn->setIcon(QIcon(":/player/play.png"));
		disconnect(playBtn, nullptr, nullptr, nullptr);
		connect(playBtn, SIGNAL(clicked()), this, SLOT(requestSongs()));
	}
}

void QUMediaPlayer::updateInfoLabel(QUMediaPlayer::States state) {
	if(state.testFlag(QUMediaPlayer::playing) || state.testFlag(QUMediaPlayer::paused)) {
		if(_currentSongIndex < 0 || _currentSongIndex >= _songs.size())
			return; // invalid index

		QUSongInfo info = _songs.at(_currentSongIndex);
		currentSongLbl->setText(QString("%1<br><b>%2</b>").arg(info.artist, info.title));
		infoIconLbl->setToolTip(
				QString(tr("Bitrate: <b>%1</b> kbit/s<br>Channels: <b>%2</b><br>Sample Rate: <b>%3</b> kHz"))
				.arg(info.bitrate)
				.arg(info.channels)
				.arg(info.sampleRate));
		updateTime();
	} else if(state.testFlag(QUMediaPlayer::stopped)) {
		currentSongLbl->setText(N_A);
		timeLbl->setText("-:--");
		infoIconLbl->setToolTip("");
		autocue->setText(tr("<i>Hit the play-button to fetch all songs of the selected list below. Then the first song will start playing.</i>"));
		autocue2->setText(tr("<i>Used for duet songs.</i>"));
	}
}

void QUMediaPlayer::updateTimeSlider(QUMediaPlayer::States state) {
	if(state.testFlag(QUMediaPlayer::playing) || state.testFlag(QUMediaPlayer::paused)) {
		if(_currentSongIndex < 0 || _currentSongIndex >= _songs.size())
			return; // invalid index
		QUSongInfo info = _songs.at(_currentSongIndex);

		timeSlider->setEnabled(true);
		timeSlider->setMaximum(info.length * 1000);
	} else if(state.testFlag(QUMediaPlayer::stopped)) {
		timeSlider->setEnabled(false);
	}
}

void QUMediaPlayer::loopShuffleToggled() {
	this->updatePlayerControls(state());
}

void QUMediaPlayer::requestSongEdit(int line) {
	if(state().testFlag(QUMediaPlayer::playing) || state().testFlag(QUMediaPlayer::paused)) {
		if(_currentSongIndex < 0 || _currentSongIndex >= _songs.size())
			return; // invalid index
		QUSongInfo info = _songs.at(_currentSongIndex);

		foreach(QUSongFile *song, songDB->songs()) {
			if(song->artist() == info.artist && song->title() == info.title) {
				emit editSongRequested(song, line);
				return;
			}
		}
	}
}

void QUMediaPlayer::setState(States newState) {
	_state = newState;
	emit stateChanged(newState);
}
