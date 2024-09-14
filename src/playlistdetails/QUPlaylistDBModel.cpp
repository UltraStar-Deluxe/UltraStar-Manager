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
			.arg(playlistDB->at(index.row())->name(), playlistDB->at(index.row())->hasUnsavedChanges() ? "*" : "", playlistDB->dir().relativeFilePath( playlistDB->at(index.row())->fileInfo().filePath()));
	} else  if(role == Qt::FontRole) {
		QFont f(
			"MS Shell Dlg", 8,
			playlistDB->at(index.row())->hasUnsavedChanges() ? QFont::Bold : QFont::Normal,
			false);
		return f;
	}

	return QVariant();
}

void QUPlaylistDBModel::reload() {
	// MB TODO: added first and third line to quick-and-dirty-fix playlist bugs, needs more work
	int currentIndex = playlistDB->currentIndex();
	beginResetModel();
	//reset();
	endResetModel();
	playlistDB->setCurrentIndex(currentIndex);
}
