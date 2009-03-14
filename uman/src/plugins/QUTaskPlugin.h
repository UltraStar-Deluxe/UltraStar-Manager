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

	// use a name to group all kinds of products of this factory
    virtual QString name() const = 0;

	// use product names to give correct information about what is being added
	// in case of adding a configuration
	virtual QString productName() const = 0;

	// enable the user to create new configurrations for this plugin
	virtual int addConfiguration(QWidget *parent = 0) = 0;
	virtual bool canAddConfigurations() const = 0;
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
