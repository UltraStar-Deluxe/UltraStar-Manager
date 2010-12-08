#ifndef QUPLUGINMANAGER_H
#define QUPLUGINMANAGER_H

#define pluginMGR (QUPluginManager::instance())

#include <QObject>
#include <QList>
#include <QDir>

class QUPlugin;
class QUTaskFactory;
class QURemoteImageSource;
class QUTaskFactoryProxy;
class QWidget;
class QPluginLoader;

class QUPluginManager: public QObject {
	Q_OBJECT

public:
	static QDir pluginFolder();
	void setUiParent(QWidget *w);
	QList<QUTaskFactoryProxy*> taskFactoryProxies() const { return _taskFactoryProxies; }

	QList<QUPlugin*> plugins() const;
	QList<QUTaskFactory*> taskPlugins() const;
	QList<QURemoteImageSource*> imageSourcePlugins() const;

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

	void loadTranslations();
	void unloadTranslations();

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
