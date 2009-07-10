#ifndef QUTASKITEM_H_
#define QUTASKITEM_H_

#include <QTreeWidgetItem>

#include "QUTaskPlugin.h"

class QUTaskItem: public QTreeWidgetItem {
public:
	QUTaskItem(QUTask *task);
	~QUTaskItem();

	QUTask* task() const { return _task; }
	void installSmartSettings();

private:
	QUTask *_task;
};

#endif /*QUTASKITEM_H_*/
