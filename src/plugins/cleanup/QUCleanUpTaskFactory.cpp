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
	result << QUCleanUpTask::SetID3Tags;
	return result;
}

QMap<QString, QString> QUCleanUpTaskFactory::translationLocations() const {
	QDir dir = QCoreApplication::applicationDirPath();
	QMap<QString, QString> locations;

	if(dir.cd("plugins") && dir.cd("languages")) {
		locations.insert(QLocale(QLocale::German, QLocale::Germany).name(), dir.filePath("cleanup.de.qm"));
		locations.insert(QLocale(QLocale::Polish, QLocale::Poland).name(), dir.filePath("cleanup.pl.qm"));
		locations.insert(QLocale(QLocale::French, QLocale::France).name(), dir.filePath("cleanup.fr.qm"));
		locations.insert(QLocale(QLocale::Spanish, QLocale::Spain).name(), dir.filePath("cleanup.es.qm"));
		locations.insert(QLocale(QLocale::Portuguese, QLocale::Portugal).name(), dir.filePath("cleanup.pt.qm"));
	}

	return locations;
}
