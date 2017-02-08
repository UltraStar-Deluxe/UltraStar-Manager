#include "QUSimpleTask.h"

QUSimpleTask::QUSimpleTask(QObject *parent): QObject(parent) {
	this->setIcon(QIcon());
	this->setDescription("");
	this->setToolTip("");
	this->setGroup(-1);
}
