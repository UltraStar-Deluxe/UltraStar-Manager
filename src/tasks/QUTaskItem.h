#ifndef QUTASKITEM_H_
#define QUTASKITEM_H_

#include <QObject>
#include <QTreeWidgetItem>

#include "QUTaskPlugin.h"

class QUTaskItem: public QObject, public QTreeWidgetItem {
	Q_OBJECT

public:
	QUTaskItem(QUTask *task);
	~QUTaskItem();

	QUTask* task() const { return _task; }
	void installSmartSettings();

public slots:
	void highlightChanges();

private:
	QUTask *_task;
};

#endif /*QUTASKITEM_H_*/
