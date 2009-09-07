#include "QUCleanUpTaskFactory.h"
#include "QUCleanUpTask.h"

#include <QDir>
#include <QLocale>
#include <QCoreApplication>

QUCleanUpTaskFactory::QUCleanUpTaskFactory(QObject *parent): QUSimpleTaskFactory(parent) {}

QString QUCleanUpTaskFactory::name() const {
	return tr("Clean-Up Tasks");
}

QString QUCleanUpTaskFactory::description() const {
	return tr("Remove obsolete song data or delete unused files.");
}

QUTask* QUCleanUpTaskFactory::createTask(int type) {
	return new QUCleanUpTask((QUCleanUpTask::TaskModes)type);
}

QList<int> QUCleanUpTaskFactory::types() const {
	QList<int> result;
	result << QUCleanUpTask::UnusedFiles;
	result << QUCleanUpTask::InvalidFileTags;
	result << QUCleanUpTask::RemoveEndTag;
	return result;
}

QMap<QString, QString> QUCleanUpTaskFactory::translationLocations() const {
	QDir dir = QCoreApplication::applicationDirPath();
	QMap<QString, QString> locations;

	if(dir.cd("plugins") && dir.cd("languages")) {
		locations.insert(QLocale(QLocale::German, QLocale::Germany).name(), dir.filePath("cleanup.de.qm"));
		locations.insert(QLocale(QLocale::Polish, QLocale::Poland).name(), dir.filePath("cleanup.pl.qm"));
	}

	return locations;
}

Q_EXPORT_PLUGIN2(qucleanuptaskfactory, QUCleanUpTaskFactory);
