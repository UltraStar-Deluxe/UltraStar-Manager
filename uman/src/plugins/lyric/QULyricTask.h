#ifndef QULYRICTASK_H_
#define QULYRICTASK_H_

#include "QU.h"
#include "QUSimpleTask.h"

class QULyricTask: public QUSimpleTask {
	Q_OBJECT

public:
	QULyricTask(QU::LyricTaskModes mode, QObject *parent = 0);

	virtual void startOn(QUSongInterface *song);

private:
	QU::LyricTaskModes _mode;
};

#endif /* QULYRICTASK_H_ */
