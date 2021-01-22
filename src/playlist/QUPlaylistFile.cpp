#include "QUPlaylistFile.h"
#include "QULogService.h"
#include "QUPlaylistDatabase.h"
#include "QUSongSupport.h"

#include <QFile>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QTextStream>
#include <QTextCodec>

QUPlaylistFile::QUPlaylistFile(QObject *parent):
		QObject(parent),
		_nameChanged(false),
		_playlistChanged(false)
{
	_fi.setFile(playlistDB->dir(), tr("unnamed.upl"));
	setName(tr("New Playlist"));
}

QUPlaylistFile::QUPlaylistFile(const QString &filePath, QObject *parent): QObject(parent), _nameChanged(false), _playlistChanged(false) {
	_fi.setFile(filePath);
	load();
}

QUPlaylistFile::~QUPlaylistFile() {
	qDeleteAll(_playlist);
	_playlist.clear();

	disconnect(this, nullptr, nullptr, nullptr);
}

/*!
 * Find proper songs for this playlist. All songs have to be loaded for this operation
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
		logSrv->add(QString(tr("Save error! The playlist file \"%1\" was NOT saved.")).arg(QDir::toNativeSeparators(_fi.filePath())), QU::Warning);
		return false;
	}

	if(QUSongSupport::allowedVocaluxePlaylistFiles().contains("*." + _fi.suffix())) {
		/* write Vocaluxe XML */
		QXmlStreamWriter* xmlWriter = new QXmlStreamWriter();
		xmlWriter->setDevice(&file);
		xmlWriter->setAutoFormatting(true);
		xmlWriter->setAutoFormattingIndent(2);

		xmlWriter->writeStartDocument();
		xmlWriter->writeStartElement("root");
		xmlWriter->writeStartElement("Info");
		xmlWriter->writeStartElement("PlaylistName");
		xmlWriter->writeCharacters(_name);
		xmlWriter->writeEndElement(); // PlaylistName
		xmlWriter->writeEndElement(); // Info
		xmlWriter->writeStartElement("Songs");

		uint num = 1;
		foreach(QUPlaylistEntry *entry, _playlist) {
			if(!entry->song())
				logSrv->add(QString(tr("Warning! The playlist entry \"%1 - %2\" will NOT be found by Vocaluxe/UltraStar!")).arg(entry->artistLink()).arg(entry->titleLink()), QU::Warning);
			else
				entry->setLinks(entry->song()->artist(), entry->song()->title());

			xmlWriter->writeStartElement("Song" + QString::number(num++));
			xmlWriter->writeStartElement("Artist");
			xmlWriter->writeCharacters(entry->artistLink());
			xmlWriter->writeEndElement(); // Artist
			xmlWriter->writeStartElement("Title");
			xmlWriter->writeCharacters(entry->titleLink());
			xmlWriter->writeEndElement(); // Title
			xmlWriter->writeStartElement("GameMode");
			xmlWriter->writeCharacters(entry->gameMode());
			xmlWriter->writeEndElement(); // GameMode
			xmlWriter->writeEndElement(); // Song
		}

		xmlWriter->writeEndElement(); // Songs
		xmlWriter->writeEndElement(); // root
		xmlWriter->writeEndDocument();
		delete xmlWriter;
	} else {
		/* write UltraStar UPL */
		QTextStream _out(&file);
		QTextCodec *codec = QTextCodec::codecForName("UTF-8");
		_out.setCodec(codec);

		// if it is a new playlist created within UltraStar Manager, we add a header to the comments
		if(_comments.isEmpty()) {
			_comments << "######################################";
			_comments << "#Ultrastar Deluxe Playlist Format v1.0";
			_comments << QString("Playlist %1 with %2 Songs.").arg(_name).arg(_playlist.length());
			_comments << "######################################";
		}

		// write comments
		foreach(QString comment, _comments) {
			_out << comment << "\n";
		}

		// write name of playlist
		_out << QString("#%1: %2\n").arg(NAME_TAG).arg(_name);
		_nameChanged = false;
		_playlistChanged = false;

		// write song links
		_out << QString("#%1:\n").arg(SONGS_TAG);
		foreach(QUPlaylistEntry *entry, _playlist) {
			if(!entry->song())
				logSrv->add(QString(tr("Warning! The playlist entry \"%1 - %2\" will NOT be found by UltraStar!")).arg(entry->artistLink()).arg(entry->titleLink()), QU::Warning);
			else
				entry->setLinks(entry->song()->artist(), entry->song()->title());

			_out << QString("%1 : %2\n").arg(entry->artistLink()).arg(entry->titleLink());
		}
	}

	file.close();
	logSrv->add(QString(tr("The playlist file \"%1\" was saved successfully.")).arg(_fi.filePath()), QU::Saving);

	return true;
}

