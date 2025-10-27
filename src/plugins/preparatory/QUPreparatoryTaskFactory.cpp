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
	result << QUPreparatoryTask::CapitalizeTitle;
	result << QUPreparatoryTask::CapitalizeArtist;
	result << QUPreparatoryTask::SetEditionIfEmpty;
	result << QUPreparatoryTask::SetGenreIfEmpty;
	result << QUPreparatoryTask::FixLanguage;
	result << QUPreparatoryTask::FixApostrophes;
	result << QUPreparatoryTask::AddMissingDuetTags;
	result << QUPreparatoryTask::FixLineEndings;
	return result;
}

QMap<QString, QString> QUPreparatoryTaskFactory::translationLocations() const {
	QDir dir = QCoreApplication::applicationDirPath();
	QMap<QString, QString> locations;

	if(dir.cd("plugins") && dir.cd("languages")) {
		locations.insert(QLocale(QLocale::German, QLocale::Germany).name(), dir.filePath("preparatory.de.qm"));
		locations.insert(QLocale(QLocale::Polish, QLocale::Poland).name(), dir.filePath("preparatory.pl.qm"));
		locations.insert(QLocale(QLocale::French, QLocale::France).name(), dir.filePath("preparatory.fr.qm"));
		locations.insert(QLocale(QLocale::Spanish, QLocale::Spain).name(), dir.filePath("preparatory.es.qm"));
		locations.insert(QLocale(QLocale::Portuguese, QLocale::Portugal).name(), dir.filePath("preparatory.pt.qm"));
	}

	return locations;
}
