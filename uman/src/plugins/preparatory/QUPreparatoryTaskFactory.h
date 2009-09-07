#ifndef QUPREPARATORYTASKFACTORY_H
#define QUPREPARATORYTASKFACTORY_H

#include "QUSimpleTaskFactory.h"

class QUPreparatoryTaskFactory: public QUSimpleTaskFactory {
public:
	QUPreparatoryTaskFactory(QObject *parent = 0);

	virtual QString version() const { return "1.0.0"; }
	virtual QString name() const;

protected:
	virtual QUTask* createTask(int type);
	virtual QList<int> types() const;
	virtual QMap<QString, QString> translationLocations() const;
};

#endif // QUPREPARATORYTASKFACTORY_H
