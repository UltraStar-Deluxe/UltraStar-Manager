#ifndef QUTASKLIST_H_
#define QUTASKLIST_H_

#include "QU.h"
#include "QUTaskItem.h"
#include "QUSongFile.h"

#include <QListWidget>
#include <QDomDocument>
#include <QDomAttr>
#include <QPoint>
#include <QList>
#include <QMenu>
#include <QKeyEvent>

class QUTaskList: public QListWidget {
	Q_OBJECT

public:
	QUTaskList(QWidget *parent = 0);

	void doTasksOn(QUSongFile *song);

public slots:
	void showContextMenu(const QPoint &point);
	void resetTaskList();

	void checkAllTasks();
	void uncheckAllTasks();
	void uncheckAllExclusiveTasks(QListWidgetItem *item);

	void addTask();
	void editTask();

signals:
	void finished(const QString &message, QU::EventMessageTypes type);

private:
	QList<QDomDocument*> loadTaskFiles();
	void appendSeparator(const QString &text);
};

#endif /*QUTASKLIST_H_*/
