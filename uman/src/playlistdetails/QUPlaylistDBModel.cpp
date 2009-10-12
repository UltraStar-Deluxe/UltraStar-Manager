#include "QUPlaylistDBModel.h"

#include "QUPlaylistDatabase.h"
#include <QVariant>
#include <QDir>
#include <QString>

QUPlaylistDBModel::QUPlaylistDBModel(QObject *parent): QAbstractListModel(parent) {
//	connect(playlistDB, SIGNAL(playlistChanged(QUPlaylistFile*)), this, SLOT(
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
