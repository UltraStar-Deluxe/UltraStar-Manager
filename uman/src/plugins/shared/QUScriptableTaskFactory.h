#ifndef QUSCRIPTABLETASKFACTORY_H_H
#define QUSCRIPTABLETASKFACTORY_H_H

#include <QObject>
#include <QDir>
#include <QDomDocument>

#include "QUTaskPlugin.h"
#include "QUSongInterface.h"

class QUScriptableTaskFactory: public QObject, public QUTaskFactory {
	Q_OBJECT
    Q_INTERFACES(QUTaskFactory)

public:
	QUScriptableTaskFactory(QObject *parent = 0);

	virtual int pluginApiVersion() const { return PLUGIN_API_VERSION; }
	virtual int songApiVersion() const { return SONG_API_VERSION; }

    virtual QList<QUTask*> createTasks();

	virtual bool canAddConfigurations() const { return true; }

protected:
	virtual QDir configurationDirectory() = 0;
	virtual QUTask* createTask(QDomDocument *configuration) = 0;

private:
    QList<QDomDocument*> loadTaskConfigurations();
};

#endif // QUSCRIPTABLETASKFACTORY_H_H
