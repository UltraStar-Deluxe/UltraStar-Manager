#include "QUTaskItem.h"

QUTaskItem::QUTaskItem(QUAbstractTask *task): 
	QListWidgetItem(), 
	_task(task) 
{
	this->setIcon(task->icon());
	this->setText(task->description());
	this->setToolTip(task->toolTip());
	
	this->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	this->setCheckState(Qt::Unchecked);
}

QUTaskItem::~QUTaskItem() {
	delete _task;
}
