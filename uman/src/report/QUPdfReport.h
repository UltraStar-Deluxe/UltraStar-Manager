#ifndef QUPDFREPORT_H
#define QUPDFREPORT_H

#include "QUAbstractReport.h"

#include <QFont>
#include <QFontMetrics>

class QUPdfReport: public QUAbstractReport {
	Q_OBJECT

public:
	QUPdfReport(
			const QList<QUSongFile*> &songFiles,
			const QList<QUAbstractReportData*> &reportDataList,
			const QFileInfo &fi,
			QU::ReportOptions options = 0,
			const QVariant &userData = QVariant(),
			QObject *parent = 0);

	virtual QString content() const { return tr("PDF Report"); }
	virtual bool save();

private:
	// vertical spacing
	int _beforeLetterVSep;
	int _afterLetterVSep;
	int _beforeTopLevelEntryVSep;
	int _afterTopLevelEntryVSep;
	int _afterSubLevelEntryVSep;

	// horizontal spacing
	int _subLevelEntryHIndent;
	int _colHSep;

	int _artistHSep;
	int _titleHSep;
	int _languageHSep;
	int _editionHSep;
	int _genreHSep;
	int _yearHSep;
	int _creatorHSep;
	int _audioExistsHSep;
	int _coverExistsHSep;
	int _backgroundExistsHSep;
	int _videoExistsHSep;
	int _speedHSep;
	int _lengthHSep;
	int _songPathHSep;
	int _songFilePathHSep;
	int _relSongFilePathHSep;
	int _defaultHSep;

	QFont _letterFnt;
	QFont _topLevelEntryFnt;
	QFont _subLevelEntryFnt;
	QFont _subSubLevelEntryFnt;
	QFont _numberFnt;
};

#endif // QUPDFREPORT_H
