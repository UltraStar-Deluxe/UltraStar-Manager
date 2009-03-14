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

QDir QURenameTaskFactory::configurationDirectory() {
	QDir dir = QCoreApplication::applicationDirPath();
	dir.cd("task-def");
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

Q_EXPORT_PLUGIN2(qurenametaskfactory, QURenameTaskFactory);
