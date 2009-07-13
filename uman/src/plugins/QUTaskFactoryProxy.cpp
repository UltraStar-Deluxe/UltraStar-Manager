#include "QUTaskFactoryProxy.h"
#include "QULogService.h"
#include "QUTaskList.h"

QUTaskFactoryProxy::QUTaskFactoryProxy(QUTaskFactory *factory, QWidget *factoryParent, QUTaskList *parent):
		QObject(parent),
		_factory(factory),
		_factoryParent(factoryParent)
{}

QUTaskFactoryProxy::~QUTaskFactoryProxy() {
	if(_factory)
		delete _factory;
}

void QUTaskFactoryProxy::addConfiguration() {
	if(_factory->addConfiguration(_factoryParent)) {
		QUTaskList *taskList = dynamic_cast<QUTaskList*>(this->parent());

		if(taskList) {
			taskList->resetTaskList();
			logSrv->add(tr("Task list was refreshed successfully."), QU::Information);
		} else {
			logSrv->add(tr("Factory proxy could not access the task list."), QU::Warning);
		}
	}
}
