#include "QUTaskList.h"
#include "QULogService.h"

//#include "QUPreparatoryTask.h"
//#include "QULyricTask.h"
//#include "QUAudioTagTask.h"
//#include "QURenameTask.h"
//#include "QUCleanTask.h"
//#include "QUAudioTagTaskDialog.h"
//#include "QURenameTaskDialog.h"

#include <QCoreApplication>
#include <QFont>
#include <QDir>
#include <QFile>
#include <QFileInfoList>
#include <QMessageBox>
#include <QLibrary>

#include "QUTaskPlugin.h"

QUTaskList::QUTaskList(QWidget *parent): QListWidget(parent) {
	// do not allow to check two exclusive tasks
	connect(this, SIGNAL(itemChanged(QListWidgetItem*)), SLOT(uncheckAllExclusiveTasks(QListWidgetItem*)));
	// TODO: Enable task editing not through double-click?
	connect(this, SIGNAL(itemDoubleClicked(QListWidgetItem*)), SLOT(editCurrentTask()));

	// context menu
	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));
}

/*!
 * Shows a context menu with actions for custom rename tasks.
 */
void QUTaskList::showContextMenu(const QPoint &point) {
//	QMenu menu(this);
//	QMenu *addMenu = menu.addMenu(QIcon(":/marks/add.png"), tr("Add"));
//
//	addMenu->addAction(tr("Song/ID3 Tag Task..."), this, SLOT(addAudioTagTask()));
//	addMenu->addAction(tr("Rename Task..."), this, SLOT(addRenameTask()));
//
//	QUTaskItem *taskItem = dynamic_cast<QUTaskItem*>(this->itemAt(point));
//	if(taskItem)
//		if(dynamic_cast<QUScriptableTask*>(taskItem->task()))
//			menu.addAction(QIcon(":/control/pencil.png"), tr("Edit..."), this, SLOT(editCurrentTask()));
//
//	menu.addSeparator();
//	menu.addAction(QIcon(":/control/refresh.png"), tr("Refresh All"), this, SLOT(resetTaskList()));
//
//	menu.exec(this->mapToGlobal(point));
}

/*!
 * Clear the list and refill it. Should be used to reload custom (rename) tasks.
 */
void QUTaskList::resetTaskList() {
	this->clear();
//
//	QList<QDomDocument*> tasks = this->loadTaskFiles(); // pre-configured tasks
//
//	this->appendSeparator(tr("Preparatory Tasks"));
//	this->addItem(new QUTaskItem(new QUPreparatoryTask(QU::autoAssignFiles)));
//	this->addItem(new QUTaskItem(new QUPreparatoryTask(QU::removeUnsupportedTags)));
//	this->addItem(new QUTaskItem(new QUPreparatoryTask(QU::fixAudioLength)));
//	this->addItem(new QUTaskItem(new QUPreparatoryTask(QU::roundGap)));
//
//	this->appendSeparator(tr("Lyric Tasks"));
//	this->addItem(new QUTaskItem(new QULyricTask(QU::fixTimeStamps)));
//	this->addItem(new QUTaskItem(new QULyricTask(QU::fixSpaces)));
//	this->addItem(new QUTaskItem(new QULyricTask(QU::removeEmptySyllables)));
//
//	this->appendSeparator(tr("Song/ID3 Tag Tasks"));
//	foreach(QDomDocument* task, tasks) {
//		if( QString::compare("id3", task->firstChild().firstChildElement("general").attribute("type"), Qt::CaseInsensitive) == 0 ) {
//			QUAudioTagTask *newTask = new QUAudioTagTask(task);
//			this->addItem(new QUTaskItem(newTask));
//		}
//	}
//
	foreach(QPluginLoader *ldr, _plugins) {
		QUTaskFactory *factory = qobject_cast<QUTaskFactory*>(ldr->instance());

		if(!factory)
			continue;

		this->appendSeparator(factory->name());
		foreach(QUTask *task, factory->createTasks()) {
			this->addItem(new QUTaskItem(task));
		}
	}

	//QList<QUTask*> renameTasks = _plugins.first()->instance()
//	foreach(QDomDocument* task, tasks) {
//		if( QString::compare("rename", task->firstChild().firstChildElement("general").attribute("type"), Qt::CaseInsensitive) == 0 ) {
//			QURenameTask *newTask = new QURenameTask(task);
//			this->addItem(new QUTaskItem(newTask));
//		}
//	}
//
//	this->appendSeparator(tr("Clean-Up Tasks"));
//	this->addItem(new QUTaskItem(new QUCleanTask(QU::unusedFiles)));
//	this->addItem(new QUTaskItem(new QUCleanTask(QU::invalidFileTags)));
//	this->addItem(new QUTaskItem(new QUCleanTask(QU::removeEndTag)));
//
//	qDeleteAll(tasks);
//	tasks.clear();
}

