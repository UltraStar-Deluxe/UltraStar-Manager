#include "QUTaskItem.h"

QUTaskItem::QUTaskItem(QUTask *task):
	QListWidgetItem(),
	_task(task)
{
	this->setIcon(task->icon());
	this->setText(task->description());
	this->setToolTip(task->toolTip());

	this->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
	this->setCheckState(Qt::Unchecked);

	if(task->isConfigurable())
		this->setFlags(this->flags() | Qt::ItemIsSelectable);
}

QUTaskItem::~QUTaskItem() {
	delete _task;
}
