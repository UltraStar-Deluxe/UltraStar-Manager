#include "QUPluginManager.h"
#include "QULogService.h"
#include "QUTaskFactoryProxy.h"

#include "QUTaskPlugin.h"
#include "QURemoteImageSourcePlugin.h"

#include <QCoreApplication>
#include <QPluginLoader>
#include <QSettings>
#include <QLocale>

QUPluginManager::QUPluginManager(QObject *parent): QObject(parent) {}
QUPluginManager* QUPluginManager::_instance = 0;
QUPluginManager* QUPluginManager::instance() {
	if(_instance == 0) _instance = new QUPluginManager(0);
	return _instance;
}

/* ----------------------------------------------------------------- */

QDir QUPluginManager::pluginFolder() {
	QDir result = QCoreApplication::applicationDirPath();
	if(!result.cd("plugins"))
		logSrv->add(tr("Could NOT open default plugin folder."), QU::Warning);
	return result;
}

void QUPluginManager::reload() {
	unloadAllPlugins();
	loadAllPlugins();

	logSrv->add(tr("%1 plugins loaded.").arg(_plugins.size()), QU::Information);
	emit reloaded();
}

void QUPluginManager::setUiParent(QWidget *w) {
	if(w != _uiParent) {
		removeTaskFactoryProxies();
		_uiParent = w;
		createTaskFactoryProxies();
	}
}

QList<QUPlugin*> QUPluginManager::plugins() const {
	QList<QUPlugin*> result;
	foreach(QPluginLoader *plugin, _plugins) {
		QUPlugin *p = dynamic_cast<QUPlugin*>(plugin->instance());
		if(p) result << p;
	}
	return result;
}

QList<QUTaskFactory*> QUPluginManager::taskPlugins() const {
	QList<QUTaskFactory*> result;
	foreach(QPluginLoader *plugin, _plugins) {
		QUTaskFactory *factory = qobject_cast<QUTaskFactory*>(plugin->instance());
		if(factory) result << factory;
	}
	return result;
}

QList<QURemoteImageSource*> QUPluginManager::imageSourcePlugins() const {
	QList<QURemoteImageSource*> result;
	foreach(QPluginLoader *plugin, _plugins) {
		QURemoteImageSource *imgSrc = qobject_cast<QURemoteImageSource*>(plugin->instance());
		if(imgSrc) result << imgSrc;
	}
	return result;
}


void QUPluginManager::loadAllPlugins() {
	if(!_plugins.isEmpty()) {
		logSrv->add(tr("Plugin loading failed. Unload loaded plugins first."), QU::Error);
		return;
	}

	QFileInfoList pluginFiList = pluginFolder().entryInfoList(QStringList("*.*"), QDir::Files, QDir::Name);

	foreach(QFileInfo pluginFi, pluginFiList) {
		if(!QLibrary::isLibrary(pluginFi.filePath()))
			continue;

		QPluginLoader *ldr = new QPluginLoader(pluginFi.filePath(), this);
		if(!ldr->load()) {
			logSrv->add(ldr->errorString(), QU::Warning);
		} else {
			_plugins << ldr;
		}
	}

	createTaskFactoryProxies();
	loadTranslations();
}

void QUPluginManager::unloadAllPlugins() {
	unloadTranslations();
	removeTaskFactoryProxies();

	foreach(QPluginLoader *plugin, _plugins)
		plugin->unload();

	qDeleteAll(_plugins);
	_plugins.clear();
}

void QUPluginManager::loadTranslations() {
	QString language = QSettings().value("language", QLocale::system().name()).toString();

	foreach(QUPlugin *plugin, plugins()) {
		// only translator for current language should be installed
		QMap<QString, QTranslator*> translations(plugin->translations());
		if(translations.contains(language))
			qApp->installTranslator(translations.value(language));
	}
}

void QUPluginManager::unloadTranslations() {
	QString language = QSettings().value("language", QLocale::system().name()).toString();

	foreach(QUPlugin *plugin, plugins()) {
		// only translator for current language should be installed
		QMap<QString, QTranslator*> translations(plugin->translations());
		if(translations.contains(language))
			qApp->removeTranslator(translations.value(language));
	}
}

void QUPluginManager::createTaskFactoryProxies() {
	if(!_taskFactoryProxies.isEmpty()) {
		logSrv->add(tr("Could not create plugin proxies. Unload them first."), QU::Error);
		return;
	}

	foreach(QPluginLoader *ldr, _plugins) {
		QUTaskFactory *factory = qobject_cast<QUTaskFactory*>(ldr->instance());
		if(factory) {
			_taskFactoryProxies << new QUTaskFactoryProxy(factory, _uiParent, this);
			connect(_taskFactoryProxies.last(), SIGNAL(configurationAdded()), this, SIGNAL(configurationAdded()));
		}
	}
}

void QUPluginManager::removeTaskFactoryProxies() {
	qDeleteAll(_taskFactoryProxies);
	_taskFactoryProxies.clear();
}

void QUPluginManager::updateTaskFactoryProxies() {
	removeTaskFactoryProxies();
	createTaskFactoryProxies();
}
