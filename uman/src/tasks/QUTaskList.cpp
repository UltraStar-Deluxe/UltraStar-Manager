#include "QUTaskList.h"

#include "QUPreparatoryTask.h"
#include "QUAudioTagTask.h"
#include "QURenameTask.h"

#include <QFont>

QUTaskList::QUTaskList(QWidget *parent): QListWidget(parent) {
	this->appendSeparator("Preparatory Tasks");
	this->addItem(new QUTaskItem(new QUPreparatoryTask(QU::autoAssignFiles)));
	this->addItem(new QUTaskItem(new QUPreparatoryTask(QU::removeUnsupportedTags)));
	
	this->appendSeparator("ID3 Tag Tasks");
	this->addItem(new QUTaskItem(new QUAudioTagTask(QU::useArtist)));
	this->addItem(new QUTaskItem(new QUAudioTagTask(QU::useTitle)));
	this->addItem(new QUTaskItem(new QUAudioTagTask(QU::useGenre)));
	this->addItem(new QUTaskItem(new QUAudioTagTask(QU::useYear)));
	
	this->appendSeparator("Renaming Tasks");
	this->addItem(new QUTaskItem(new QURenameTask(QU::renameDirectory)));
	this->addItem(new QUTaskItem(new QURenameTask(QU::renameDirectorySpecial)));
	this->addItem(new QUTaskItem(new QURenameTask(QU::renameSongFile)));
	this->addItem(new QUTaskItem(new QURenameTask(QU::renameAudioFile)));
	this->addItem(new QUTaskItem(new QURenameTask(QU::renameCoverFile)));
	this->addItem(new QUTaskItem(new QURenameTask(QU::renameBackgroundFile)));
	this->addItem(new QUTaskItem(new QURenameTask(QU::renameVideoFile)));
	this->addItem(new QUTaskItem(new QURenameTask(QU::renameVideoFileSpecial)));
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
	
	if(!task)
		return;
	
	for(int i = 0; i < this->count(); i++) {
		if(this->item(i)->checkState() == Qt::Unchecked)
			continue;
		
		QUTaskItem *exclusiveTaskItem = dynamic_cast<QUTaskItem*>(this->item(i));
		
		if(!exclusiveTaskItem)
			continue;
		
		QURenameTask *exclusiveTask = dynamic_cast<QURenameTask*>(exclusiveTaskItem->task());
		
		if(!exclusiveTask)
			continue;
		
		if( ((task->mode() == QU::renameDirectory) && (exclusiveTask->mode() == QU::renameDirectorySpecial))
			||
			((task->mode() == QU::renameDirectorySpecial) && (exclusiveTask->mode() == QU::renameDirectory))
			||
			((task->mode() == QU::renameVideoFile) && (exclusiveTask->mode() == QU::renameVideoFileSpecial))
			||
			((task->mode() == QU::renameVideoFileSpecial) && (exclusiveTask->mode() == QU::renameVideoFile))
		)
			exclusiveTaskItem->setCheckState(Qt::Unchecked);
	}
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
