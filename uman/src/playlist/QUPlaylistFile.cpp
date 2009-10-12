#include "QUPlaylistFile.h"
#include "QULogService.h"
#include "QUPlaylistDatabase.h"

#include <QFile>
#include <QRegExp>

QUPlaylistFile::QUPlaylistFile(QObject *parent):
		QObject(parent),
		_nameChanged(false),
		_playlistChanged(false)
{
	_fi.setFile(playlistDB->dir(), "unnamed.upl");
	setName(tr("New Playlist"));
}

QUPlaylistFile::QUPlaylistFile(const QString &filePath, QObject *parent): QObject(parent), _nameChanged(false), _playlistChanged(false) {
	_fi.setFile(filePath);
	load();
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
void QUPlaylistFile::connectSongs() {
	foreach(QUPlaylistEntry *entry, _playlist)
		entry->connectSong();
}

/*!
 * Removes the connections to songs in all playlist entries.
 */
void QUPlaylistFile::disconnectSongs() {
	foreach(QUPlaylistEntry *entry, _playlist)
		entry->disconnectSong();
}


bool QUPlaylistFile::save() {
	QFile file(_fi.filePath());

	if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
		logSrv->add(QString(tr("Save error! The playlist file \"%1\" was NOT saved.")).arg(_fi.filePath()), QU::Warning);
		return false;
	}

	// write comments
	foreach(QString comment, _comments) {
		file.write(comment.toLocal8Bit());
		file.write("\n");
	}

	// write name of playlist
	file.write(QString("#%1: %2\n").arg(NAME_TAG).arg(_name).toLocal8Bit());
	_nameChanged = false;
	_playlistChanged = false;

	// write song links
	file.write(QString("#%1:\n").arg(SONGS_TAG).toLocal8Bit());
	foreach(QUPlaylistEntry *entry, _playlist) {
		if(!entry->song())
			logSrv->add(QString(tr("Warning! The playlist entry \"%1 - %2\" will NOT be found by UltraStar!")).arg(entry->artistLink()).arg(entry->titleLink()), QU::Warning);
		else
			entry->setLinks(entry->song()->artist(), entry->song()->title());

		file.write(QString("%1 : %2\n").arg(entry->artistLink()).arg(entry->titleLink()).toLocal8Bit());
	}

	file.close();
	logSrv->add(QString(tr("The playlist file \"%1\" was saved successfully.")).arg(_fi.filePath()), QU::Saving);
	return true;
}

QUPlaylistEntry* QUPlaylistFile::entry(int index) {
	if(index < 0 or index >= _playlist.size())
		return 0;

	return _playlist.at(index);
}

QUPlaylistEntry* QUPlaylistFile::last() {
	return entry(count() - 1);
}

bool QUPlaylistFile::hasUnsavedChanges() const {
	if(_nameChanged or _playlistChanged or !_fi.exists())
		return true;

	foreach(QUPlaylistEntry *entry, _playlist) {
		if(entry->hasUnsavedChanges())
			return true;
	}

	return false;
}

/*!
 * You can only add each song once. It's more like a subset than a playlist.
 */
bool QUPlaylistFile::addEntry(QUSongFile *song) {
	if(!song)
		return false; // only add entries with valid songs

	foreach(QUPlaylistEntry *entry, _playlist) { // TOOO: a progressbar helpful?! (for big playlists)
		if(song == entry->song()) {
			logSrv->add(QString(tr("The song \"%1 - %2\" is already in the current playlist. It was not added.")).arg(song->artist()).arg(song->title()), QU::Warning);
			return false;
		}
	}

	QUPlaylistEntry *newEntry = new QUPlaylistEntry(song->artist(), song->title(), this);
	newEntry->connectSong(song);

	_playlist.append(newEntry);
	_playlistChanged = true;
	return true;
}

bool QUPlaylistFile::removeEntry(QUPlaylistEntry *entry) {
	if(!entry)
		return false;

	_playlist.removeAll(entry); // return value should be 1 (or 0)
	_playlistChanged = true;
	return true;
}

void QUPlaylistFile::removeDisconnectedEntries() {
	foreach(QUPlaylistEntry *entry, _playlist)
		if(!entry->song())
			removeEntry(entry);
}

void QUPlaylistFile::changeOrder(const QList<QUPlaylistEntry*> &newOrder) {
	if(newOrder.size() != this->count())
		return;

	foreach(QUPlaylistEntry *entry, newOrder) {
		if(!_playlist.contains(entry)) {
			logSrv->add(QString("The order of the playlist \"%1\" could NOT be changed. The new order misses an entry.").arg(name()), QU::Warning);
			return;
		}
	}

	_playlist = newOrder;
	_playlistChanged = true;
}

/*!
 * \returns A list of all connected songs.
 */
QList<QUSongFile*> QUPlaylistFile::connectedSongs() const {
	QList<QUSongFile*> result;

	foreach(QUPlaylistEntry *entry, _playlist) {
		if(entry->song()) // can be NULL, if not connected to database
			result.append(entry->song());
	}

	return result;
}

void QUPlaylistFile::load() {
	QFile file(_fi.filePath());

	if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QString line = QString::fromLocal8Bit(file.readLine()).trimmed();

		while(!line.startsWith(QString("#%1:").arg(SONGS_TAG), Qt::CaseInsensitive)) {
			if(line.startsWith(QString("#%1:").arg(NAME_TAG), Qt::CaseInsensitive))
				_name = line.mid(QString("#%1:").arg(NAME_TAG).length()).trimmed();
			else if(line.startsWith("#"))
				_comments << line;

			if(file.atEnd())
				break;

			line = QString::fromLocal8Bit(file.readLine()).trimmed();
		}

		while(!file.atEnd()) {
			if(!line.startsWith(QString("#%1:").arg(SONGS_TAG), Qt::CaseInsensitive))
				_playlist.append(new QUPlaylistEntry(line.section(":", 0, 0).trimmed(), line.section(":", 1, 1).trimmed(), this));

			line = QString::fromLocal8Bit(file.readLine()).trimmed();
		}

		// use last line buffer
		// TODO: a little bit dirty here (duplicate code)
		if(!line.startsWith(QString("#%1:").arg(SONGS_TAG), Qt::CaseInsensitive))
			_playlist.append(new QUPlaylistEntry(line.section(":", 0, 0).trimmed(), line.section(":", 1, 1).trimmed(), this));

		file.close();
	}

	connectSongs();
}
