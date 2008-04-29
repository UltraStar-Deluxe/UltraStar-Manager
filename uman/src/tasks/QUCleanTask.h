#ifndef QUCLEANTASK_H_
#define QUCLEANTASK_H_

#include "QU.h"
#include "QUAbstractTask.h"

class QUCleanTask: public QUAbstractTask {
	Q_OBJECT
	
public:
	QUCleanTask(QU::CleanTaskModes mode, QObject *parent = 0);
	
	virtual void startOn(QUSongFile *song);
	
private:
	QU::CleanTaskModes _mode;
};

#endif /*QUCLEANTASK_H_*/
