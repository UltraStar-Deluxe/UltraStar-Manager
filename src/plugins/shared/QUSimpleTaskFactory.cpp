#include "QUSimpleTaskFactory.h"

QUSimpleTaskFactory::QUSimpleTaskFactory(QObject *parent): QUMultilingualTaskFactory(parent) {}

QList<QUTask*> QUSimpleTaskFactory::createTasks() {
	QList<QUTask*> tasks;

	foreach(int type, this->types()) {
		QUTask *task = this->createTask(type);
		if(task)
			tasks << task;
	}

	return tasks;
}
