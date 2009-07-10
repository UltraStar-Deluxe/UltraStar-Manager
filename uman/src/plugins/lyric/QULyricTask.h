#ifndef QULYRICTASK_H_
#define QULYRICTASK_H_

#include "QU.h"
#include "QUSimpleTask.h"

class QUSmartSetting;

class QULyricTask: public QUSimpleTask {
	Q_OBJECT

public:
	QULyricTask(QU::LyricTaskModes mode, QObject *parent = 0);

	virtual void startOn(QUSongInterface *song);
	virtual QList<QUSmartSetting*> smartSettings() const;

private:
	QU::LyricTaskModes _mode;
	mutable QList<QUSmartSetting*> _smartSettings;

	void fixTimeStamps(QUSongInterface *song, int start = 0);
	void fixSpaces(QUSongInterface *song);
	void removeEmptySyllables(QUSongInterface *song);
	void convertSyllablePlaceholder(QUSongInterface *song, const QString &before, const QString &after);
};

#endif /* QULYRICTASK_H_ */
