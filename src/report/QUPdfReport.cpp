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
		const QPrinter::PaperSize paperSize,
		const QPrinter::Orientation orientation,
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
		const float colHSep,
		QObject *parent): QUAbstractReport(songFiles, reportDataList, fi, options, userData, parent)
{
	//
	_paperSize = paperSize;
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
	_colHSep = colHSep;

	_subLevelEntryHSep		= 2000;
	_artistHSep				= 2000;
	_titleHSep				= 2000;
	_languageHSep			= 500;
	_editionHSep			= 1000;
	_genreHSep				= 1000;
	_yearHSep				= 400;
	_creatorHSep			= 700;
	_booleanHSep			= 100;
	_speedHSep				= 400;
	_lengthHSep				= 400;
	_songPathHSep			= 2000;
	_songFilePathHSep		= 2000;
	_relSongFilePathHSep	= 2000;
	_defaultHSep			= 1000;

	_numberFnt				= QFont("Verdana", 6, QFont::Normal, true);
}

bool QUPdfReport::save() {
	if(reportDataList().size() < 2)
		return false;

	QPrinter printer(QPrinter::HighResolution);
	printer.setOutputFormat(QPrinter::PdfFormat);
	printer.setOutputFileName(fileInfo().filePath());

	printer.setPaperSize(_paperSize);
	printer.setOrientation(_orientation);
	printer.setPageMargins(_leftMargin, _topMargin, _rightMargin, _bottomMargin, QPrinter::Millimeter);

	QPainter painter;
	painter.begin(&printer);

	QUProgressDialog pDlg(tr("Creating PDF report..."), songs().size());
	pDlg.setPixmap(":/types/folder.png");
	pDlg.show();

	QString prevCategory("Zyxel");
	QString previousTopLevelEntry("Zyxel");

	int pageHeight = painter.window().height();
	int columnWidth = painter.window().width()/_layoutColumns;
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

	// find longest entries
	int maxSubLevelEntryWidth	= 0;
	int maxArtistWidth			= 0;
	int maxTitleWidth			= 0;
	int maxLanguageWidth		= 0;
	int maxEditionWidth			= 0;
	int maxGenreWidth			= 0;
	int maxYearWidth			= 0;
	int maxCreatorWidth			= 0;
	int maxSpeedWidth			= 0;
	int maxLengthWidth			= 0;
	int maxPathWidth			= 0;
	int maxFilePathWidth		= 0;
	int maxRelFilePathWidth		= 0;

	painter.setFont(_subSubLevelEntryFnt);
	_booleanHSep = painter.fontMetrics().width(QString::fromUtf8(CHAR_UTF8_CHECK));


	foreach(QUSongFile *song, songs()) {
		painter.setFont(_subLevelEntryFnt);
		int currentSubLevelEntryWidth = painter.fontMetrics().width(reportDataList().at(1)->textData(song));
		maxSubLevelEntryWidth = (currentSubLevelEntryWidth > maxSubLevelEntryWidth) ? currentSubLevelEntryWidth : maxSubLevelEntryWidth;
		painter.setFont(_subSubLevelEntryFnt);
		for(int i = 2; i < reportDataList().size(); ++i) {
			int currentEntryWidth = painter.fontMetrics().width(reportDataList().at(i)->textData(song));
			int currentColumn = reportDataList().at(i)->id();

			switch(currentColumn)
			{
			case ARTIST_COL:
				maxArtistWidth = (currentEntryWidth > maxArtistWidth) ? currentEntryWidth : maxArtistWidth;
				break;
			case TITLE_COL:
				maxTitleWidth = (currentEntryWidth > maxTitleWidth) ? currentEntryWidth : maxTitleWidth;
				break;
			case LANGUAGE_COL:
				maxLanguageWidth = (currentEntryWidth > maxLanguageWidth) ? currentEntryWidth : maxLanguageWidth;
				break;
			case EDITION_COL:
				maxEditionWidth = (currentEntryWidth > maxEditionWidth) ? currentEntryWidth : maxEditionWidth;
				break;
			case GENRE_COL:
				maxGenreWidth = (currentEntryWidth > maxGenreWidth) ? currentEntryWidth : maxGenreWidth;
				break;
			case YEAR_COL:
				//maxYearWidth = (currentEntryWidth > maxYearWidth) ? currentEntryWidth : maxYearWidth;
				break;
			case CREATOR_COL:
				maxCreatorWidth = (currentEntryWidth > maxCreatorWidth) ? currentEntryWidth : maxCreatorWidth;
				break;
			case SPEED_COL:
				maxSpeedWidth = (currentEntryWidth > maxSpeedWidth) ? currentEntryWidth : maxSpeedWidth;
				break;
			case LENGTH_COL:
				maxLengthWidth = (currentEntryWidth > maxLengthWidth) ? currentEntryWidth : maxLengthWidth;
				break;
			case SONG_PATH_COL:
				maxPathWidth = (currentEntryWidth > maxPathWidth) ? currentEntryWidth : maxPathWidth;
				break;
			case SONG_FILE_PATH_COL:
				maxFilePathWidth = (currentEntryWidth > maxFilePathWidth) ? currentEntryWidth : maxFilePathWidth;
				break;
			case REL_SONG_FILE_PATH_COL:
				maxRelFilePathWidth = (currentEntryWidth > maxRelFilePathWidth) ? currentEntryWidth : maxRelFilePathWidth;
			}
		}
	}

	_subLevelEntryHSep		= qMin(_subLevelEntryHSep, maxSubLevelEntryWidth);
	_artistHSep				= qMin(_artistHSep, maxArtistWidth);
	_titleHSep				= qMin(_titleHSep, maxTitleWidth);
	_languageHSep			= qMin(_languageHSep, maxLanguageWidth);
	_editionHSep			= qMin(_editionHSep, maxEditionWidth);
	_genreHSep				= qMin(_genreHSep, maxGenreWidth);
	_yearHSep				= 400;//qMin(_yearHSep, maxYearWidth);
	_creatorHSep			= qMin(_creatorHSep, maxCreatorWidth);
	_speedHSep				= qMin(_speedHSep, maxSpeedWidth);
	_lengthHSep				= qMin(_lengthHSep, maxLengthWidth);
	_songPathHSep			= qMin(_songPathHSep, maxPathWidth);
	_songFilePathHSep		= qMin(_songFilePathHSep, maxFilePathWidth);
	_relSongFilePathHSep	= qMin(_relSongFilePathHSep, maxRelFilePathWidth);

	bool categoryBefore = false;
	bool topLevelEntryBefore = false;

	/* // find optimal colHSep to spread contents over page width
	int usedLineWidth = _subLevelEntryHIndent + _subLevelEntryHSep;
	for(int i = 2; i < reportDataList().size(); ++i) {
		int currentColumn = reportDataList().at(i)->id();

		if(currentColumn == ARTIST_COL) {
			usedLineWidth += _artistHSep;
		} else if(currentColumn == TITLE_COL) {
			usedLineWidth += _titleHSep;
		} else if(currentColumn == LANGUAGE_COL) {
			usedLineWidth += _languageHSep;
		} else if(currentColumn == EDITION_COL) {
			usedLineWidth += _editionHSep;
		} else if(currentColumn == GENRE_COL) {
			usedLineWidth += _genreHSep;
		} else if(currentColumn == YEAR_COL) {
			usedLineWidth += _yearHSep;
		} else if(currentColumn == CREATOR_COL) {
			usedLineWidth += _creatorHSep;
		} else if(currentColumn == SPEED_COL) {
			usedLineWidth += _speedHSep;
		} else if(currentColumn == LENGTH_COL) {
			usedLineWidth += _lengthHSep;
		} else if(currentColumn == SONG_PATH_COL) {
			usedLineWidth += _songPathHSep;
		} else if(currentColumn == SONG_FILE_PATH_COL) {
			usedLineWidth += _songFilePathHSep;
		} else if(currentColumn == REL_SONG_FILE_PATH_COL) {
			usedLineWidth += _relSongFilePathHSep;
		} else if(currentColumn == CUSTOM_TAG_COL)
			usedLineWidth += _defaultHSep;
	}

	if(usedLineWidth < painter.window().width() && reportDataList().length() > 2)
		_colHSep = qMax(_colHSep, (painter.window().width() - usedLineWidth) / (reportDataList().length() - 2));

	if(usedLineWidth > painter.window().width())
		logSrv->add(tr(""), QU::Warning);
	*/

	int currentColumn = 0;

	foreach(QUSongFile *song, songs()) {
		pDlg.update(QString("%1 - %2").arg(song->artist()).arg(song->title()));
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
		int subLevelIndex;
		if(reportDataList().at(0)->id() == ARTIST_COL || reportDataList().at(0)->id() == TITLE_COL) {
			// for artist or title as first column, use first letter as category
			category = reportDataList().at(0)->textData(song).at(0).toUpper();
			topLevelEntry = reportDataList().at(0)->textData(song);
			subLevelEntry = reportDataList().at(1)->textData(song);
			subLevelIndex = 2;
		} else {
			// for all other fields as first column, use full text as category
			category = reportDataList().at(0)->textData(song).toUpper();
			topLevelEntry = reportDataList().at(1)->textData(song);
			subLevelEntry = reportDataList().at(2)->textData(song);
			subLevelIndex = 3;
		}

		if(category != prevCategory) {
			// new category
			x = currentColumn * columnWidth;
			if(currentColumn > 0) {
				x += _colHSep/2;
			}

			// check if category + topLevelEntry + first subLevelEntry will fit in current column
			if((y + _subLevelToCategoryVSep + _categoryToTopLevelVSep + _topLevelToSubLevelVSep) > pageHeight) {
				y = categoryHeight;

				currentColumn = (currentColumn + 1) % _layoutColumns;
				x = currentColumn * columnWidth;
				if(currentColumn > 0) {
					x += _colHSep/2;
				} else {
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
			painter.drawText(x, y, painter.fontMetrics().elidedText(category, Qt::ElideRight, columnWidth - _colHSep/2));

			prevCategory = category;
			categoryBefore = true;
		}

		if(topLevelEntry != previousTopLevelEntry) {
			// new topLevelEntry
			x = currentColumn * columnWidth;
			if(currentColumn > 0) {
				x += _colHSep/2;
			}

			// check if topLevelEntry + first subLevelEntry will fit in current column
			if(categoryBefore) {
				categoryBefore = false;
				y += _categoryToTopLevelVSep;
			}
			else {
				if((y + _subLevelToTopLevelVSep + _topLevelToSubLevelVSep) > pageHeight) {
					y = topLevelEntryHeight;

					currentColumn = (currentColumn + 1) % _layoutColumns;
					x = currentColumn * columnWidth;
					if(currentColumn > 0) {
						x += _colHSep/2;
					} else {
						printer.newPage();
					}

				} else {
					y += _subLevelToTopLevelVSep;
				}
			}

			painter.setPen(_topLevelEntryClr);
			painter.setFont(_topLevelEntryFnt);
			painter.drawText(x, y, painter.fontMetrics().elidedText(topLevelEntry, Qt::ElideRight, columnWidth - _colHSep/2));

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
				x = currentColumn * columnWidth + _subLevelEntryHIndent;
				if(currentColumn > 0) {
					x += _colHSep/2;
				} else {
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
			painter.drawText(x, y, painter.fontMetrics().elidedText(subLevelEntry, Qt::ElideRight, qMin(_subLevelEntryHSep, columnWidth - _subLevelEntryHIndent - _colHSep/2)));
			x += _subLevelEntryHSep + _colHSep;
		}

		// paint all subsublevel entries
		for(subLevelIndex; subLevelIndex < reportDataList().size(); ++subLevelIndex) {
			painter.setFont(_subSubLevelEntryFnt);

			/*
			QUBooleanSongData *bData = dynamic_cast<QUBooleanSongData*>(reportDataList().at(subLevelIndex));
			QUSongFileData *fData = dynamic_cast<QUSongFileData*>(reportDataList().at(subLevelIndex));
			QUSongTagData *tData = dynamic_cast<QUSongTagData*>(reportDataList().at(subLevelIndex));
			if(bData)
				logSrv->add(bData->_tag, QU::Information);
			if(fData)
				logSrv->add(fData->_property, QU::Information);
			if(tData)
				logSrv->add(tData->_tag, QU::Information);
			*/

			int currentColumn = reportDataList().at(subLevelIndex)->id();

			if(currentColumn == ARTIST_COL) {
				painter.setPen(_subSubLevelEntryClr);
				painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(subLevelIndex)->textData(song), Qt::ElideRight, _artistHSep));
				x += _artistHSep + _colHSep;
			} else if(currentColumn == TITLE_COL) {
				painter.setPen(_subSubLevelEntryClr);
				painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(subLevelIndex)->textData(song), Qt::ElideRight, _titleHSep));
				x += _titleHSep + _colHSep;
			} else if(currentColumn == LANGUAGE_COL) {
				painter.setPen(_subSubLevelEntryClr);
				painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(subLevelIndex)->textData(song), Qt::ElideRight, _languageHSep));
				x += _languageHSep + _colHSep;
			} else if(currentColumn == EDITION_COL) {
				painter.setPen(_subSubLevelEntryClr);
				painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(subLevelIndex)->textData(song), Qt::ElideRight, _editionHSep));
				x += _editionHSep + _colHSep;
			} else if(currentColumn == GENRE_COL) {
				painter.setPen(_subSubLevelEntryClr);
				painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(subLevelIndex)->textData(song), Qt::ElideRight, _genreHSep));
				x +=_genreHSep + _colHSep;
			} else if(currentColumn == YEAR_COL) {
				painter.setPen(_subSubLevelEntryClr);
				painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(subLevelIndex)->textData(song), Qt::ElideRight, _yearHSep));
				x += _yearHSep + _colHSep;
			} else if(currentColumn == CREATOR_COL) {
				painter.setPen(_subSubLevelEntryClr);
				painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(subLevelIndex)->textData(song), Qt::ElideRight, _creatorHSep));
				x += _creatorHSep + _colHSep;
			} else if(currentColumn == SPEED_COL) {
				painter.setPen(_subSubLevelEntryClr);
				painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(subLevelIndex)->textData(song), Qt::ElideRight, _speedHSep));
				x += _speedHSep + _colHSep;
			} else if(currentColumn == LENGTH_COL) {
				painter.setPen(_subSubLevelEntryClr);
				painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(subLevelIndex)->textData(song), Qt::ElideRight, _lengthHSep));
				x += _lengthHSep + _colHSep;
			} else if(currentColumn == SONG_PATH_COL) {
				painter.setPen(_subSubLevelEntryClr);
				painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(subLevelIndex)->textData(song), Qt::ElideLeft, _songPathHSep));
				x += _songPathHSep + _colHSep;
			} else if(currentColumn == SONG_FILE_PATH_COL) {
				painter.setPen(_subSubLevelEntryClr);
				painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(subLevelIndex)->textData(song), Qt::ElideLeft, _songFilePathHSep));
				x += _songFilePathHSep + _colHSep;
			} else if(currentColumn == REL_SONG_FILE_PATH_COL) {
				painter.setPen(_subSubLevelEntryClr);
				painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(subLevelIndex)->textData(song), Qt::ElideLeft, _relSongFilePathHSep));
				x += _relSongFilePathHSep + _colHSep;
			} else if(currentColumn == AUDIO_EXISTS_COL ||
					  currentColumn == COVER_EXISTS_COL ||
					  currentColumn == BACKGROUND_EXISTS_COL ||
					  currentColumn == VIDEO_EXISTS_COL ||
					  currentColumn == MEDLEY_EXISTS_COL ||
					  currentColumn == GOLDEN_NOTES_EXIST_COL ||
					  currentColumn == RAP_NOTES_EXIST_COL) {
				if(reportDataList().at(subLevelIndex)->textData(song) == tr("yes")) {
					painter.setPen(_subSubLevelEntryClr);
					painter.drawText(x, y, QString::fromUtf8(CHAR_UTF8_CHECK));
					//painter.drawText(x, y, "y");
				} else {
					painter.setPen(_subSubLevelEntryClr);
					painter.drawText(x, y, QString::fromUtf8(CHAR_UTF8_BALLOT));
					//painter.drawText(x, y, "n");
				}
				x += _booleanHSep + _colHSep;
			} else {
				painter.setPen(_subSubLevelEntryClr);
				painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(subLevelIndex)->textData(song), Qt::ElideRight, _defaultHSep));
				x += _defaultHSep + _colHSep;
			}
		}

		x = currentColumn * columnWidth;
		if(currentColumn > 0) {
			x += _colHSep/2;
		}

	}

	painter.end();

	logSrv->add(tr("Adding PDF report...done."), QU::Information);
	return true;
}
