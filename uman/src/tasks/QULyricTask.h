#ifndef QULYRICTASK_H_
#define QULYRICTASK_H_

#include "QU.h"
#include "QUAbstractTask.h"

class QULyricTask: public QUAbstractTask {
	Q_OBJECT

public:
	QULyricTask(QU::LyricTaskModes mode, QObject *parent = 0);

	virtual void startOn(QUSongFile *song);

private:
	QU::LyricTaskModes _mode;
};

#endif /* QULYRICTASK_H_ */
