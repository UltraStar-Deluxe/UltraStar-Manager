#ifndef QUTASKTHREAD_H_
#define QUTASKTHREAD_H_

#include "QUAbstractThread.h"
#include "QUTaskList.h"
#include "QUSongItem.h"
#include "QUSongFile.h"

#include <QList>
#include <QTreeWidgetItem>

/*!
 * This thread is responsible for applying all checked tasks of the tasklist
 * to all selected songs.
 */
class QUTaskThread: public QUAbstractThread {
	Q_OBJECT
	
public:
	QUTaskThread(const QList<QTreeWidgetItem*> &items, QUTaskList *taskList, QObject *parent = 0);
	
	virtual void run();
	virtual int count() const { return _songItems.size(); }
	
private:
	QUTaskList *_taskList;
	QList<QUSongItem*> _songItems;
};

#endif /*QUTASKTHREAD_H_*/
