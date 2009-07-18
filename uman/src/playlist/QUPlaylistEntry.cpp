#include "QUPlaylistEntry.h"
#include "QUSongDatabase.h"

QUPlaylistEntry::QUPlaylistEntry(const QString &artist, const QString & title, QObject *parent):
	QObject(parent), _song(0), _artistLink(artist), _titleLink(title)
{
	connect(songDB, SIGNAL(songDeleted(QUSongFile*)), this, SLOT(disconnectSong(QUSongFile*)));
}

void QUPlaylistEntry::setLinks(const QString &artistLink, const QString &titleLink) {
	_artistLink = artistLink;
	_titleLink = titleLink;
}

/*!
 * Connects the given song if artist and title match.
 */
void QUPlaylistEntry::connectSong(QUSongFile* song) {
	if( QString::compare(artistLink(), song->artist(), Qt::CaseInsensitive) == 0 and
		QString::compare(titleLink(),  song->title(),  Qt::CaseInsensitive) == 0 ) {
		this->_song = song;
	}
}

/*!
 * Looks for a proper song and connects it with this playlist entry.
 */
void QUPlaylistEntry::connectSong() {
	if(_song)
		return; // disconnect a song first!, for performance issues (consider a big song DB...)

	foreach(QUSongFile *song, songDB->songs()) {
		this->connectSong(song);
	}
}

/*!
 * \returns Whether this playlist entry has to be saved.
 */
bool QUPlaylistEntry::hasUnsavedChanges() const {
	return( song() and (
			QString::compare(artistLink(), song()->artist(), Qt::CaseInsensitive) != 0 or
			QString::compare(titleLink(),  song()->title(),  Qt::CaseInsensitive) != 0 ));
}

void QUPlaylistEntry::disconnectSong(QUSongFile *song) {
	if(this->song() == song)
		disconnectSong();

	emit disconnected();
}