void QUTaskList::doTasksOn(QUSongFile *song) {
	for(int i = 0; i < this->count(); i++) {
		QUTaskItem *item = dynamic_cast<QUTaskItem*>(this->item(i));

		if(item && item->checkState() == Qt::Checked)
			item->task()->startOn(song);
	}
}

void QUTaskList::checkAllTasks() {
	for(int i = 0; i < this->count(); i++) {
		if(this->item(i)->flags() & Qt::ItemIsUserCheckable)
			this->item(i)->setCheckState(Qt::Checked);
	}
}

void QUTaskList::uncheckAllTasks() {
	for(int i = 0; i < this->count(); i++) {
		if(this->item(i)->flags() & Qt::ItemIsUserCheckable)
			this->item(i)->setCheckState(Qt::Unchecked);
	}
}

/*!
 * Unchecks all tasks that cannot be used with several other
 * tasks together. These are some renaming tasks up to now.
 */
void QUTaskList::uncheckAllExclusiveTasks(QListWidgetItem *item) {
	if(item->checkState() == Qt::Unchecked)
		return;

	QUTaskItem *taskItem = dynamic_cast<QUTaskItem*>(item);

	if(!taskItem)
		return;

	QUTask *task = taskItem->task();

	if(task->group() == -1)
		return;

	for(int i = 0; i < this->count(); i++) {
		if(this->item(i) == item)
			continue; // do not compare with current item

		if(this->item(i)->checkState() == Qt::Unchecked)
			continue;

		QUTaskItem *exclusiveTaskItem = dynamic_cast<QUTaskItem*>(this->item(i));

		if(!exclusiveTaskItem)
			continue;

		QUTask *exclusiveTask = exclusiveTaskItem->task();

		if(task->group() == exclusiveTask->group())
			exclusiveTaskItem->setCheckState(Qt::Unchecked);
	}
}

void QUTaskList::addAudioTagTask() {
//	QUAudioTagTaskDialog *dlg = new QUAudioTagTaskDialog(this);
//
//	if(dlg->exec()) {
//		this->resetTaskList();
//		logSrv->add(tr("Task list was refreshed successfully."), QU::information);
//	}
//
//	delete dlg;
}

void QUTaskList::addRenameTask() {
//	QURenameTaskDialog *dlg = new QURenameTaskDialog(this);
//
//	if(dlg->exec()) {
//		this->resetTaskList();
//		logSrv->add(tr("Task list was refreshed successfully."), QU::information);
//	}
//
//	delete dlg;
}

/*!
 * Shows a dialog to edit the selected "rename" task if such a task is
 * selected.
 */
void QUTaskList::editCurrentTask() {
//	QUTaskItem *taskItem = dynamic_cast<QUTaskItem*>(this->currentItem());
//
//	if(!taskItem)
//		return;
//
//	QUTaskDialog *dlg;
//
//	if( dynamic_cast<QURenameTask*>(taskItem->task()) )
//		dlg = new QURenameTaskDialog(dynamic_cast<QURenameTask*>(taskItem->task()), this);
//	else if( dynamic_cast<QUAudioTagTask*>(taskItem->task()) )
//		dlg = new QUAudioTagTaskDialog(dynamic_cast<QUAudioTagTask*>(taskItem->task()), this);
//	else
//		return;
//
//	if(dlg->exec()) {
//		this->resetTaskList();
//		logSrv->add(tr("Task list was refreshed successfully."), QU::information);
//	}
//
//	delete dlg;
}

void QUTaskList::reloadAllPlugins() {
	logSrv->add(tr("Looking for task plugins..."), QU::information);

	qDeleteAll(_plugins);
	_plugins.clear();

	QDir pluginDir = QCoreApplication::applicationDirPath();
	pluginDir.cd("plugins");

	QFileInfoList pluginFiList = pluginDir.entryInfoList(QStringList("*.*"), QDir::Files, QDir::Name);

	foreach(QFileInfo pluginFi, pluginFiList) {
		if(!QLibrary::isLibrary(pluginFi.filePath()))
			continue;

		QPluginLoader *ldr = new QPluginLoader(pluginFi.filePath(), this);
		if(!ldr->load()) {
			logSrv->add(ldr->errorString(), QU::warning);
		} else {
			_plugins << ldr;
		}
	}

	logSrv->add(tr("Plugin loading finished."), QU::information);
	this->resetTaskList();
}

void QUTaskList::appendSeparator(const QString &text) {
	QListWidgetItem *separator = new QListWidgetItem(text);

	separator->setTextAlignment(Qt::AlignLeft);
	separator->setFlags(Qt::ItemIsEnabled);
	separator->setBackgroundColor(Qt::darkGray);
	separator->setTextColor(Qt::white);

	QFont f(separator->font());
	f.setBold(true);
	separator->setFont(f);

	this->addItem(separator);
}
