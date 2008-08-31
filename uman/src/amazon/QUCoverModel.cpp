#include "QUCoverModel.h"

#include <QPixmap>

QUCoverModel::QUCoverModel(QObject *parent): QAbstractListModel(parent) {
}

QVariant QUCoverModel::data (const QModelIndex &index, int role) const {
	if(!index.isValid())
		return QVariant();

	if(role == Qt::DecorationRole)
		return _iconList.at(index.row());
	else if(role == Qt::DisplayRole)
		return QString("%1 x %2").arg(_dimensionsList.at(index.row()).width()).arg(_dimensionsList.at(index.row()).height());
	else if(role == Qt::UserRole)
		return _filePathList.at(index.row());

	return QVariant();
}

int QUCoverModel::rowCount (const QModelIndex &parent) const {
	return _iconList.size();
}

/*!
 * Append a new thumbnail of the cover (filePath) to the end of the list.
 */
void QUCoverModel::addCover(const QString &filePath) {
	QPixmap pixmap(filePath);

	beginInsertRows(QModelIndex(), rowCount(), rowCount());

	_iconList.append(QIcon(pixmap.scaledToWidth(COVER_ICON_WIDTH, Qt::SmoothTransformation)));
	_dimensionsList.append(pixmap.size());
	_filePathList.append(filePath);

     endInsertRows();
}

/*!
 * Remove all thumbnails/items from the list.
 */
void QUCoverModel::clear() {
	beginRemoveRows(QModelIndex(), 0, rowCount() - 1);

	_iconList.clear();
	_dimensionsList.clear();
	_filePathList.clear();

	endRemoveRows();
}
