#include "QUTaskItem.h"

QUTaskItem::QUTaskItem(QUTask *task):
	QTreeWidgetItem(),
	_task(task)
{
	this->setIcon(0, task->icon());
	this->setText(0, task->description());
	this->setToolTip(0, task->toolTip());

	this->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
	this->setCheckState(0, Qt::Unchecked);

	if(task->isConfigurable())
		this->setFlags(this->flags() | Qt::ItemIsSelectable);
}

QUTaskItem::~QUTaskItem() {
	delete _task;
}
