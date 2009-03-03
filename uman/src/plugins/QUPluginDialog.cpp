#include "QUPluginDialog.h"

#include <QPluginLoader>
#include <QFileInfo>

#include "QUTaskPlugin.h"

QUPluginDialog::QUPluginDialog(const QList<QPluginLoader*> &plugins, QWidget *parent): QDialog(parent) {
	setupUi(this);

	connect(okBtn, SIGNAL(clicked()), this, SLOT(accept()));

	foreach(QPluginLoader *plugin, plugins) {
		QFileInfo fi(plugin->fileName());
		QString entry = QString(tr("%1 (%2, %3 tasks)")).arg(fi.fileName());

		QUTaskFactory *factory = qobject_cast<QUTaskFactory*>(plugin->instance());

		if(factory) {
			entry = entry.arg("factory found");
			QList<QUTask*> tasks = factory->createTasks();
			entry = entry.arg(tasks.size());
		} else {
			entry = entry.arg("error").arg(0);
		}

		pluginList->addItem(entry);
	}
}
