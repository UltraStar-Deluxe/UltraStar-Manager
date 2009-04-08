#ifndef QUTASKLIST_H_
#define QUTASKLIST_H_

#include "QU.h"
#include "QUTaskItem.h"
#include "QUSongFile.h"

#include <QListWidget>
#include <QPoint>
#include <QList>
#include <QMenu>
#include <QKeyEvent>
#include <QPluginLoader>

class QUTaskFactoryProxy;

class QUTaskList: public QListWidget {
	Q_OBJECT

public:
	QUTaskList(QWidget *parent = 0);

	void doTasksOn(QUSongFile *song);

	QList<QPluginLoader*> plugins() const { return _plugins; }
	QList<QUTaskFactoryProxy*> factoryProxies() const { return _factoryProxies; }

public slots:
	void showContextMenu(const QPoint &point);
	void reloadAllPlugins();
	void resetTaskList();

	void checkAllTasks();
	void uncheckAllTasks();
	void uncheckAllExclusiveTasks(QListWidgetItem *item);

	void editCurrentTask();

	void backupCurrentSelection();
	void restoreCurrentSelection();
	void setCurrentSlot(int i);

signals:
	void pluginsReloaded(const QList<QPluginLoader*>&);

private:
	QList<QPluginLoader*> _plugins;
	QList<QUTaskFactoryProxy*> _factoryProxies;
	int _currentSlot; // for backup and restore item selection

	void updateFactoryProxies();
	void appendSeparator(const QString &text);
};

#endif /*QUTASKLIST_H_*/
