#include "QUPlaylistEntry.h"

QUPlaylistEntry::QUPlaylistEntry(const QString &artist, const QString & title, QObject *parent):
	QObject(parent), _song(0), _artistLink(artist), _titleLink(title)
{

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
 * connection will be overwritten.
 */
void QUPlaylistEntry::connectSong(const QList<QUSongFile*> &songs) {
	foreach(QUSongFile *song, songs) {
		this->connectSong(song);
	}
}
