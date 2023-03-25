#ifndef QUCLEANUPTASK_H_
#define QUCLEANUPTASK_H_

#include "QU.h"
#include "QUSimpleTask.h"

#include "audioproperties.h"
#include "fileref.h"
#include "tag.h"
#include "tstring.h"

class QUCleanUpTask: public QUSimpleTask {
	Q_OBJECT
	
public:
	enum TaskMode {
		UnusedFiles,
		InvalidFileTags,
		RemoveEndTag,
		SetID3Tags
	};
	Q_DECLARE_FLAGS(TaskModes, TaskMode)

	QUCleanUpTask(TaskModes mode, QObject *parent = nullptr);
	
	virtual void startOn(QUSongInterface *song);
	virtual QList<QUSmartSettingInterface*> smartSettings() const;
	
private:
	TaskModes _mode;
	mutable QList<QUSmartSettingInterface*> _smartSettings;
	
	QUSongInterface *_currentSong;

	TagLib::FileRef ref();
};

#endif /*QUCLEANUPTASK_H_*/
