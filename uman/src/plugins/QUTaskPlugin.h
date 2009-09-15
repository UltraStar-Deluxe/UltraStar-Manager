#ifndef QUTASKPLUGIN_H_
#define QUTASKPLUGIN_H_

#define PLUGIN_API_VERSION 1

#include "QUPlugin.h"

#include <QVariant>

class QWidget;
class QString;
class QIcon;
class QTranslator;

class QUSongInterface;
class QUTask;
class QUSmartSettingInterface;

class QUTaskFactory: public QUPlugin {
	Q_INTERFACES(QUPlugin)

public:
    virtual ~QUTaskFactory() {}

    // create all tasks based on their configurations
	virtual QList<QUTask*> createTasks() = 0;

	// use product names to give correct information about what is being added
	// in case of adding a configuration
	virtual QString productName() const = 0;

	// enable the user to create new configurrations for this plugin
	virtual int addConfiguration(QWidget *parent = 0) = 0;
	virtual bool canAddConfigurations() const = 0;
};

class QUTask {
public:
	enum TaskDataType {
		UndefinedData = -1,
		UnsupportedTags = 0
	};
	Q_DECLARE_FLAGS(TaskDataTypes, TaskDataType)

    virtual ~QUTask() {}

    virtual void startOn(QUSongInterface *song) = 0;

    virtual QIcon icon() const = 0;
    virtual QString description() const = 0;
    virtual QString toolTip() const = 0;
    virtual int group() const = 0;

	virtual int configure(QWidget *parent = 0) = 0;
    virtual bool isConfigurable() const = 0;

	// smart settings (small inline configuration possibilities in the task list)
	virtual QList<QUSmartSettingInterface*> smartSettings() const = 0;
	virtual void provideData(const QVariant &data, TaskDataTypes type = UndefinedData) = 0;
};

Q_DECLARE_INTERFACE(QUTaskFactory, "net.sf.uman.QUTaskFactory/1.0");
Q_DECLARE_INTERFACE(QUTask, "net.sf.uman.QUTask/1.0");

#endif // QUPLUGIN_H_
