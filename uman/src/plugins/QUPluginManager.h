#ifndef QUPLUGINMANAGER_H
#define QUPLUGINMANAGER_H

#define pluginMGR (QUPluginManager::instance())

#include <QObject>
#include <QList>
#include <QDir>

#include "QUTaskPlugin.h"
#include "QURemoteImageSourcePlugin.h"

class QUTaskFactoryProxy;
class QPluginLoader;
class QWidget;

class QUPluginManager: public QObject {
	Q_OBJECT

public:
	static QDir pluginFolder();
	void setUiParent(QWidget *w);
	QList<QUTaskFactoryProxy*> taskFactoryProxies() const { return _taskFactoryProxies; }

	QList<QPluginLoader*> plugins() const { return _plugins; }

public slots:
	void reload();

signals:
	void reloaded();
	void configurationAdded();

private:
	QWidget *_uiParent; // just ensure proper ui rendering for plugin stuff
	QList<QPluginLoader*> _plugins;
	QList<QUTaskFactoryProxy*> _taskFactoryProxies;

	void loadAllPlugins();
	void unloadAllPlugins();

	void loadTranslationsForTasks();
	void unloadTranslationsForTasks();

	void createTaskFactoryProxies();
	void removeTaskFactoryProxies();
	void updateTaskFactoryProxies();

/*!
 * Use the singleton pattern because we need only one
 * plugin manager at a time.
 */
public:
	static QUPluginManager* instance();
protected:
	QUPluginManager(QObject *parent = 0);
private:
	static QUPluginManager *_instance;
};

#endif // QUPLUGINMANAGER_H
