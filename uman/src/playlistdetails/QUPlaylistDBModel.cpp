#include "QUPlaylistDBModel.h"

#include "QUPlaylistDatabase.h"
#include "QULogService.h"
#include <QVariant>
#include <QDir>
#include <QString>

QUPlaylistDBModel::QUPlaylistDBModel(QObject *parent): QAbstractListModel(parent) {
	connect(playlistDB, SIGNAL(playlistAdded(QUPlaylistFile*)), this, SLOT(reload()));
	connect(playlistDB, SIGNAL(playlistChanged(QUPlaylistFile*)), this, SLOT(reload()));
	connect(playlistDB, SIGNAL(playlistDeleted(QUPlaylistFile*)), this, SLOT(reload()));
	connect(playlistDB, SIGNAL(databaseCleared()), this, SLOT(reload()));
	connect(playlistDB, SIGNAL(databaseReloaded()), this, SLOT(reload()));
}

int QUPlaylistDBModel::rowCount(const QModelIndex &parent) const {
	return playlistDB->size();
}

QVariant QUPlaylistDBModel::data(const QModelIndex &index, int role) const {
	if(!index.isValid())
		return QVariant();

	if(role == Qt::DisplayRole) {
		return QString("%1%2 (%3)")
			.arg( playlistDB->at(index.row())->name() )
			.arg( playlistDB->at(index.row())->hasUnsavedChanges() ? "*" : "" )
			.arg( playlistDB->dir().relativeFilePath( playlistDB->at(index.row())->fileInfo().filePath()) );
	}

	return QVariant();
}

void QUPlaylistDBModel::reload() {
	reset();
}
