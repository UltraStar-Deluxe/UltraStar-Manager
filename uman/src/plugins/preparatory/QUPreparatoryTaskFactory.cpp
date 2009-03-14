#include "QUPreparatoryTaskFactory.h"
#include "QUPreparatoryTask.h"

QUPreparatoryTaskFactory::QUPreparatoryTaskFactory(QObject *parent): QUSimpleTaskFactory(parent) {}


QString QUPreparatoryTaskFactory::name() const {
	return tr("Preparatory Tasks");
}

QUTask* QUPreparatoryTaskFactory::createTask(int type) {
	return new QUPreparatoryTask((QU::PreparatoryTaskModes)type);
}

QList<int> QUPreparatoryTaskFactory::types() const {
	QList<int> result;
	result << QU::autoAssignFiles;
	result << QU::removeUnsupportedTags;
	result << QU::fixAudioLength;
	result << QU::roundGap;
	return result;
}

Q_EXPORT_PLUGIN2(qupreparatorytaskfactory, QUPreparatoryTaskFactory);
