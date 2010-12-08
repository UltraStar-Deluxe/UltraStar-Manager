#include "QUPlaylistModel.h"

#include "QUPlaylistDatabase.h"
#include <QVariant>
#include <QDir>
#include <QString>

QUPlaylistModel::QUPlaylistModel(QObject *parent): QAbstractListModel(parent) {
	// some playlist changed ... maybe it was the 'current' playlist =D ... could be optimized
	connect(playlistDB, SIGNAL(playlistChanged(QUPlaylistFile*)), this, SLOT(reload()));

	// new playlist as 'current' selected -> show contents!
	connect(playlistDB, SIGNAL(currentPlaylistChanged(QUPlaylistFile*)), this, SLOT(reload()));
}

int QUPlaylistModel::rowCount(const QModelIndex &parent) const {
	if(!playlistDB->currentPlaylist())
		return 0;

	return playlistDB->currentPlaylist()->count();
}

QVariant QUPlaylistModel::data(const QModelIndex &index, int role) const {
	if(!index.isValid() || !playlistDB->currentPlaylist())
		return QVariant();

	QUPlaylistEntry *entry = playlistDB->currentPlaylist()->entry(index.row());

	if(!entry)
		return QVariant();

	QUSongFile *song = entry->song();

	if(role == Qt::DisplayRole) {
		if(song)
			return QString("%1. %2 - %3")
				.arg( index.row() + 1)
				.arg( song->artist() )
				.arg( song->title() );
		else
			return QString("%1. %2 - %3 (not found)")
				.arg( index.row() + 1)
				.arg( entry->artistLink() )
				.arg( entry->titleLink() );
	} else if(role == Qt::FontRole) {
		QFont f("MS Shell Dlg", 8, QFont::Normal, false);
		if(entry->hasUnsavedChanges())
			f.setBold(true);
		return f;
	} else if(role == Qt::ForegroundRole) {
		if(song)
			return Qt::black;
		else
			return Qt::gray;
	}

	return QVariant();
}

void QUPlaylistModel::reload() {
	reset();
}