QUPlaylistEntry* QUPlaylistFile::entry(int index) {
	if(index < 0 || index >= _playlist.size())
		return nullptr;

	return _playlist.at(index);
}

QUPlaylistEntry* QUPlaylistFile::last() {
	return entry(count() - 1);
}

bool QUPlaylistFile::hasUnsavedChanges() const {
	if(_nameChanged || _playlistChanged || !_fi.exists())
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

	foreach(QUPlaylistEntry *entry, _playlist) { // TODO: a progressbar helpful?! (for big playlists)
		if(song == entry->song()) {
			logSrv->add(QString(tr("The song \"%1 - %2\" is already in the current playlist. It was not added.")).arg(song->artist()).arg(song->title()), QU::Warning);
			return false;
		}
	}

	QUPlaylistEntry *newEntry = new QUPlaylistEntry(song->artist(), song->title(), GAMEMODE_NORMAL, this);
	newEntry->connectSong(song);

	_playlist.append(newEntry);
	_playlistChanged = true;

	emit dataChanged();

	return true;
}

bool QUPlaylistFile::removeEntry(QUPlaylistEntry *entry) {
	if(!entry)
		return false;

	_playlist.removeAll(entry); // return value should be 1 (or 0)
	_playlistChanged = true;

	emit dataChanged();

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
			logSrv->add(QString(tr("The order of the playlist \"%1\" could NOT be changed. The new order misses an entry.")).arg(name()), QU::Warning);
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

	if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		logSrv->add(QString(tr("Read error! The playlist file \"%1\" could NOT be opened.")).arg(QDir::toNativeSeparators(_fi.filePath())), QU::Warning);
		return;
	}

	if(QUSongSupport::allowedVocaluxePlaylistFiles().contains("*." + _fi.suffix())) {
		/* read Vocaluxe XML */
		QString artist, title, gameMode;
		QXmlStreamReader xml(&file);

		while(!xml.atEnd() && !xml.hasError()) {
			QXmlStreamReader::TokenType token = xml.readNext();

			if(token == QXmlStreamReader::StartDocument) {
				continue;
			}

			if(token == QXmlStreamReader::StartElement) {
				if(xml.name() == "Info") {
					continue;
				}
				else if(xml.name() == "PlaylistName") {
					_name = xml.readElementText();
				}
				else if(xml.name() == "Songs") {
					continue;
				}
				else if(xml.name().startsWith("Song")) {
					continue;
				}
				else if(xml.name() == "Artist") {
					artist = xml.readElementText();
				}
				else if(xml.name() == "Title") {
					title = xml.readElementText();
				}
				else if(xml.name() == "GameMode") {
					gameMode = xml.readElementText();
				}
				if(!artist.isEmpty() && !title.isEmpty() && !gameMode.isEmpty()) {
					_playlist.append(new QUPlaylistEntry(artist, title, gameMode, this));
					artist.clear();
					title.clear();
					gameMode.clear();
				}
			}
		}
		/* Error handling. */
		if(xml.hasError()) {
			logSrv->add(QString(tr("Read error! The playlist file \"%1\" contains XML syntax errors.")).arg(QDir::toNativeSeparators(_fi.filePath())), QU::Warning);
		}
		xml.clear();
	} else {
		/* read UltraStar UPL */
		QString line = QString::fromUtf8(file.readLine()).trimmed();

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
				_playlist.append(new QUPlaylistEntry(line.section(":", 0, 0).trimmed(), line.section(":", 1, 1).trimmed(), GAMEMODE_NORMAL, this));

			line = QString::fromLocal8Bit(file.readLine()).trimmed();
		}

		// use last line buffer
		// TODO: a little bit dirty here (duplicate code)
		if(!line.startsWith(QString("#%1:").arg(SONGS_TAG), Qt::CaseInsensitive))
			_playlist.append(new QUPlaylistEntry(line.section(":", 0, 0).trimmed(), line.section(":", 1, 1).trimmed(), GAMEMODE_NORMAL, this));
	}

	file.close();

	connectSongs();
}
