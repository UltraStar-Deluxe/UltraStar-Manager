#ifndef QUAUDIOTAGTASK_H_
#define QUAUDIOTAGTASK_H_

#include "QU.h"
#include "QUAbstractTask.h"

/*!
 * Use the ID3 tag of an audio file for the change
 * of several song data (e.g. #ARTIST, #TITLE, #GENRE, ...).
 */
class QUAudioTagTask: public QUAbstractTask {
	Q_OBJECT
	
public:
	QUAudioTagTask(QU::AudioTagTaskModes mode, QObject *parent = 0);
	
	virtual void startOn(QUSongFile *song);
	
private:
	QU::AudioTagTaskModes _mode;
};

#endif /*QUAUDIOTAGTASK_H_*/
