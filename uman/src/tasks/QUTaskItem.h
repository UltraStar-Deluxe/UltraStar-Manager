#ifndef QUTASKITEM_H_
#define QUTASKITEM_H_

#include <QListWidgetItem>

#include "QUTaskPlugin.h"

class QUTaskItem: public QListWidgetItem {
public:
	QUTaskItem(QUTask *task);
	~QUTaskItem();

	QUTask* task() const { return _task; }

private:
	QUTask *_task;
};

#endif /*QUTASKITEM_H_*/
