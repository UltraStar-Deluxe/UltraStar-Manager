#ifndef QUCLEANUPTASK_H_
#define QUCLEANUPTASK_H_

#include "QU.h"
#include "QUSimpleTask.h"

class QUCleanUpTask: public QUSimpleTask {
	Q_OBJECT
	
public:
	QUCleanUpTask(QU::CleanTaskModes mode, QObject *parent = 0);
	
	virtual void startOn(QUSongInterface *song);
	virtual QList<QUSmartSetting*> smartSettings() const;
	
private:
	QU::CleanTaskModes _mode;
	mutable QList<QUSmartSetting*> _smartSettings;
};

#endif /*QUCLEANUPTASK_H_*/
