#ifndef QUSCRIPTABLETASKFACTORY_H_H
#define QUSCRIPTABLETASKFACTORY_H_H

#include <QObject>
#include <QDir>
#include <QDomDocument>

#include "QUTaskPlugin.h"

class QUScriptableTaskFactory: public QObject, public QUTaskFactory {
	Q_OBJECT
    Q_INTERFACES(QUTaskFactory)

public:
	QUScriptableTaskFactory(QObject *parent = 0);

    virtual QList<QUTask*> createTasks();

	virtual bool canAddConfigurations() const { return true; }

protected:
	virtual QDir configurationDirectory() = 0;
	virtual QUTask* createTask(QDomDocument *configuration) = 0;

private:
    QList<QDomDocument*> loadTaskConfigurations();
};

#endif // QUSCRIPTABLETASKFACTORY_H_H
