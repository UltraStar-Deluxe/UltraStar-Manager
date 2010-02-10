#include "QUPlaylistModel.h"

#include "QUPlaylistDatabase.h"
#include <QVariant>
#include <QDir>
#include <QString>

QUPlaylistModel::QUPlaylistModel(QObject *parent): QAbstractListModel(parent) {
//	connect(playlistDB, SIGNAL(playlistChanged(QUPlaylistFile*)), this, SLOT(
	_playlistIndex = 0;
}

int QUPlaylistModel::rowCount(const QModelIndex &parent) const {
	if(!playlist())
		return 0;

	return playlist()->count();
}

QVariant QUPlaylistModel::data(const QModelIndex &index, int role) const {
	if(!index.isValid() || !playlist())
		return QVariant();

	QUPlaylistEntry *entry = playlist()->entry(index.row());
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
		QFont f("Verdana", 10, QFont::Normal, false);
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

void QUPlaylistModel::setIndex(int playlistIndex) {
	if(playlistIndex < 0 or playlistIndex >= playlistDB->size())
		return;
	_playlistIndex = playlistIndex;
	reload();
}

QUPlaylistFile* QUPlaylistModel::playlist() const {
	if(_playlistIndex < 0 or _playlistIndex >= playlistDB->size())
		return 0;

	return playlistDB->at(_playlistIndex);
}

void QUPlaylistModel::reload() {
	reset();
}
