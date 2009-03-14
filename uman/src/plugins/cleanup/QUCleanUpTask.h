#ifndef QUCLEANUPTASK_H_
#define QUCLEANUPTASK_H_

#include "QU.h"
#include "QUSimpleTask.h"

class QUCleanUpTask: public QUSimpleTask {
	Q_OBJECT
	
public:
	QUCleanUpTask(QU::CleanTaskModes mode, QObject *parent = 0);
	
	virtual void startOn(QUSongInterface *song);
	
private:
	QU::CleanTaskModes _mode;
};

#endif /*QUCLEANUPTASK_H_*/
