#ifndef QUSIMPLETASKFACTORY_H
#define QUSIMPLETASKFACTORY_H

#include <QObject>
#include <QList>

#include "QUTaskPlugin.h"
#include "QUSongInterface.h"

class QUSimpleTaskFactory: public QObject, public QUTaskFactory {
	Q_OBJECT
	Q_INTERFACES(QUTaskFactory)

public:
	QUSimpleTaskFactory(QObject *parent = 0);

	virtual int pluginApiVersion() const { return PLUGIN_API_VERSION; }
	virtual int songApiVersion() const { return SONG_API_VERSION; }
	virtual QMap<QString, QTranslator*> translations() const { return QMap<QString, QTranslator*>(); }

	virtual QList<QUTask*> createTasks();

	virtual QString productName() const { return tr("Simple Task"); }

	virtual int addConfiguration(QWidget *parent = 0) { return 0; }
	virtual bool canAddConfigurations() const { return false; }

protected:
	virtual QUTask* createTask(int type) = 0;
	virtual QList<int> types() const = 0;
};

#endif // QUSIMPLETASKFACTORY_H
