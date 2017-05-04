#ifndef QUPDFREPORT_H
#define QUPDFREPORT_H

#include "QUAbstractReport.h"

#include <QFont>
#include <QFontMetrics>
#include <QPrinter>

class QUPdfReport: public QUAbstractReport {
	Q_OBJECT

public:
	QUPdfReport(const QList<QUSongFile*> &songFiles,
			const QList<QUAbstractReportData*> &reportDataList,
			const QFileInfo &fi,
			QU::ReportOptions options = 0,
			const QVariant &userData = QVariant(),
			const QPrinter::PaperSize paperSize = QPrinter::A4,
			const QPrinter::Orientation orientation = QPrinter::Portrait,
			const int layoutColumns = 1,
			const int leftMargin = 10,
			const int rightMargin = 10,
			const int topMargin = 8,
			const int bottomMargin = 8,
			const QFont &letterFnt = QFont("Verdana", 14, QFont::Black, false),
			const QFont &topLevelEntryFnt = QFont("Verdana", 7, QFont::DemiBold, false),
			const QFont &subLevelEntryFnt = QFont("Verdana", 7, QFont::Normal, false),
			const QFont &subSubLevelEntryFnt = QFont("Verdana", 6, QFont::Normal, false),
			const QColor &letterClr = Qt::darkGreen,
			const QColor &topLevelEntryClr = Qt::black,
			const QColor &subLevelEntryClr = Qt::black,
			const QColor &subSubLevelEntryClr = Qt::gray,
			QObject *parent = 0);

	virtual QString content() const { return tr("PDF Report"); }
	virtual bool save();

private:
	QPrinter::PaperSize _paperSize;
	QPrinter::Orientation _orientation;
	int _layoutColumns;
	int _leftMargin;
	int _rightMargin;
	int _topMargin;
	int _bottomMargin;
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
	QColor _letterClr;
	QColor _topLevelEntryClr;
	QColor _subLevelEntryClr;
	QColor _subSubLevelEntryClr;
};

#endif // QUPDFREPORT_H
