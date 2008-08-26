#ifndef QURENAMETASK_H_
#define QURENAMETASK_H_

#include "QU.h"
#include "QUScriptableTask.h"

class QURenameTask: public QUScriptableTask {
	Q_OBJECT

public:
	QURenameTask(QDomDocument *taskConfig, QObject *parent = 0);

	virtual void startOn(QUSongFile *song);

	static QStringList availableTargets();

	static QStringList availableSources();
	static QStringList availableSpecialSources();
	static QStringList availableCommonSources();

private:
	QStringList filterUnknownTags(const QString &text);
};

#endif /*QURENAMETASK_H_*/
