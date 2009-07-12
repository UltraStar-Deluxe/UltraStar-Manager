#include "QULyricTaskFactory.h"
#include "QULyricTask.h"

#include <QDir>
#include <QLocale>
#include <QCoreApplication>

QULyricTaskFactory::QULyricTaskFactory(QObject *parent): QUSimpleTaskFactory(parent) {}

QString QULyricTaskFactory::name() const {
	return tr("Lyric Tasks");
}

QUTask* QULyricTaskFactory::createTask(int type) {
	return new QULyricTask((QU::LyricTaskModes)type);
}

QList<int> QULyricTaskFactory::types() const {
	QList<int> result;
	result << QU::fixTimeStamps;
	result << QU::fixSpaces;
	result << QU::removeEmptySyllables;
	result << QU::convertSyllablePlaceholder1;
	result << QU::convertSyllablePlaceholder2;
	return result;
}

QMap<QString, QString> QULyricTaskFactory::translationLocations() const {
	QDir dir = QCoreApplication::applicationDirPath();
	QMap<QString, QString> locations;

	if(dir.cd("plugins") && dir.cd("languages")) {
		locations.insert(QLocale(QLocale::German, QLocale::Germany).name(), dir.filePath("lyric.de.qm"));
		locations.insert(QLocale(QLocale::Polish, QLocale::Poland).name(), dir.filePath("lyric.pl.qm"));
	}

	return locations;
}

Q_EXPORT_PLUGIN2(qulyrictaskfactory, QULyricTaskFactory);
