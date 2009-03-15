#include "QULyricTaskFactory.h"
#include "QULyricTask.h"

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
	return result;
}

Q_EXPORT_PLUGIN2(qulyrictaskfactory, QULyricTaskFactory);
