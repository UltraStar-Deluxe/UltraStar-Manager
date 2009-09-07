#include "QUPluginModel.h"
#include "QUPluginManager.h"

#include "QUPlugin.h"

#include <QVariant>

QUPluginModel::QUPluginModel(QObject *parent): QAbstractListModel(parent) {
	connect(pluginMGR, SIGNAL(reloaded()), this, SLOT(invalidateData()));
}

int QUPluginModel::rowCount(const QModelIndex &parent) const {
	return pluginMGR->plugins().size();
}

QVariant QUPluginModel::data(const QModelIndex &index, int role) const {
	if(!index.isValid())
		return QVariant();

	if(role == Qt::DisplayRole) {
		return pluginMGR->plugins().at(index.row())->name();
	}

	return QVariant();
}

void QUPluginModel::invalidateData() {
	reset(); // ... because this is no slot
}
