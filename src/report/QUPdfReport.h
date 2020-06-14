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
			QU::ReportOptions options,
			const QVariant &userData = QVariant(),
			const QPrinter::PaperSize paperSize = QPrinter::A4,
			const QPrinter::Orientation orientation = QPrinter::Portrait,
			const int layoutColumns = 1,
			const float leftMargin = 16.0,
			const float rightMargin = 8.0,
			const float topMargin = 8.0,
			const float bottomMargin = 8.0,
			const QFont &categoryFnt = QFont("Verdana", 14, QFont::Black, false),
			const QFont &topLevelEntryFnt = QFont("Verdana", 7, QFont::DemiBold, false),
			const QFont &subLevelEntryFnt = QFont("Verdana", 7, QFont::Normal, false),
			const QFont &subSubLevelEntryFnt = QFont("Verdana", 6, QFont::Normal, false),
			const QColor &categoryClr = Qt::darkGreen,
			const QColor &topLevelEntryClr = Qt::black,
			const QColor &subLevelEntryClr = Qt::black,
			const QColor &subSubLevelEntryClr = Qt::gray,
			const float categoryToTopLevelVSep = 12.0,
			const float topLevelToSubLevelVSep = 11.0,
			const float subLevelToSubLevelVSep = 11.0,
			const float subLevelToTopLevelVSep = 12.0,
			const float subLevelToCategoryVSep = 24.0,
			const float subLevelEntryHIndent = 4.0,
			const float subLevelEntryHSep = 4.0,
			const float colHSep = 4.0,
			QObject *parent = nullptr);

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
	int _subLevelToCategoryVSep;
	int _categoryToTopLevelVSep;
	int _topLevelToSubLevelVSep;
	int _subLevelToSubLevelVSep;
	int _subLevelToTopLevelVSep;

	// horizontal spacing
	int _subLevelEntryHIndent;
	int _subLevelEntryHSep;
	int _colHSep;

	QFont _categoryFnt;
	QFont _topLevelEntryFnt;
	QFont _subLevelEntryFnt;
	QFont _subSubLevelEntryFnt;
	QFont _numberFnt;
	QColor _categoryClr;
	QColor _topLevelEntryClr;
	QColor _subLevelEntryClr;
	QColor _subSubLevelEntryClr;
};

#endif // QUPDFREPORT_H
