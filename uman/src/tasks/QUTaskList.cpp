#include "QUTaskList.h"
#include "QULogService.h"
#include "QUTaskPlugin.h"
#include "QUPluginManager.h"
#include "QUTaskFactoryProxy.h"

#include <QCoreApplication>
#include <QFont>
#include <QDir>
#include <QFile>
#include <QFileInfoList>
#include <QMessageBox>
#include <QLibrary>
#include <QSettings>
#include <QMap>

QUTaskList::QUTaskList(QWidget *parent): QTreeWidget(parent) {
	// do not allow to check two exclusive tasks
	connect(this, SIGNAL(itemChanged(QTreeWidgetItem*, int)), SLOT(uncheckAllExclusiveTasks(QTreeWidgetItem*)));
	// TODO: Enable task editing not through double-click?
	connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), SLOT(editCurrentTask()));

	// context menu
	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));

	// backup and restore of selection
	connect(this, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(backupCurrentSelection()));

	setHeaderHidden(true);
	setRootIsDecorated(false);
	setIndentation(10);
}

void QUTaskList::setCurrentSlot(int i) {
	if(i < 0)
		return;

	_currentSlot = i;
	this->restoreCurrentSelection();
}

/*!
 * Shows a context menu with actions for custom rename tasks.
 */
void QUTaskList::showContextMenu(const QPoint &point) {
	QMenu menu(this);
	QMenu *addMenu = menu.addMenu(QIcon(":/marks/add.png"), tr("Add"));

	foreach(QUTaskFactoryProxy *fp, pluginMGR->taskFactoryProxies()) {
		if(fp->factory()->canAddConfigurations())
			addMenu->addAction(QString("%1...").arg(fp->factory()->productName()), fp, SLOT(addConfiguration()));
	}

	QUTaskItem *taskItem = dynamic_cast<QUTaskItem*>(this->itemAt(point));
	if(taskItem && taskItem->task()->isConfigurable())
		menu.addAction(QIcon(":/control/pencil.png"), tr("Edit..."), this, SLOT(editCurrentTask()));

	menu.addSeparator();
	menu.addAction(QIcon(":/control/refresh.png"), tr("Refresh All"), this, SLOT(resetTaskList()));

	menu.exec(this->mapToGlobal(point));
}

/*!
 * Clear the list and refill it. Should be used to reload custom (rename) tasks.
 */
void QUTaskList::resetTaskList() {
	disconnect(this, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(backupCurrentSelection()));

	this->clear();

	foreach(QUTaskFactoryProxy *fp, pluginMGR->taskFactoryProxies()) {
		QTreeWidgetItem *sep = this->appendSeparator(fp->factory()->name());
		foreach(QUTask *task, fp->factory()->createTasks()) {
			QUTaskItem *taskItem = new QUTaskItem(task);
			sep->addChild(taskItem);
			taskItem->installSmartSettings();
		}
		sep->setExpanded(true);
	}

	restoreCurrentSelection();
	connect(this, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(backupCurrentSelection()));
}

void QUTaskList::doTasksOn(QUSongFile *song) {
	foreach(QUTaskItem *taskItem, allTaskItems())
		if(taskItem->checkState(0) == Qt::Checked)
			taskItem->task()->startOn(song);
}

void QUTaskList::checkAllTasks() {
	foreach(QUTaskItem *taskItem, allTaskItems())
		taskItem->setCheckState(0, Qt::Checked);
}

void QUTaskList::uncheckAllTasks() {
	foreach(QUTaskItem *taskItem, allTaskItems())
		taskItem->setCheckState(0, Qt::Unchecked);
}

/*!
 * Unchecks all tasks that cannot be used with several other
 * tasks together. These are some renaming tasks up to now.
 */
