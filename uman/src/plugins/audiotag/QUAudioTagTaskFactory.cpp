#include "QUAudioTagTaskFactory.h"
#include "QUAudioTagTask.h"

#include <QCoreApplication>

QUAudioTagTaskFactory::QUAudioTagTaskFactory(QObject *parent): QUScriptableTaskFactory(parent) {}

QString QUAudioTagTaskFactory::name() const {
	return tr("Song/ID3 Tag Tasks");
}

QDir QUAudioTagTaskFactory::configurationDirectory() {
	QDir dir = QCoreApplication::applicationDirPath();
	dir.cd("task-def");
	return dir;
}

QUTask* QUAudioTagTaskFactory::createTask(QDomDocument *configuration) {
	if( QString::compare("id3", configuration->firstChild().firstChildElement("general").attribute("type"), Qt::CaseInsensitive) == 0 ) {
		return new QUAudioTagTask(configuration);
	} else {
		return 0;
	}
}

Q_EXPORT_PLUGIN2(quaudiotagtaskfactory, QUAudioTagTaskFactory);
