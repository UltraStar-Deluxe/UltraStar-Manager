#include "QUCleanUpTaskFactory.h"
#include "QUCleanUpTask.h"

QUCleanUpTaskFactory::QUCleanUpTaskFactory(QObject *parent): QUSimpleTaskFactory(parent) {}


QString QUCleanUpTaskFactory::name() const {
	return tr("Clean-Up Tasks");
}

QUTask* QUCleanUpTaskFactory::createTask(int type) {
	return new QUCleanUpTask((QU::CleanTaskModes)type);
}

QList<int> QUCleanUpTaskFactory::types() const {
	QList<int> result;
	result << QU::unusedFiles;
	result << QU::invalidFileTags;
	result << QU::removeEndTag;
	return result;
}

Q_EXPORT_PLUGIN2(qucleanuptaskfactory, QUCleanUpTaskFactory);
