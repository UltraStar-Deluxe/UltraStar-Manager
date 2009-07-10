#include "QUTaskItem.h"
#include "QUSmartSetting.h"
#include "QU.h"
#include "QUMonty.h"

#include <QColor>
#include <QFont>

QUTaskItem::QUTaskItem(QUTask *task):
	QObject(),
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
	task()->provideData(monty->unsupportedTags(), QU::unsupportedTags);
	foreach(QUSmartSetting *smartSetting, task()->smartSettings()) {
		QTreeWidgetItem *sItem = new QTreeWidgetItem();
		sItem->setBackgroundColor(0, QColor(239, 239, 239));
		addChild(sItem);
		treeWidget()->setItemWidget(sItem, 0, smartSetting->editor());

		connect(smartSetting, SIGNAL(changed()), this, SLOT(highlightChanges()));
	}
	highlightChanges();
}

void QUTaskItem::highlightChanges() {
	QFont f(this->font(0));
	f.setItalic(false);

	foreach(QUSmartSetting *smartSetting, task()->smartSettings())
		if(smartSetting->hasChanges()) {
			f.setItalic(true);
			break;
		}

	this->setFont(0, f);
}
