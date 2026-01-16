#ifndef QUPREPARATORYTASK_H_
#define QUPREPARATORYTASK_H_

#include "QUSimpleTask.h"

#include <QList>
#include <QString>
#include <QStringList>

class QUSmartSetting;

class QUPreparatoryTask : public QUSimpleTask {
	Q_OBJECT

public:
	enum TaskMode {
		AutoAssignFiles,
		RemoveUnsupportedTags,
		FixAudioLength,
		RoundGap,
		CapitalizeTitle,
		CapitalizeArtist,
		SetEditionIfEmpty,
		SetGenreIfEmpty,
		FixLanguage,
		FixApostrophes,
		AddMissingDuetTags,
		FixLineEndings,
		SetTextVersion
	};
	Q_DECLARE_FLAGS(TaskModes, TaskMode)

	QUPreparatoryTask(TaskModes mode, QObject *parent = 0);
	~QUPreparatoryTask();
	
	virtual void startOn(QUSongInterface *song);
	virtual QList<QUSmartSettingInterface*> smartSettings() const;
	virtual void provideData(const QVariant &data, TaskDataTypes type = UnsupportedTags);

private:
	TaskModes _mode;

	mutable QList<QUSmartSettingInterface*> _smartSettings;
	QStringList _unsupportedTags;

	void autoSetFiles(QUSongInterface *song, const QString &coverPattern, const QString &backgroundPattern);
	void capitalizeTitle(QUSongInterface *song);
	void capitalizeArtist(QUSongInterface *song);
	void setEditionIfEmpty(QUSongInterface *song);
	void setGenreIfEmpty(QUSongInterface *song);
	void fixLanguage(QUSongInterface *song);
	void fixApostrophes(QUSongInterface *song);
	void addMissingDuetTags(QUSongInterface *song);
	void fixLineEndings(QUSongInterface *song);
	void setTextVersion(QUSongInterface *song);
};

#endif /*QUPREPARATORYTASK_H_*/
