#ifndef QULYRICTASK_H_
#define QULYRICTASK_H_

#include "QU.h"
#include "QUSimpleTask.h"

class QUSmartSetting;

class QULyricTask: public QUSimpleTask {
	Q_OBJECT

public:
	enum TaskMode {
		FixTimeStamps,
		FixSpaces,
		RemoveEmptySyllables,
		ConvertSyllablePlaceholder1,
		ConvertSyllablePlaceholder2,
		ConvertRelativeToAbsolute,
		ConvertAbsoluteToRelative
	};
	Q_DECLARE_FLAGS(TaskModes, TaskMode)

	QULyricTask(TaskModes mode, QObject *parent = 0);

	virtual void startOn(QUSongInterface *song);
	virtual QList<QUSmartSettingInterface*> smartSettings() const;

private:
	TaskModes _mode;
	mutable QList<QUSmartSettingInterface*> _smartSettings;

	void fixTimeStamps(QUSongInterface *song, int start = 0);
	void fixSpaces(QUSongInterface *song);
	void removeEmptySyllables(QUSongInterface *song);
	void convertSyllablePlaceholder(QUSongInterface *song, const QString &before, const QString &after);
	void convertRelativeToAbsolute(QUSongInterface *song);
	void convertAbsoluteToRelative(QUSongInterface *song);
};

#endif /* QULYRICTASK_H_ */
