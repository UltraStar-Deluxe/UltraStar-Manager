#ifndef QUABSTRACTTASK_H_
#define QUABSTRACTTASK_H_

#include <QObject>
#include <QIcon>

#include "QUSongInterface.h"
#include "QUTaskPlugin.h"
#include "QUSmartSetting.h"

class QUSimpleTask: public QObject, public QUTask {
	Q_OBJECT
    Q_INTERFACES(QUTask)

public:
	QUSimpleTask(QObject *parent = 0);

	virtual QIcon icon() const { return _icon; }
	virtual QString description() const { return _description; }
	virtual QString toolTip() const { return _toolTip; }
	virtual int group() const { return _group; }

	// a simple task is not configurable
	virtual bool isConfigurable() const { return false; }
	virtual int configure(QWidget *parent) { return 0; }

	// a simple task has no smart settings
	virtual QList<QUSmartSettingInterface*> smartSettings() const { return QList<QUSmartSettingInterface*>(); }
	virtual void provideData(const QVariant &data, TaskDataTypes type = UndefinedData) {}

private:
	QIcon   _icon;
	QString _description;
	QString _toolTip;
	int     _group;

protected:
	void setIcon(const QIcon &icon) { _icon = icon; }
	void setDescription(const QString &description) { _description = description; }
	void setToolTip(const QString &toolTip) { _toolTip = toolTip; }
	void setGroup(int group) { _group = group; }
};

#endif /*QUABSTRACTTASK_H_*/
