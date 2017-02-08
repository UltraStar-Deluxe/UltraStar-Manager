#ifndef QUPLAYLISTENTRY_H_
#define QUPLAYLISTENTRY_H_

#include "QU.h"
#include "QUSongFile.h"

#include <QObject>
#include <QString>
#include <QList>

class QUPlaylistEntry: public QObject {
	Q_OBJECT

public:
	QUPlaylistEntry(const QString &artist, const QString &title, const QString &gameMode, QObject *parent = 0);

	QUSongFile* song() const { return _song; }

	QString artistLink() const { return _artistLink; }
	QString titleLink() const { return _titleLink; }
	QString gameMode() const { return _gameMode; }
	void setLinks(const QString &artistLink, const QString &titleLink);
	void setGameMode(const QString &gameMode);

	void connectSong(QUSongFile* song);
	void connectSong();
	void disconnectSong() { _song = 0; }

	bool hasUnsavedChanges() const;

public slots:
	void disconnectSong(QUSongFile *song);

signals:
	void disconnected();

private:
	QUSongFile *_song;

	// textual links that should be used to find the real songs
	QString _artistLink;
	QString _titleLink;
	QString _gameMode;
};

#endif // QUPLAYLISTENTRY_H_
