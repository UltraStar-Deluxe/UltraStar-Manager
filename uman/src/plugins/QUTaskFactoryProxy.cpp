#include "QUTaskFactoryProxy.h"

QUTaskFactoryProxy::QUTaskFactoryProxy(QUTaskFactory *factory, QWidget *factoryParent, QObject *parent):
		QObject(parent),
		_factory(factory),
		_factoryParent(factoryParent)
{}


void QUTaskFactoryProxy::addConfiguration() {
	if(_factory->addConfiguration(_factoryParent))
		emit configurationAdded();
}
