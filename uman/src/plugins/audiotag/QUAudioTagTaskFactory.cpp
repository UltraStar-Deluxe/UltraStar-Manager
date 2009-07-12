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

Q_EXPORT_PLUGIN2(quaudiotagtaskfactory, QUAudioTagTaskFactory);
