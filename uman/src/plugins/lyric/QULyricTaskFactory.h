#ifndef QULYRICTASKFACTORY_H
#define QULYRICTASKFACTORY_H

#include "QUSimpleTaskFactory.h"

class QULyricTaskFactory: public QUSimpleTaskFactory {
public:
	QULyricTaskFactory(QObject *parent = 0);

	virtual QString versionString() const { return "1.0.0"; }
	virtual QString name() const;

protected:
	virtual QUTask* createTask(int type);
	virtual QList<int> types() const;
	virtual QMap<QString, QString> translationLocations() const;
};

#endif // QULYRICTASKFACTORY_H
