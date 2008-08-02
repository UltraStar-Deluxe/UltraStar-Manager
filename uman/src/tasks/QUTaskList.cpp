#include "QUTaskList.h"

#include "QUPreparatoryTask.h"
#include "QUAudioTagTask.h"
#include "QURenameTask.h"
#include "QUCleanTask.h"

#include <QCoreApplication>
#include <QFont>
#include <QDir>
#include <QFile>
#include <QFileInfoList>
#include <QMessageBox>

QUTaskList::QUTaskList(QWidget *parent): QListWidget(parent) {
	QList<QDomDocument*> tasks = this->loadTaskFiles(); // pre-configured tasks

	this->appendSeparator(tr("Preparatory Tasks"));
	this->addItem(new QUTaskItem(new QUPreparatoryTask(QU::autoAssignFiles)));
	this->addItem(new QUTaskItem(new QUPreparatoryTask(QU::removeUnsupportedTags)));

	this->appendSeparator(tr("ID3 Tag Tasks"));
	this->addItem(new QUTaskItem(new QUAudioTagTask(QU::useArtist)));
	this->addItem(new QUTaskItem(new QUAudioTagTask(QU::useTitle)));
	this->addItem(new QUTaskItem(new QUAudioTagTask(QU::useGenre)));
	this->addItem(new QUTaskItem(new QUAudioTagTask(QU::useYear)));

	this->appendSeparator(tr("Renaming Tasks"));
	foreach(QDomDocument* task, tasks) {
		if( QString::compare("rename", task->firstChild().firstChildElement("general").attribute("type"), Qt::CaseInsensitive) == 0 )
			this->addItem(new QUTaskItem(new QURenameTask(task)));
	}

	this->appendSeparator(tr("Clean-Up Tasks"));
	this->addItem(new QUTaskItem(new QUCleanTask(QU::unusedFiles)));
	this->addItem(new QUTaskItem(new QUCleanTask(QU::invalidFileTags)));

	qDeleteAll(tasks);
	tasks.clear();

	// do not allow to check two exclusive tasks
	connect(this, SIGNAL(itemChanged(QListWidgetItem*)), SLOT(uncheckAllExclusiveTasks(QListWidgetItem*)));
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

	QURenameTask *task = dynamic_cast<QURenameTask*>(taskItem->task());

	if(!task or task->group() == -1)
		return;

	for(int i = 0; i < this->count(); i++) {
		if(this->item(i) == item)
			continue; // do not compare with current item

		if(this->item(i)->checkState() == Qt::Unchecked)
			continue;

		QUTaskItem *exclusiveTaskItem = dynamic_cast<QUTaskItem*>(this->item(i));

		if(!exclusiveTaskItem)
			continue;

		QURenameTask *exclusiveTask = dynamic_cast<QURenameTask*>(exclusiveTaskItem->task());

		if(!exclusiveTask)
			continue;

		if(task->group() == exclusiveTask->group())
			exclusiveTaskItem->setCheckState(Qt::Unchecked);
	}
}

/*!
 * Opens all XML-Files which could be task configurations.
 */
QList<QDomDocument*> QUTaskList::loadTaskFiles() {
	QList<QDomDocument*> tasks;

	QDir taskDir = QCoreApplication::applicationDirPath();
	// TODO: Make task config path available through registry - not hard-coded.
	taskDir.cd("task-def");

	QFileInfoList taskFiList = taskDir.entryInfoList(QStringList("*.xml"), QDir::Files);

	foreach(QFileInfo taskFi, taskFiList) {
		QFile file(taskFi.filePath());
		if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QDomDocument *newTask = new QDomDocument(taskFi.fileName());
			newTask->setContent(file.readAll());
			tasks.append(newTask);
		}
	}

	return tasks;
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
