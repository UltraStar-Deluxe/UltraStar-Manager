#include "QUAudioTagTaskFactory.h"
#include "QUAudioTagTask.h"
#include "QUAudioTagTaskDialog.h"

#include <QCoreApplication>

QUAudioTagTaskFactory::QUAudioTagTaskFactory(QObject *parent): QUScriptableTaskFactory(parent) {}

QString QUAudioTagTaskFactory::name() const {
	return tr("Song/ID3 Tag Tasks");
}

QString QUAudioTagTaskFactory::productName() const {
	return tr("Song/ID3 Tag Task");
}

QString QUAudioTagTaskFactory::description() const {
	return tr("Use meta-data from audio files to edit a song.");
}

QDir QUAudioTagTaskFactory::configurationDirectory() {
	QDir dir = QCoreApplication::applicationDirPath();
	dir.cd("plugins");
	dir.cd("config");
	dir.cd("audiotag");
	return dir;
}

QUTask* QUAudioTagTaskFactory::createTask(QDomDocument *configuration) {
	if( QString::compare("id3", configuration->firstChild().firstChildElement("general").attribute("type"), Qt::CaseInsensitive) == 0 ) {
		return new QUAudioTagTask(configuration);
	} else {
		return 0;
	}
}

int QUAudioTagTaskFactory::addConfiguration(QWidget *parent) {
	return QUAudioTagTaskDialog(0, parent).exec();
}

QMap<QString, QString> QUAudioTagTaskFactory::translationLocations() const {
	QDir dir = QCoreApplication::applicationDirPath();
	QMap<QString, QString> locations;

	if(dir.cd("plugins") && dir.cd("languages")) {
		locations.insert(QLocale(QLocale::German, QLocale::Germany).name(), dir.filePath("audiotag.de.qm"));
		locations.insert(QLocale(QLocale::Polish, QLocale::Poland).name(), dir.filePath("audiotag.pl.qm"));
	}

	return locations;
}

Q_EXPORT_PLUGIN2(quaudiotagtaskfactory, QUAudioTagTaskFactory);