void QUTaskList::uncheckAllExclusiveTasks(QTreeWidgetItem *item) {
	if(item->checkState(0) == Qt::Unchecked)
		return;

	QUTaskItem *taskItem = dynamic_cast<QUTaskItem*>(item);

	if(!taskItem)
		return;

	QUTask *task = taskItem->task();

	if(task->group() == -1)
		return;

	foreach(QUTaskItem *otherTaskItem, allTaskItems()) {
		if(otherTaskItem == taskItem)
			continue; // do not compare with current item

		if(otherTaskItem->checkState(0) == Qt::Unchecked)
			continue;

		QUTask *exclusiveTask = otherTaskItem->task();

		if(task->group() == exclusiveTask->group())
			otherTaskItem->setCheckState(0, Qt::Unchecked);
	}
}

/*!
 * Shows a dialog to edit the selected "rename" task if such a task is
 * selected.
 */
void QUTaskList::editCurrentTask() {
	QUTaskItem *taskItem = dynamic_cast<QUTaskItem*>(this->currentItem());

	if(!taskItem or !taskItem->task()->isConfigurable())
		return;

	if(taskItem->task()->configure(this)) {
		this->resetTaskList();		
		logSrv->add(tr("Task list was refreshed successfully."), QU::Information);
	}
}

QTreeWidgetItem* QUTaskList::appendSeparator(const QString &text) {
	QTreeWidgetItem *separator = new QTreeWidgetItem(QStringList(text));

	separator->setTextAlignment(0, Qt::AlignLeft);
	separator->setFlags(Qt::ItemIsEnabled);
	separator->setBackgroundColor(0, QColor(239, 239, 239));
	separator->setTextColor(0, QColor(134, 134, 134));

	QFont f(separator->font(0));
	f.setBold(true);
	f.setPointSize(8);
	separator->setFont(0, f);

	this->addTopLevelItem(separator);
	return separator;
}

/*!
 * Save the checkstate of each task item into the registry.
 */
void QUTaskList::backupCurrentSelection() {
	updateToolTip();

	QString state;
	for(int i = 0; i < topLevelItemCount(); i++) {
		for(int j = 0; j < topLevelItem(i)->childCount(); j++)
			state.append(topLevelItem(i)->child(j)->checkState(0) == Qt::Checked ? "1" : "0");
		state.append("-");
	}

	QSettings s;
	s.setValue(QString("taskListState%1").arg(_currentSlot), state);
}

void QUTaskList::restoreCurrentSelection() {
	QSettings s;
	QStringList states = s.value(QString("taskListState%1").arg(_currentSlot), "").toString().split("-", QString::SkipEmptyParts);

	this->uncheckAllTasks();

	for(int i = 0; i < states.size() && i < topLevelItemCount(); i++)
		for(int j = 0; j < states.at(i).length() && j < topLevelItem(i)->childCount(); j++)
			topLevelItem(i)->child(j)->setCheckState(0, states.at(i).at(j) == QChar('1') ? Qt::Checked : Qt::Unchecked);

	updateToolTip();
}

/*!
 * Update the tooltip of the current slot.
 */
void QUTaskList::updateToolTip() {
	QString tooltip;
	for(int i = 0; i < topLevelItemCount(); i++) {
		bool isCheckedFirst = true;

		for(int j = 0; j < topLevelItem(i)->childCount(); j++) {
			if(topLevelItem(i)->child(j)->checkState(0) == Qt::Unchecked)
				continue;

			if(isCheckedFirst) {
				tooltip.append(QString("<b>%1</b><br>").arg(topLevelItem(i)->text(0)));
				isCheckedFirst = false;
			}

			QString text = topLevelItem(i)->child(j)->text(0);
			if(text.length() > 30)
				tooltip.append(QString("- %1...<br>").arg(text.left(30)));
			else
				tooltip.append(QString("- %1<br>").arg(text));
		}
	}
	if(tooltip.endsWith("<br>"))
		tooltip.truncate(tooltip.size() - 4);
	emit toolTipChanged(_currentSlot, tooltip);
}

QList<QUTaskItem*> QUTaskList::allTaskItems() const {
	QList<QUTaskItem*> result;
	for(int i = 0; i < topLevelItemCount(); i++)
		for(int j = 0; j < topLevelItem(i)->childCount(); j++) {
			QUTaskItem *taskItem = dynamic_cast<QUTaskItem*>(topLevelItem(i)->child(j));
			if(taskItem)
				result.append(taskItem);
		}
	return result;
}
