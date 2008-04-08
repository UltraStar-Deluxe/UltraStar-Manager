#ifndef QUTASKLIST_H_
#define QUTASKLIST_H_

#include <QListWidget>

#include "QU.h"
#include "QUTaskItem.h"
#include "QUSongFile.h"

class QUTaskList: public QListWidget {
	Q_OBJECT
	
public:
	QUTaskList(QWidget *parent = 0);
	
	void doTasksOn(QUSongFile *song);
	
public slots:
	void checkAllTasks();
	void uncheckAllTasks();
	void uncheckAllExclusiveTasks(QListWidgetItem *item);
	
private:
	void appendSeparator(const QString &text);
};

#endif /*QUTASKLIST_H_*/
