#include "QUPdfReport.h"
#include "QUProgressDialog.h"
#include "QULogService.h"

#include "QUBooleanSongData.h"
#include "QUSongFileData.h"
#include "QUSongTagData.h"

#include <QPrinter>
#include <QPainter>
#include <QFontMetrics>
#include <QDebug>

QUPdfReport::QUPdfReport(const QList<QUSongFile*> &songFiles,
		const QList<QUAbstractReportData*> &reportDataList,
		const QFileInfo &fi,
		QU::ReportOptions options,
		const QVariant &userData,
		const QPageSize pageSize,
		const QPageLayout::Orientation orientation,
		const int layoutColumns,
		const float leftMargin,
		const float rightMargin,
		const float topMargin,
		const float bottomMargin,
		const QFont &categoryFnt,
		const QFont &topLevelEntryFnt,
		const QFont &subLevelEntryFnt,
		const QFont &subSubLevelEntryFnt,
		const QColor &categoryClr,
		const QColor &topLevelEntryClr,
		const QColor &subLevelEntryClr,
		const QColor &subSubLevelEntryClr,
		const float categoryToTopLevelVSep,
		const float topLevelToSubLevelVSep,
		const float subLevelToSubLevelVSep,
		const float subLevelToTopLevelVSep,
		const float subLevelToCategoryVSep,
		const float subLevelEntryHIndent,
		const float subLevelEntryHSep,
		const float colHSep,
		QObject *parent): QUAbstractReport(songFiles, reportDataList, fi, options, userData, parent)
{
	//
	_pageSize = pageSize;
	_orientation = orientation;
	_layoutColumns = layoutColumns;
	_leftMargin = leftMargin;
	_rightMargin = rightMargin;
	_topMargin = topMargin;
	_bottomMargin = bottomMargin;
	_categoryFnt = categoryFnt;
	_topLevelEntryFnt = topLevelEntryFnt;
	_subLevelEntryFnt = subLevelEntryFnt;
	_subSubLevelEntryFnt = subSubLevelEntryFnt;
	_categoryClr = categoryClr;
	_topLevelEntryClr = topLevelEntryClr;
	_subLevelEntryClr = subLevelEntryClr;
	_subSubLevelEntryClr = subSubLevelEntryClr;
	_categoryToTopLevelVSep = categoryToTopLevelVSep;
	_topLevelToSubLevelVSep = topLevelToSubLevelVSep;
	_subLevelToSubLevelVSep = subLevelToSubLevelVSep;
	_subLevelToTopLevelVSep = subLevelToTopLevelVSep;
	_subLevelToCategoryVSep = subLevelToCategoryVSep;
	_subLevelEntryHIndent = subLevelEntryHIndent;
	_subLevelEntryHSep = subLevelEntryHSep;
	_colHSep = colHSep;

	_numberFnt = QFont("Verdana", 6, QFont::Normal, true);
}

