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
		FixOverlappingNotes,
		FixLineBreaks,
		FixSpaces,
		FixApostrophes,
		FixLowBPM,
		SetMedleyTags,
		FixLineCapitalization,
		NormalizePitches,
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
	void fixOverlappingNotes(QUSongInterface *song);
	void fixLineBreaks(QUSongInterface *song);
	void fixSpaces(QUSongInterface *song);
	void fixApostrophes(QUSongInterface *song);
	void fixLowBPM(QUSongInterface *song, int threshold = 200);
	void setMedleyTags(QUSongInterface *song, int medleyMinDuration = 30, bool overwriteExisting = false);
	void fixLineCapitalization(QUSongInterface *song);
	void normalizePitches(QUSongInterface *song);
	void removeEmptySyllables(QUSongInterface *song);
	void convertSyllablePlaceholder(QUSongInterface *song, const QString &before, const QString &after);
	void convertRelativeToAbsolute(QUSongInterface *song);
	void convertAbsoluteToRelative(QUSongInterface *song);
};

#endif /* QULYRICTASK_H_ */
