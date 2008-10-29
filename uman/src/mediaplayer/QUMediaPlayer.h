#ifndef QUMEDIAPLAYER_H_
#define QUMEDIAPLAYER_H_

#include "QUSongFile.h"

#include <QObject>
#include <QTimer>

#include "bass.h"
#include "ui_QUMediaPlayer.h"

class QUMediaPlayer: public QWidget, private Ui::QUMediaPlayer {
	Q_OBJECT

public:
	QUMediaPlayer(QWidget *parent = 0);
	~QUMediaPlayer();

	void setCurrentSong(QUSongFile &song);

public slots:
	void play();
	void stop();

	void updateTime();

signals:
	void currentSongRequested();

private:
	HSTREAM _mediaStream;
	QString _currentArtist;
	QString _currentTitle;
	QString _currentMp3FilePath;
	int _mp3Length;
};

#endif /* QUMEDIAPLAYER_H_ */
