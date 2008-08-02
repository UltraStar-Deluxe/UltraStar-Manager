#ifndef QUTASKLIST_H_
#define QUTASKLIST_H_

#include <QListWidget>
#include <QDomDocument>
#include <QDomAttr>

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

	void editTask(QListWidgetItem *item);

signals:
	void finished(const QString &message, QU::EventMessageTypes type);

private:
	QList<QDomDocument*> loadTaskFiles();
	void appendSeparator(const QString &text);
};

#endif /*QUTASKLIST_H_*/
