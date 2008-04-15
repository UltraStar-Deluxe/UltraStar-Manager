#include "QUTaskThread.h"

QUTaskThread::QUTaskThread(const QList<QTreeWidgetItem*> &items, QUTaskList *taskList, QObject *parent): QUAbstractThread(parent) {
	foreach(QTreeWidgetItem *item, items) {
		QUSongItem *songItem = dynamic_cast<QUSongItem*>(item);
		
		if(songItem)
			_songItems.append(songItem);
	}
	
	_taskList = taskList;
}

void QUTaskThread::run() {
	qRegisterMetaType<QU::EventMessageTypes>("QU::EventMessageTypes");
	
	foreach(QUSongItem *item, _songItems) {
		QUSongFile *song = item->song();
		
		emit continued(QString("%1 - %2").arg(song->artist()).arg(song->title()));
		
		_taskList->doTasksOn(song);
		song->save();
	}
}
