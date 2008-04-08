#ifndef QUTASKITEM_H_
#define QUTASKITEM_H_

#include <QListWidgetItem>

#include "QUAbstractTask.h"

class QUTaskItem: public QListWidgetItem {
public:
	QUTaskItem(QUAbstractTask *task);
	~QUTaskItem();
	
	QUAbstractTask* task() const { return _task; }
	
private:
	QUAbstractTask *_task;
};

#endif /*QUTASKITEM_H_*/
