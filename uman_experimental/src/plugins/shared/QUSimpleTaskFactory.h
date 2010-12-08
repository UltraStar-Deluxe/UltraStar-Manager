#ifndef QUSIMPLETASKFACTORY_H
#define QUSIMPLETASKFACTORY_H

#include <QList>

#include "QUMultilingualTaskFactory.h"

class QUSimpleTaskFactory: public QUMultilingualTaskFactory {
	Q_OBJECT

public:
	QUSimpleTaskFactory(QObject *parent = 0);

	virtual QList<QUTask*> createTasks();

	virtual QString productName() const { return tr("Simple Task"); }

	virtual int addConfiguration(QWidget *parent = 0) { return 0; }
	virtual bool canAddConfigurations() const { return false; }

protected:
	virtual QUTask* createTask(int type) = 0;
	virtual QList<int> types() const = 0;
};

#endif // QUSIMPLETASKFACTORY_H
