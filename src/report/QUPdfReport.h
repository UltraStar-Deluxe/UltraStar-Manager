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
	QList<int> _colWidth;
	// vertical spacing
	int _subLevelToLetterVSep;
	int _letterToTopLevelVSep;
	int _topLevelToSubLevelVSep;
	int _subLevelToSubLevelVSep;
	int _subLevelToTopLevelVSep;

	// horizontal spacing
	int _subLevelEntryHIndent;
	int _colHSep;

	int _subLevelEntryHSep;
	int _artistHSep;
	int _titleHSep;
	int _languageHSep;
	int _editionHSep;
	int _genreHSep;
	int _yearHSep;
	int _creatorHSep;
	int _booleanHSep;
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
