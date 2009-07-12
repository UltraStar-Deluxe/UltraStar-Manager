#include "QUScriptableTaskFactory.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>

QUScriptableTaskFactory::QUScriptableTaskFactory(QObject *parent): QUMultilingualTaskFactory(parent) {}

QList<QUTask*> QUScriptableTaskFactory::createTasks() {
    QList<QUTask*> tasks;
    QList<QDomDocument*> configurations = this->loadTaskConfigurations();

	foreach(QDomDocument *configuration, configurations) {
		QUTask *task = this->createTask(configuration);
		if(task)
			tasks << task;
	}

    return tasks;
}

/*!
 * Opens all XML-Files which could be task configurations.
 */
QList<QDomDocument*> QUScriptableTaskFactory::loadTaskConfigurations() {
	QList<QDomDocument*> configurations;

	QDir configDir = this->configurationDirectory();

	QFileInfoList configFiList = configDir.entryInfoList(QStringList("*.xml"), QDir::Files, QDir::Name);

	foreach(QFileInfo configFi, configFiList) {
		QFile file(configFi.filePath());
		if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QDomDocument *configuration = new QDomDocument();
			configuration->setContent(file.readAll());

			// save current fileName for later use
			configuration->firstChildElement("task").setAttribute("file", configFi.fileName());

			configurations.append(configuration);
		}
	}

	return configurations;
}
