#ifndef QUPREPARATORYTASK_H_
#define QUPREPARATORYTASK_H_

#include "QU.h"
#include "QUAbstractTask.h"

class QUPreparatoryTask : public QUAbstractTask {
	Q_OBJECT
	
public:
	QUPreparatoryTask(QU::PreparatoryTaskModes mode, QObject *parent = 0);
	
	virtual void startOn(QUSongFile *song);
	
private:
	QU::PreparatoryTaskModes _mode;
};

#endif /*QUPREPARATORYTASK_H_*/
