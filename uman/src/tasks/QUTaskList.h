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

class QUTaskList: public QListWidget {
	Q_OBJECT

public:
	QUTaskList(QWidget *parent = 0);

	void doTasksOn(QUSongFile *song);

	QList<QPluginLoader*> plugins() const { return _plugins; };

public slots:
	void showContextMenu(const QPoint &point);

	void resetTaskList();
	void reloadAllPlugins();

	void checkAllTasks();
	void uncheckAllTasks();
	void uncheckAllExclusiveTasks(QListWidgetItem *item);

	void addAudioTagTask();
	void addRenameTask();

	void editCurrentTask();

private:
	QList<QPluginLoader*> _plugins;

	void appendSeparator(const QString &text);
};

#endif /*QUTASKLIST_H_*/
