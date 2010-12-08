#ifndef QULYRICTASKFACTORY_H
#define QULYRICTASKFACTORY_H

#include "QUSimpleTaskFactory.h"

class QULyricTaskFactory: public QUSimpleTaskFactory {
	Q_OBJECT

public:
	QULyricTaskFactory(QObject *parent = 0);

	virtual QString version() const { return "1.0.0"; }
	virtual QString name() const;
	virtual QString description() const;
	virtual QString author() const { return "Marcel Taeumel"; }

protected:
	virtual QUTask* createTask(int type);
	virtual QList<int> types() const;
	virtual QMap<QString, QString> translationLocations() const;
};

#endif // QULYRICTASKFACTORY_H
