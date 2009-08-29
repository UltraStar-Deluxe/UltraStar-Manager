#include "QUPluginDialog.h"

#include <QPluginLoader>
#include <QFileInfo>
#include <QHeaderView>
#include <QMap>

#include "QUTaskPlugin.h"
#include "QUSongInterface.h"

QUPluginDialog::QUPluginDialog(const QList<QPluginLoader*> &plugins, QWidget *parent): QDialog(parent) {
	setupUi(this);

	connect(okBtn, SIGNAL(clicked()), this, SLOT(accept()));
	connect(reloadBtn, SIGNAL(clicked()), this, SIGNAL(pluginReloadRequested()));

	apiLbl->setText(QString(tr("<b>API:</b> Song v%1, Plugin v%2")).arg(SONG_API_VERSION).arg(PLUGIN_API_VERSION));

	pluginTable->setColumnCount(8);

	pluginTable->setHorizontalHeaderLabels(QStringList() << tr("Factory") << tr("Product") << tr("Operations") << tr("Quantity") << tr("Languages") << tr("Version") << tr("API") << tr("Path"));
	pluginTable->horizontalHeader()->setResizeMode(PLUGIN_FACTORY_COL, QHeaderView::ResizeToContents);
	pluginTable->horizontalHeader()->setResizeMode(PLUGIN_PRODUCT_COL, QHeaderView::ResizeToContents);
	pluginTable->horizontalHeader()->setResizeMode(PLUGIN_OPERATIONS_COL, QHeaderView::ResizeToContents);
	pluginTable->horizontalHeader()->setResizeMode(PLUGIN_QUANTITY_COL, QHeaderView::ResizeToContents);
	pluginTable->horizontalHeader()->setResizeMode(PLUGIN_LANG_COL, QHeaderView::ResizeToContents);
	pluginTable->horizontalHeader()->setResizeMode(PLUGIN_VERSION_COL, QHeaderView::ResizeToContents);
	pluginTable->horizontalHeader()->setResizeMode(PLUGIN_PATH_COL, QHeaderView::Stretch);

	pluginTable->verticalHeader()->hide();

	this->updatePluginTable(plugins);
}

void QUPluginDialog::updatePluginTable(const QList<QPluginLoader*> &plugins) {
	pluginTable->setRowCount(plugins.size());
	int row = 0;

	foreach(QPluginLoader *plugin, plugins) {
		QFileInfo fi(plugin->fileName());
		QUTaskFactory *factory = qobject_cast<QUTaskFactory*>(plugin->instance());

		if(factory) {
			pluginTable->setItem(row, PLUGIN_FACTORY_COL, this->createItem(factory->name()));
			pluginTable->setItem(row, PLUGIN_PRODUCT_COL, this->createItem(factory->productName()));
			pluginTable->setItem(row, PLUGIN_VERSION_COL, this->createItem(factory->versionString()));

			QList<QUTask*> tasks = factory->createTasks();
			QStringList operations;
			if (factory->canAddConfigurations())
				operations << "add";
			int configurableCount = 0;
			foreach(QUTask *task, tasks) {
				if(task->isConfigurable())
					configurableCount++;
			}
			if (configurableCount > 0) {
				if (configurableCount < tasks.size())
					operations << QString("edit (%1/%2)").arg(configurableCount).arg(tasks.size());
				else
					operations << "edit";
			}

			pluginTable->setItem(row, PLUGIN_OPERATIONS_COL, this->createItem(operations.join(", ")));
			pluginTable->setItem(row, PLUGIN_QUANTITY_COL, this->createItem(QString(tr("%1 tasks")).arg(tasks.size())));

			QMapIterator<QString, QTranslator*> i(factory->translations());
			QStringList translations;
			while(i.hasNext()) {
				i.next();
				translations << i.key();
			}
			pluginTable->setItem(row, PLUGIN_LANG_COL, this->createItem(translations.join(", ")));

			pluginTable->setItem(row, PLUGIN_API_COL, this->createItem(QString(tr("Song v%1, Plugin v%2")).arg(factory->songApiVersion()).arg(factory->pluginApiVersion())));
		} else {
			pluginTable->setItem(row, PLUGIN_FACTORY_COL, this->createItem(tr("No factory found.")));
		}
		pluginTable->setItem(row, PLUGIN_PATH_COL, this->createItem(fi.filePath()));
		row++;
	}
}

QTableWidgetItem* QUPluginDialog::createItem(const QString &text) {
	QTableWidgetItem* result = new QTableWidgetItem(text);
	result->setFlags(Qt::ItemIsEnabled);
	result->setToolTip(text);
	return result;
}
