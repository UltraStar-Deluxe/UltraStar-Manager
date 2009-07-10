#include "QUTaskItem.h"
#include "QUSmartSetting.h"

#include <QColor>

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

void QUTaskItem::installSmartSettings() {
	foreach(QUSmartSetting *smartSetting, task()->smartSettings()) {
		QTreeWidgetItem *sItem = new QTreeWidgetItem();
		sItem->setBackgroundColor(0, QColor(239, 239, 239));
		addChild(sItem);
		treeWidget()->setItemWidget(sItem, 0, smartSetting->editor());
	}
}
