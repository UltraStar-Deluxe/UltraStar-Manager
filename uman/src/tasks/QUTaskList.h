#ifndef QUTASKLIST_H_
#define QUTASKLIST_H_

#include "QU.h"
#include "QUTaskItem.h"
#include "QUSongFile.h"

#include <QTreeWidget>
#include <QPoint>
#include <QList>
#include <QMenu>
#include <QKeyEvent>

class QUTaskList: public QTreeWidget {
	Q_OBJECT

public:
	QUTaskList(QWidget *parent = 0);

	void doTasksOn(QUSongFile *song);

public slots:
	void showContextMenu(const QPoint &point);
	void resetTaskList();

	void checkAllTasks();
	void uncheckAllTasks();
	void uncheckAllExclusiveTasks(QTreeWidgetItem *item);

	void editCurrentTask();

	void backupCurrentSelection();
	void restoreCurrentSelection();
	void updateToolTip();
	void setCurrentSlot(int i);

signals:
	void toolTipChanged(int, const QString&);

private:
	int _currentSlot; // for backup and restore item selection

	QTreeWidgetItem* appendSeparator(const QString &text);
	QList<QUTaskItem*> allTaskItems() const;
};

#endif /*QUTASKLIST_H_*/