bool QUPdfReport::save() {
	if(reportDataList().size() < 2)
		return false;

	QPrinter printer(QPrinter::HighResolution);
	printer.setOutputFormat(QPrinter::PdfFormat);
	printer.setOutputFileName(fileInfo().filePath());

	printer.setPageSize(_pageSize);
	printer.setPageOrientation(_orientation);
	printer.setPageMargins(QMarginsF(_leftMargin, _topMargin, _rightMargin, _bottomMargin), QPageLayout::Millimeter);

	QPainter painter;
	painter.begin(&printer);

	QUProgressDialog pDlg(tr("Creating PDF report..."), songs().size());
	pDlg.setPixmap(":/types/folder.png");
	pDlg.show();

	QString prevCategory("Zyxel");
	QString previousTopLevelEntry("Zyxel");

	int pageHeight = painter.window().height();
	int columnWidth = (painter.window().width() - (_layoutColumns-1)*_colHSep)/_layoutColumns;
	int x = 0;
	int y = 0;

	if(options().testFlag(QU::reportPrependCurrentPath)) {
		// TODO
	}

	painter.setFont(_categoryFnt);
	int categoryHeight = painter.fontMetrics().height();
	painter.setFont(_topLevelEntryFnt);
	int topLevelEntryHeight = painter.fontMetrics().height();
	painter.setFont(_subLevelEntryFnt);
	int subLevelEntryHeight = painter.fontMetrics().height();

	int subLevelIndex;
	int subSubLevelIndex;
	// find longest entries for sub- and sublevel entries (categories and toplevel entries get full column width)
	if(reportDataList().at(0)->id() == ARTIST_COL || reportDataList().at(0)->id() == TITLE_COL) {
		subLevelIndex = 1;
		subSubLevelIndex = 2;
	} else {
		subLevelIndex = 2;
		subSubLevelIndex = 3;
	}

	QList<int> maxWidth;
	for(int i = 0; i < reportDataList().size(); ++i) {
		maxWidth.insert(i, 0);
	}

	//painter.setFont(_subSubLevelEntryFnt);
	//_booleanHSep = painter.fontMetrics().width(QString::fromUtf8(CHAR_UTF8_CHECK));

	foreach(QUSongFile *song, songs()) {
		for(int i = subLevelIndex; i < reportDataList().size(); ++i) {
			if(i == subLevelIndex) {
				painter.setFont(_subLevelEntryFnt);
			} else {
				painter.setFont(_subSubLevelEntryFnt);
			}
			QString currentEntry = reportDataList().at(i)->textData(song);
			if(currentEntry == tr("yes")) {
				currentEntry = QString::fromUtf8(CHAR_UTF8_CHECK);
				//currentEntry = "y";
			} else if(currentEntry == tr("no")) {
				currentEntry = QString::fromUtf8(CHAR_UTF8_BALLOT);
				//currentEntry = "n";
			}
			int currentEntryWidth = painter.fontMetrics().horizontalAdvance(currentEntry);
			if( currentEntryWidth > maxWidth.at(i)) {
				maxWidth.replace(i, currentEntryWidth);
			}
		}
	}

	int neededWidth = _subLevelEntryHIndent;
	for(int i = subLevelIndex; i < reportDataList().size(); ++i) {
		if(i > subLevelIndex) {
			neededWidth += _subLevelEntryHSep;
		}
		neededWidth += maxWidth.at(i);
	}

	float elideFactor = 1.0;
	if(neededWidth > columnWidth) {
		elideFactor = float(columnWidth - (reportDataList().size() - 1) * _subLevelEntryHSep)/float(neededWidth - (reportDataList().size() - 1) * _subLevelEntryHSep);
	}

	QList<int> actualWidth(maxWidth);
	for(int i = 0; i < reportDataList().size(); ++i) {
		actualWidth.replace(i, int(maxWidth.at(i)*elideFactor));
	}

	bool categoryBefore = false;
	bool topLevelEntryBefore = false;

	int currentColumn = 0;

	foreach(QUSongFile *song, songs()) {
		pDlg.update(QString("%1 - %2").arg(song->artist(), song->title()));
		if(pDlg.cancelled())
			break;

		// running number
		//rn++;
		//painter.setFont(numberFnt);
		//painter.drawText(printer.pageRect(), QString("%1").arg(rn++, max, 10, QChar('0')), Qt::AlignTop | Qt::AlignLeft);
		//painter.translate(200,0);
		//painter.setFont(artist);

		QString category;
		QString topLevelEntry;
		QString subLevelEntry;
		if(reportDataList().at(0)->id() == ARTIST_COL || reportDataList().at(0)->id() == TITLE_COL) {
			// for artist or title as first column, use first letter as category
			category = reportDataList().at(0)->textData(song).at(0).toUpper();
			topLevelEntry = reportDataList().at(0)->textData(song);
			subLevelEntry = reportDataList().at(1)->textData(song);
		} else {
			// for all other fields as first column, use full text as category
			category = reportDataList().at(0)->textData(song).toUpper();
			topLevelEntry = reportDataList().at(1)->textData(song);
			subLevelEntry = reportDataList().at(2)->textData(song);
		}

		if(category != prevCategory) {
			// new category
			x = currentColumn * (columnWidth +_colHSep);

			// check if category + topLevelEntry + first subLevelEntry will fit in current column
			if((y + _subLevelToCategoryVSep + _categoryToTopLevelVSep + _topLevelToSubLevelVSep) > pageHeight) {
				y = categoryHeight;

				currentColumn = (currentColumn + 1) % _layoutColumns;
				x = currentColumn * (columnWidth + _colHSep);
				if(currentColumn == 0) {
					printer.newPage();
				}
			} else {
				if(y == 0)
					y = categoryHeight;
				else
					y += _subLevelToCategoryVSep;
			}

			painter.setPen(_categoryClr);
			painter.setFont(_categoryFnt);
			painter.drawText(x, y, painter.fontMetrics().elidedText(category, Qt::ElideRight, columnWidth));

			prevCategory = category;
			categoryBefore = true;
		}

		if(topLevelEntry != previousTopLevelEntry) {
			// new topLevelEntry
			x = currentColumn * (columnWidth + _colHSep);

			// check if topLevelEntry + first subLevelEntry will fit in current column
			if(categoryBefore) {
				categoryBefore = false;
				y += _categoryToTopLevelVSep;
			}
			else {
				if((y + _subLevelToTopLevelVSep + _topLevelToSubLevelVSep) > pageHeight) {
					y = topLevelEntryHeight;

					currentColumn = (currentColumn + 1) % _layoutColumns;
					x = currentColumn * (columnWidth + _colHSep);
					if(currentColumn == 0) {
						printer.newPage();
					}

				} else {
					y += _subLevelToTopLevelVSep;
				}
			}

			painter.setPen(_topLevelEntryClr);
			painter.setFont(_topLevelEntryFnt);
			painter.drawText(x, y, painter.fontMetrics().elidedText(topLevelEntry, Qt::ElideRight, columnWidth));

			previousTopLevelEntry = topLevelEntry;

			topLevelEntryBefore = true;
		}

		x += _subLevelEntryHIndent;

		// check if topLevelEntry + first subLevelEntry will fit in current column
		if(topLevelEntryBefore) {
			topLevelEntryBefore = false;
			y += _topLevelToSubLevelVSep;
			}
		else {
			if((y + _subLevelToSubLevelVSep) > pageHeight) {
				y = subLevelEntryHeight;

				currentColumn = (currentColumn + 1) % _layoutColumns;
				x = currentColumn * (columnWidth + _colHSep) + _subLevelEntryHIndent;
				if(currentColumn == 0) {
					printer.newPage();
				}
			} else {
				y += _subLevelToSubLevelVSep;
			}
		}

		// paint sublevel entry
		if(reportDataList().size() > 1) {
			painter.setPen(_subLevelEntryClr);
			painter.setFont(_subLevelEntryFnt);
			painter.drawText(x, y, painter.fontMetrics().elidedText(subLevelEntry, Qt::ElideRight, actualWidth.at(subLevelIndex)));
			x += actualWidth.at(subLevelIndex) + _subLevelEntryHSep;
		}

		// paint all subsublevel entries
		for(int i = subSubLevelIndex; i < reportDataList().size(); ++i) {
			painter.setFont(_subSubLevelEntryFnt);
			painter.setPen(_subSubLevelEntryClr);

			/*
			QUBooleanSongData *bData = dynamic_cast<QUBooleanSongData*>(reportDataList().at(i));
			QUSongFileData *fData = dynamic_cast<QUSongFileData*>(reportDataList().at(i));
			QUSongTagData *tData = dynamic_cast<QUSongTagData*>(reportDataList().at(i));
			if(bData)
				logSrv->add(bData->_tag, QU::Information);
			if(fData)
				logSrv->add(fData->_property, QU::Information);
			if(tData)
				logSrv->add(tData->_tag, QU::Information);
			*/

			QString currentEntry = reportDataList().at(i)->textData(song);
			if(currentEntry == tr("yes")) {
				//currentEntry = QString::fromUtf8(CHAR_UTF8_CHECK);
				currentEntry = "y";
			} else if(currentEntry == tr("no")) {
				//currentEntry = QString::fromUtf8(CHAR_UTF8_BALLOT);
				currentEntry = "n";
			}
			painter.drawText(x, y, painter.fontMetrics().elidedText(currentEntry, Qt::ElideRight, actualWidth.at(i)));
			x += actualWidth.at(i) + _subLevelEntryHSep;
		}

		x = currentColumn * (columnWidth + _colHSep);
	}

	painter.end();

	logSrv->add(tr("Adding PDF report...done."), QU::Information);
	return true;
}
