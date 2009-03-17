#ifndef QUMEDIAPLAYER_H_
#define QUMEDIAPLAYER_H_

#include "QUSongFile.h"

#include <QObject>
#include <QTimer>

#include "bass.h"
#include "ui_QUMediaPlayer.h"

class QUSongInfo {
public:
	QUSongInfo(QUSongFile *song);

	QList<QUSongLine*> melody;
	double bpm;
	double gap;
	bool isRelative;

	// properties from usdx song
	QString filePath;
	QString artist;
	QString title;
	int length; // from audio file, can be reset through #END tag

	// properties from id3tag
	QString album;
	int bitrate;
	int channels;
	int sampleRate;
};

class QUMediaPlayer: public QWidget, private Ui::QUMediaPlayer {
	Q_OBJECT

public:
	QUMediaPlayer(QWidget *parent = 0);
	~QUMediaPlayer();

	void setSongs(const QList<QUSongFile*> &songs);

public slots:
	void play();
	void stop();
	void pause();
	void prev();
	void next();

	void updateTime();

	void requestSongs();

signals:
	void selectedSongsRequested();
	void allSongsRequested();
	void visibleSongsRequested();
	void currentPlaylistRequested();

private:
	HSTREAM _mediaStream;
	QList<QUSongInfo> _songs;
	int _currentSongIndex;
};

#endif /* QUMEDIAPLAYER_H_ */
