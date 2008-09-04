#ifndef QUPLAYLISTFILE_H_
#define QUPLAYLISTFILE_H_

#include "QU.h"
#include "QUSongFile.h"
#include "QUPlaylistEntry.h"

#include <QObject>
#include <QFileInfo>
#include <QList>
#include <QSettings>
#include <QStringList>

#define NAME_TAG  "Name"
#define SONGS_TAG "Songs"

class QUPlaylistFile: public QObject {
	Q_OBJECT

public:
	QUPlaylistFile(const QString &filePath, QObject *parent = 0);
	~QUPlaylistFile();

	void connectSongs(const QList<QUSongFile*> &songs);
	void disconnectSongs();

	static QDir dir();
	static void setDir(const QDir &dir);

	bool save();

	void setName(const QString &name) { _name = name; _nameChanged = true; }
	QString name() const { return _name; }

	void setFileInfo(const QFileInfo &newFi) { _fi = newFi; }
	QFileInfo fileInfo() const { return _fi; }

	int count() const { return _playlist.count(); }
	QUPlaylistEntry* entry(int index);

	bool hasUnsavedChanges() const;

	bool addEntry(QUSongFile *song);
	bool removeEntry(QUPlaylistEntry *entry);

	void changeOrder(const QList<QUPlaylistEntry*> &newOrder);

	QList<QUSongFile*> connectedSongs() const;

signals:
	void finished(const QString &message, QU::EventMessageTypes type);

private:
	QFileInfo               _fi;
	QString                 _name;
	QStringList             _comments; // comments at the beginning of a playlist file
	QList<QUPlaylistEntry*> _playlist;

	bool _nameChanged;
	bool _playlistChanged;
};

#endif /* QUPLAYLISTFILE_H_ */
