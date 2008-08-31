#include "QUCoverModel.h"

QUCoverModel::QUCoverModel(QObject *parent): QAbstractListModel(parent) {
}

QVariant QUCoverModel::data (const QModelIndex &index, int role) const {
	return QVariant();
}

int QUCoverModel::rowCount (const QModelIndex &parent) const {
	return 0;
}
