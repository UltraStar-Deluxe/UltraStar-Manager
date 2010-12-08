#include "QULyricTaskFactory.h"
#include "QULyricTask.h"

#include <QDir>
#include <QLocale>
#include <QCoreApplication>

QULyricTaskFactory::QULyricTaskFactory(QObject *parent): QUSimpleTaskFactory(parent) {}

QString QULyricTaskFactory::name() const {
	return tr("Lyric Tasks");
}

QString QULyricTaskFactory::description() const {
	return tr("Modify a song's lyrics.");
}

QUTask* QULyricTaskFactory::createTask(int type) {
	return new QULyricTask((QULyricTask::TaskModes)type);
}

QList<int> QULyricTaskFactory::types() const {
	QList<int> result;
	result << QULyricTask::ConvertRelativeToAbsolute;
	result << QULyricTask::ConvertAbsoluteToRelative;
	result << QULyricTask::FixTimeStamps;
	result << QULyricTask::FixSpaces;
	result << QULyricTask::RemoveEmptySyllables;
	result << QULyricTask::ConvertSyllablePlaceholder1;
	result << QULyricTask::ConvertSyllablePlaceholder2;
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
