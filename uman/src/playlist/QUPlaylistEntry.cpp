#include "QUPlaylistEntry.h"

QUPlaylistEntry::QUPlaylistEntry(const QString &artist, const QString & title, QObject *parent):
	QObject(parent), _song(0), _artistLink(artist), _titleLink(title)
{

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
 * Looks for a proper song and connects it with this playlist entry. An old song
 * connection may be overwritten.
 */
void QUPlaylistEntry::connectSong(const QList<QUSongFile*> &songs) {
	if(_song)
		return; // disconnect a song first!

	foreach(QUSongFile *song, songs) {
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
