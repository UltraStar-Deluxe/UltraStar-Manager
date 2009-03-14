#ifndef QUCLEANUPTASKFACTORY_H
#define QUCLEANUPTASKFACTORY_H

#include "QUSimpleTaskFactory.h"

class QUCleanUpTaskFactory: public QUSimpleTaskFactory {
public:
	QUCleanUpTaskFactory(QObject *parent = 0);

	virtual QString name() const;

protected:
	virtual QUTask* createTask(int type);
	virtual QList<int> types() const;
};

#endif // QUCLEANUPTASKFACTORY_H
