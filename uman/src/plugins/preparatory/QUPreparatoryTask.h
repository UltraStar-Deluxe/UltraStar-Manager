#ifndef QUPREPARATORYTASK_H_
#define QUPREPARATORYTASK_H_

#include "QU.h"
#include "QUSimpleTask.h"

#include <QList>
#include <QStringList>
class QUSmartSetting;

class QUPreparatoryTask : public QUSimpleTask {
	Q_OBJECT
	
public:
	QUPreparatoryTask(QU::PreparatoryTaskModes mode, QObject *parent = 0);
	~QUPreparatoryTask();
	
	virtual void startOn(QUSongInterface *song);
	virtual QList<QUSmartSetting*> smartSettings() const;
	virtual void provideData(const QVariant &data, QU::TaskDataTypes type = QU::unsupportedTags);

private:
	QU::PreparatoryTaskModes _mode;

	mutable QList<QUSmartSetting*> _smartSettings;
	QStringList _unsupportedTags;
};

#endif /*QUPREPARATORYTASK_H_*/
