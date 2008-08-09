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

#define NAME_TAG  "name"
#define SONGS_TAG "songs"

class QUPlaylistFile: public QObject {
	Q_OBJECT

public:
	QUPlaylistFile(const QString &filePath, QObject *parent = 0);
	~QUPlaylistFile();

	static QDir dir();
	static void setDir(const QDir &dir);
	static QStringList allowedTypes();

	bool save();

	QString name() const { return _name; }
	QFileInfo fileInfo() const { return _fi; }

signals:
	void finished(const QString &message, QU::EventMessageTypes type);

private:
	QFileInfo               _fi;
	QString                 _name;
	QStringList             _comments; // comments at the beginning of a playlist file
	QList<QUPlaylistEntry*> _playlist;
};

#endif /* QUPLAYLISTFILE_H_ */
