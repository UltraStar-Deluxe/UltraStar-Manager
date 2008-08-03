#include "QUTaskItem.h"
#include "QURenameTask.h"

QUTaskItem::QUTaskItem(QUAbstractTask *task):
	QListWidgetItem(),
	_task(task)
{
	this->setIcon(task->icon());
	this->setText(task->description());
	this->setToolTip(task->toolTip());

	this->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
	this->setCheckState(Qt::Unchecked);

	if(dynamic_cast<QURenameTask*>(task))
		this->setFlags(this->flags() | Qt::ItemIsSelectable);
}

QUTaskItem::~QUTaskItem() {
	delete _task;
}
