#include "QUAbstractTask.h"

QUAbstractTask::QUAbstractTask(QObject *parent): QObject(parent) {
	this->setIcon(QIcon());
	this->setDescription("");
	this->setToolTip("");
}
