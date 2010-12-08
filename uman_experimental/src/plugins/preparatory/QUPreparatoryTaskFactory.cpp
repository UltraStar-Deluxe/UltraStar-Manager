#include "QUPreparatoryTaskFactory.h"
#include "QUPreparatoryTask.h"

#include <QDir>
#include <QLocale>
#include <QCoreApplication>

QUPreparatoryTaskFactory::QUPreparatoryTaskFactory(QObject *parent): QUSimpleTaskFactory(parent) {}

QString QUPreparatoryTaskFactory::name() const {
	return tr("Preparatory Tasks");
}

QString QUPreparatoryTaskFactory::description() const {
	return tr("Fix general issues and prepare song for other tasks.");
}

QUTask* QUPreparatoryTaskFactory::createTask(int type) {
	return new QUPreparatoryTask((QUPreparatoryTask::TaskModes)type);
}

QList<int> QUPreparatoryTaskFactory::types() const {
	QList<int> result;
	result << QUPreparatoryTask::AutoAssignFiles;
	result << QUPreparatoryTask::RemoveUnsupportedTags;
	result << QUPreparatoryTask::FixAudioLength;
	result << QUPreparatoryTask::RoundGap;
	result << QUPreparatoryTask::FixCapitalization;
	result << QUPreparatoryTask::CapitalizeArtist;
	return result;
}

QMap<QString, QString> QUPreparatoryTaskFactory::translationLocations() const {
	QDir dir = QCoreApplication::applicationDirPath();
	QMap<QString, QString> locations;

	if(dir.cd("plugins") && dir.cd("languages")) {
		locations.insert(QLocale(QLocale::German, QLocale::Germany).name(), dir.filePath("preparatory.de.qm"));
		locations.insert(QLocale(QLocale::Polish, QLocale::Poland).name(), dir.filePath("preparatory.pl.qm"));
	}

	return locations;
}

Q_EXPORT_PLUGIN2(qupreparatorytaskfactory, QUPreparatoryTaskFactory);
