#ifndef QUCLEANUPTASKFACTORY_H
#define QUCLEANUPTASKFACTORY_H

#include "QUSimpleTaskFactory.h"

class QUCleanUpTaskFactory: public QUSimpleTaskFactory {
public:
	QUCleanUpTaskFactory(QObject *parent = 0);

	virtual QString versionString() const { return "1.0.0"; }
	virtual QString name() const;

protected:
	virtual QUTask* createTask(int type);
	virtual QList<int> types() const;
	virtual QMap<QString, QString> translationLocations() const;
};

#endif // QUCLEANUPTASKFACTORY_H
