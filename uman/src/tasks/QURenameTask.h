#ifndef QURENAMETASK_H_
#define QURENAMETASK_H_

#include "QU.h"
#include "QUAbstractTask.h"

class QURenameTask: public QUAbstractTask {
	Q_OBJECT
	
public:
	QURenameTask(QU::RenameTaskModes mode, QObject *parent = 0);
	
	virtual void startOn(QUSongFile *song);
	
	QU::RenameTaskModes mode() const { return _mode; }
	
private:
	QU::RenameTaskModes _mode;
	
	void renameSongDirSpecial(QUSongFile *song);
	void renameSongVideoSpecial(QUSongFile *song);
};

#endif /*QURENAMETASK_H_*/
