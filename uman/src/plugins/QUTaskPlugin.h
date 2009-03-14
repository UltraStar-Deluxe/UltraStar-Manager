#ifndef QUTASKPLUGIN_H_
#define QUTASKPLUGIN_H_

#include <QtPlugin>

class QWidget;
class QString;
class QIcon;

class QUSongInterface;
class QUTask;

class QUTaskFactory {
public:
    virtual ~QUTaskFactory() {}

    // create all tasks based on their configurations
    virtual QList<QUTask*> createTasks() = 0;

    // give it a name so it can be displayed better
    virtual QString name() const = 0;
};

class QUTask {
public:
    virtual ~QUTask() {}

    virtual void startOn(QUSongInterface *song) = 0;

    virtual QIcon icon() const = 0;
    virtual QString description() const = 0;
    virtual QString toolTip() const = 0;
    virtual int group() const = 0;

	virtual int configure(QWidget *parent = 0) = 0;
    virtual bool isConfigurable() const = 0;
};

Q_DECLARE_INTERFACE(QUTaskFactory, "net.sf.uman.QUTaskFactory/1.0");
Q_DECLARE_INTERFACE(QUTask, "net.sf.uman.QUTask/1.0");

#endif // QUPLUGIN_H_
