#ifndef QUTASKFACTORYPROXY_H
#define QUTASKFACTORYPROXY_H

#include <QObject>
#include <QWidget>

#include "QUTaskPlugin.h"

class QUTaskList;

class QUTaskFactoryProxy: public QObject {
	Q_OBJECT

public:
	QUTaskFactoryProxy(QUTaskFactory *factory, QWidget *factoryParent = 0, QUTaskList *parent = 0);

	QUTaskFactory* factory() const { return _factory; }

public slots:
	void addConfiguration();

private:
	QUTaskFactory *_factory;
	QWidget *_factoryParent;
};

#endif // QUTASKFACTORYPROXY_H
