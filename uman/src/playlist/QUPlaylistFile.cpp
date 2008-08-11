#include "QUPlaylistFile.h"
#include "QUMainWindow.h"

#include <QFile>
#include <QRegExp>

QUPlaylistFile::QUPlaylistFile(const QString &filePath, QObject *parent): QObject(parent), _nameChanged(false) {
	_fi.setFile(filePath);

	QFile file(filePath);

	if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QString line = QString::fromLocal8Bit(file.readLine());

		while(!line.startsWith(QString("#%1:").arg(SONGS_TAG), Qt::CaseInsensitive)) {
			if(line.startsWith(QString("#%1:").arg(NAME_TAG), Qt::CaseInsensitive))
				_name = line.mid(QString("#%1:").arg(NAME_TAG).length()).trimmed();
			else if(line.startsWith("#"))
				_comments << line;

			if(file.atEnd())
				break;

			line = QString::fromLocal8Bit(file.readLine());
		}

		while(!file.atEnd()) {
			if(!line.startsWith(QString("#%1:").arg(SONGS_TAG), Qt::CaseInsensitive))
				_playlist.append(new QUPlaylistEntry(line.section(":", 0, 0).trimmed(), line.section(":", 1, 1).trimmed(), this));

			line = QString::fromLocal8Bit(file.readLine());
		}

		// use last line buffer
		// TODO: a little bit dirty here (duplicate code)
		if(!line.startsWith(QString("#%1:").arg(SONGS_TAG), Qt::CaseInsensitive))
			_playlist.append(new QUPlaylistEntry(line.section(":", 0, 0).trimmed(), line.section(":", 1, 1).trimmed(), this));

		file.close();
	}
}

QUPlaylistFile::~QUPlaylistFile() {
	qDeleteAll(_playlist);
	_playlist.clear();

	disconnect(this, 0, 0, 0);
}

/*!
 * Find proper songs for this playlist. All songs has to be loaded for this operation
 * to succeed.
 */
void QUPlaylistFile::connectSongs(const QList<QUSongFile*> &songs) {
	foreach(QUPlaylistEntry *entry, _playlist) {
		entry->connectSong(songs);
	}
}

/*!
 * Removes the connections to songs in all playlist entries.
 */
void QUPlaylistFile::disconnectSongs() {
	foreach(QUPlaylistEntry *entry, _playlist) {
		entry->disconnectSong();
	}
}

/*!
 * \returns directory of the current playlistFile location
 */
QDir QUPlaylistFile::dir() {
	QSettings settings;

	QDir defaultDir = QUMainWindow::BaseDir;
	defaultDir.cdUp();
	defaultDir.cd("playlists"); // default location of playlistFiles, relative to song dir

	return QDir(settings.value("playlistFilePath", defaultDir.path()).toString());
}

void QUPlaylistFile::setDir(const QDir &dir) {
	QSettings settings;

	settings.setValue("playlistFilePath", dir.path());
}

QStringList QUPlaylistFile::allowedTypes() {
	return QString("*.upl").split(" ");
}

bool QUPlaylistFile::save() {
	QFile file(_fi.filePath());

	if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
		emit finished(QString(tr("Save error! The playlist file \"%1\" was NOT saved.")).arg(_fi.fileName()), QU::warning);
		return false;
	}

	// write comments
	foreach(QString comment, _comments) {
		file.write(comment.toLocal8Bit());
	}

	// write name of playlist
	file.write(QString("#%1: %2\n").arg(NAME_TAG).arg(_name).toLocal8Bit());
	_nameChanged = false;

	// write song links
	file.write(QString("#%1:\n").arg(SONGS_TAG).toLocal8Bit());
	foreach(QUPlaylistEntry *entry, _playlist) {
		if(!entry->song())
			emit finished(QString(tr("Warning! The playlist entry \"%1 : %2\" may NOT be found by UltraStar!")).arg(entry->artistLink()).arg(entry->titleLink()), QU::warning);
		else
			entry->setLinks(entry->song()->artist(), entry->song()->title());

		file.write(QString("%1 : %2\n").arg(entry->artistLink()).arg(entry->titleLink()).toLocal8Bit());
	}

	file.close();
	emit finished(QString(tr("The playlist file \"%1\" was saved successfully.")).arg(_fi.fileName()), QU::saving);
	return true;
}

QUPlaylistEntry* QUPlaylistFile::entry(int index) {
	if(index < 0 or index >= _playlist.size())
		return 0;

	return _playlist.at(index);
}

bool QUPlaylistFile::hasUnsavedChanges() const {
	if(_nameChanged or !_fi.exists())
		return true;

	foreach(QUPlaylistEntry *entry, _playlist) {
		if(entry->hasUnsavedChanges())
			return true;
	}

	return false;
}
