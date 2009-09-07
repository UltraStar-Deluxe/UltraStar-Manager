#include "QURenameTaskFactory.h"
#include "QURenameTask.h"
#include "QURenameTaskDialog.h"

#include <QCoreApplication>

QURenameTaskFactory::QURenameTaskFactory(QObject *parent): QUScriptableTaskFactory(parent) {}

QString QURenameTaskFactory::name() const {
	return tr("Rename Tasks");
}

QString QURenameTaskFactory::productName() const {
	return tr("Rename Task");
}

QString QURenameTaskFactory::description() const {
	return tr("Move or rename files and folders based on song data.");
}

QDir QURenameTaskFactory::configurationDirectory() {
	QDir dir = QCoreApplication::applicationDirPath();
	dir.cd("plugins");
	dir.cd("config");
	dir.cd("rename");
	return dir;
}

QUTask* QURenameTaskFactory::createTask(QDomDocument *configuration) {
	if( QString::compare("rename", configuration->firstChild().firstChildElement("general").attribute("type"), Qt::CaseInsensitive) == 0 ) {
		return new QURenameTask(configuration);
	} else {
		return 0;
	}
}

int QURenameTaskFactory::addConfiguration(QWidget *parent) {
	return QURenameTaskDialog(0, parent).exec();
}

QMap<QString, QString> QURenameTaskFactory::translationLocations() const {
	QDir dir = QCoreApplication::applicationDirPath();
	QMap<QString, QString> locations;

	if(dir.cd("plugins") && dir.cd("languages")) {
		locations.insert(QLocale(QLocale::German, QLocale::Germany).name(), dir.filePath("rename.de.qm"));
		locations.insert(QLocale(QLocale::Polish, QLocale::Poland).name(), dir.filePath("rename.pl.qm"));
	}

	return locations;
}

Q_EXPORT_PLUGIN2(qurenametaskfactory, QURenameTaskFactory);
