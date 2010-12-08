#ifndef QUTASKFACTORYPROXY_H
#define QUTASKFACTORYPROXY_H

#include <QObject>
#include <QWidget>

#include "QUTaskPlugin.h"

/*!
 * This class ensures that configuration dialogs in plugins have the correct parent widget.
 */
class QUTaskFactoryProxy: public QObject {
	Q_OBJECT

public:
	QUTaskFactoryProxy(QUTaskFactory *factory, QWidget *factoryParent = 0, QObject *parent = 0);
	QUTaskFactory* factory() const { return _factory; }

public slots:
	void addConfiguration();

signals:
	void configurationAdded();

private:
	QUTaskFactory *_factory;
	QWidget *_factoryParent;
};

#endif // QUTASKFACTORYPROXY_H
