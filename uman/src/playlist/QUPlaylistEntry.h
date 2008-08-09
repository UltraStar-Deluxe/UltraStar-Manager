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
	QUPlaylistEntry(const QString &artist, const QString & title, QObject *parent = 0);

	QUSongFile* song() const { return _song; }

	QString artistLink() const { return _artistLink; }
	QString titleLink() const { return _titleLink; }

	void connectSong(QUSongFile* song);
	void connectSong(const QList<QUSongFile*> &songs);

private:
	QUSongFile *_song;

	// links that should be used to find the real songs
	QString     _artistLink;
	QString     _titleLink;
};

#endif // QUPLAYLISTENTRY_H_
