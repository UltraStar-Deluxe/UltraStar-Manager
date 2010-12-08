#include "QUPluginModel.h"
#include "QUPluginManager.h"

#include "QUPlugin.h"
#include "QUTaskPlugin.h"
#include "QURemoteImageSourcePlugin.h"

#include <QVariant>
#include <QString>

QUPluginModel::QUPluginModel(QObject *parent): QAbstractListModel(parent) {
	connect(pluginMGR, SIGNAL(reloaded()), this, SLOT(invalidateData()));
}

int QUPluginModel::rowCount(const QModelIndex &parent) const {
	return pluginMGR->plugins().size();
}

QVariant QUPluginModel::data(const QModelIndex &index, int role) const {
	if(!index.isValid())
		return QVariant();

	QUTaskFactory *factory = dynamic_cast<QUTaskFactory*>(pluginMGR->plugins().at(index.row()));
	QURemoteImageSource *imgSrc = dynamic_cast<QURemoteImageSource*>(pluginMGR->plugins().at(index.row()));

	switch(role) {
	case NameRole:
		return pluginMGR->plugins().at(index.row())->name();
	case VersionRole:
		return pluginMGR->plugins().at(index.row())->version();
	case AuthorRole:
		return pluginMGR->plugins().at(index.row())->author();
	case DescriptionRole:
		return pluginMGR->plugins().at(index.row())->description();
	case IconRole:
		return pluginMGR->plugins().at(index.row())->icon();
	case Qt::ToolTipRole:
		QString stuff;

		if(factory) {
			stuff = tr("Type: Task Factory Plugin");
		} else if(imgSrc) {
			stuff = tr("Type: Remote Image Source Plugin");
			if(imgSrc->type() == QURemoteImageSource::CoverImage)
				stuff += tr("<br>Subtype: Cover Images");
			else if(imgSrc->type() == QURemoteImageSource::BackgroundImage)
				stuff += tr("<br>Subtype: Background Images");
		}

		return tr("<b>%1</b><br>%2<br><br>Author: %3<br>Version: %4<br>%5")
				.arg(pluginMGR->plugins().at(index.row())->name())
				.arg(pluginMGR->plugins().at(index.row())->description())
				.arg(pluginMGR->plugins().at(index.row())->author())
				.arg(pluginMGR->plugins().at(index.row())->version())
				.arg(stuff);
	}

	return QVariant();
}

void QUPluginModel::invalidateData() {
	reset(); // ... because this is no slot
}
