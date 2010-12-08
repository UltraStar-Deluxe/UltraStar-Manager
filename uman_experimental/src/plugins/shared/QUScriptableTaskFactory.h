#ifndef QUSCRIPTABLETASKFACTORY_H_H
#define QUSCRIPTABLETASKFACTORY_H_H

#include <QDir>
#include <QDomDocument>

#include "QUMultilingualTaskFactory.h"

class QUScriptableTaskFactory: public QUMultilingualTaskFactory {
	Q_OBJECT

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
