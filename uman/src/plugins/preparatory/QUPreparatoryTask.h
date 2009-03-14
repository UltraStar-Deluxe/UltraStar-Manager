#ifndef QUPREPARATORYTASK_H_
#define QUPREPARATORYTASK_H_

#include "QU.h"
#include "QUSimpleTask.h"

class QUPreparatoryTask : public QUSimpleTask {
	Q_OBJECT
	
public:
	QUPreparatoryTask(QU::PreparatoryTaskModes mode, QObject *parent = 0);
	
	virtual void startOn(QUSongInterface *song);
	
private:
	QU::PreparatoryTaskModes _mode;
};

#endif /*QUPREPARATORYTASK_H_*/
