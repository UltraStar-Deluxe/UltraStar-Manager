#include "QUPdfReport.h"
#include "QUProgressDialog.h"
#include "QULogService.h"

#include "QUBooleanSongData.h"
#include "QUSongFileData.h"
#include "QUSongTagData.h"

#include <QPrinter>
#include <QPainter>
#include <QFontMetrics>

QUPdfReport::QUPdfReport(
		const QList<QUSongFile*> &songFiles,
		const QList<QUAbstractReportData*> &reportDataList,
		const QFileInfo &fi,
		QU::ReportOptions options,
		const QVariant &userData,
		QObject *parent): QUAbstractReport(songFiles, reportDataList, fi, options, userData, parent)
{
	// vertical spacing
	_letterToTopLevelVSep	= 200;
	_topLevelToSubLevelVSep	= 180;
	_subLevelToSubLevelVSep	= 180;
	_subLevelToTopLevelVSep	= 200;
	_subLevelToLetterVSep	= 400;

	// horizontal spacing
	_subLevelEntryHIndent	= 120;
	_colHSep				= 120;

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

	_letterFnt				= QFont("Verdana", 14, QFont::Black, false);
	_topLevelEntryFnt		= QFont("Verdana", 7, QFont::DemiBold, false);
	_subLevelEntryFnt		= QFont("Verdana", 7, QFont::Normal, false);
	_subSubLevelEntryFnt	= QFont("Verdana", 6, QFont::Normal, false);
	_numberFnt				= QFont("Verdana", 6, QFont::Normal, true);
}

bool QUPdfReport::save() {
	if(reportDataList().size() < 2)
		return false;

	QPrinter printer(QPrinter::HighResolution);
	printer.setOutputFormat(QPrinter::PdfFormat);
	printer.setOutputFileName(fileInfo().filePath());
	printer.setPaperSize(QPrinter::A4);
	printer.setPageMargins(10, 8, 10, 0, QPrinter::Millimeter);

	QPainter painter;
	painter.begin(&printer);

	QUProgressDialog pDlg(tr("Creating PDF report..."), songs().size());
	pDlg.setPixmap(":/types/folder.png");
	pDlg.show();

	QString previousTopLevelEntry("Zyxel");

	int pageHeight = painter.window().height() - 2*200;
	int x = 0;
	int y = 0;

	if(options().testFlag(QU::reportPrependCurrentPath)) {
		// TODO
	}

	painter.setFont(_letterFnt);
	int letterHeight = painter.fontMetrics().height();
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
				maxYearWidth = (currentEntryWidth > maxYearWidth) ? currentEntryWidth : maxYearWidth;
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
	_yearHSep				= qMin(_yearHSep, maxYearWidth);
	_creatorHSep			= qMin(_creatorHSep, maxCreatorWidth);
	_speedHSep				= qMin(_speedHSep, maxSpeedWidth);
	_lengthHSep				= qMin(_lengthHSep, maxLengthWidth);
	_songPathHSep			= qMin(_songPathHSep, maxPathWidth);
	_songFilePathHSep		= qMin(_songFilePathHSep, maxFilePathWidth);
	_relSongFilePathHSep	= qMin(_relSongFilePathHSep, maxRelFilePathWidth);

	bool letterBefore = false;
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

	bool secondColumn = false;

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

		if(reportDataList().at(0)->textData(song).at(0).toUpper() != previousTopLevelEntry.at(0).toUpper()) {
			// new letter
			if(secondColumn) {
				x = painter.window().width()/2;
			} else {
				x = 0;
			}

			// check if letter + topLevelEntry + first subLevelEntry will fit on page
			if((y + _subLevelToLetterVSep + _letterToTopLevelVSep + _topLevelToSubLevelVSep) > pageHeight) {
				y = letterHeight;

				if( secondColumn ) {
					printer.newPage();
					secondColumn = false;
					x = 0;
				} else {
					secondColumn = true;
					x = painter.window().width()/2;
				}

			} else {
				if(y == 0)
					y = letterHeight;
				else
					y += _subLevelToLetterVSep;
			}

			painter.setPen(Qt::darkGreen);
			painter.setFont(_letterFnt);
			painter.drawText(x, y, reportDataList().at(0)->textData(song).at(0).toUpper());

			letterBefore = true;
		}

		if(reportDataList().at(0)->textData(song) != previousTopLevelEntry) {
			// new topLevelEntry
			if(secondColumn) {
				x = painter.window().width()/2;
			} else {
				x = 0;
			}

			// check if topLevelEntry + first subLevelEntry will fit on page
			if(letterBefore) {
				letterBefore = false;
				y += _letterToTopLevelVSep;
			}
			else {
				if((y + _subLevelToTopLevelVSep + _topLevelToSubLevelVSep) > pageHeight) {
					y = topLevelEntryHeight;

					if( secondColumn ) {
						printer.newPage();
						secondColumn = false;
						x = 0;
					} else {
						secondColumn = true;
						x = painter.window().width()/2;
					}

				} else {
					y += _subLevelToTopLevelVSep;
				}
			}

			painter.setPen(Qt::black);
			painter.setFont(_topLevelEntryFnt);
			painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(0)->textData(song), Qt::ElideRight, painter.window().width()));

			previousTopLevelEntry = reportDataList().at(0)->textData(song);

			topLevelEntryBefore = true;
		}

		x += _subLevelEntryHIndent;

		// check if topLevelEntry + first subLevelEntry will fit on page
		if(topLevelEntryBefore) {
			topLevelEntryBefore = false;
			y += _topLevelToSubLevelVSep;
			}
		else {
			if((y + _subLevelToSubLevelVSep) > pageHeight) {
				y = subLevelEntryHeight;

				if( secondColumn ) {
					printer.newPage();
					secondColumn = false;
					x = _subLevelEntryHIndent;
				} else {
					secondColumn = true;
					x = painter.window().width()/2 + _subLevelEntryHIndent;
				}
			} else {
				y += _subLevelToSubLevelVSep;
			}
		}

		// paint sublevel entry
		if(reportDataList().size() > 1) {
			painter.setPen(Qt::black);
			painter.setFont(_subLevelEntryFnt);
			painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(1)->textData(song), Qt::ElideRight, _subLevelEntryHSep));
			x += _subLevelEntryHSep + _colHSep;
		}

		// paint all subsublevel entries
		for(int i = 2; i < reportDataList().size(); ++i) {
			painter.setFont(_subSubLevelEntryFnt);

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

			int currentColumn = reportDataList().at(i)->id();

			if(currentColumn == ARTIST_COL) {
				painter.setPen(Qt::black);
				painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(i)->textData(song), Qt::ElideRight, _artistHSep));
				x += _artistHSep + _colHSep;
			} else if(currentColumn == TITLE_COL) {
				painter.setPen(Qt::black);
				painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(i)->textData(song), Qt::ElideRight, _titleHSep));
				x += _titleHSep + _colHSep;
			} else if(currentColumn == LANGUAGE_COL) {
				painter.setPen(Qt::gray);
				painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(i)->textData(song), Qt::ElideRight, _languageHSep));
				x += _languageHSep + _colHSep;
			} else if(currentColumn == EDITION_COL) {
				painter.setPen(Qt::gray);
				painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(i)->textData(song), Qt::ElideRight, _editionHSep));
				x += _editionHSep + _colHSep;
			} else if(currentColumn == GENRE_COL) {
				painter.setPen(Qt::gray);
				painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(i)->textData(song), Qt::ElideRight, _genreHSep));
				x +=_genreHSep + _colHSep;
			} else if(currentColumn == YEAR_COL) {
				painter.setPen(Qt::gray);
				painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(i)->textData(song), Qt::ElideRight, _yearHSep));
				x += _yearHSep + _colHSep;
			} else if(currentColumn == CREATOR_COL) {
				painter.setPen(Qt::gray);
				painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(i)->textData(song), Qt::ElideRight, _creatorHSep));
				x += _creatorHSep + _colHSep;
			} else if(currentColumn == SPEED_COL) {
				painter.setPen(Qt::gray);
				painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(i)->textData(song), Qt::ElideRight, _speedHSep));
				x += _speedHSep + _colHSep;
			} else if(currentColumn == LENGTH_COL) {
				painter.setPen(Qt::gray);
				painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(i)->textData(song), Qt::ElideRight, _lengthHSep));
				x += _lengthHSep + _colHSep;
			} else if(currentColumn == SONG_PATH_COL) {
				painter.setPen(Qt::gray);
				painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(i)->textData(song), Qt::ElideLeft, _songPathHSep));
				x += _songPathHSep + _colHSep;
			} else if(currentColumn == SONG_FILE_PATH_COL) {
				painter.setPen(Qt::gray);
				painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(i)->textData(song), Qt::ElideLeft, _songFilePathHSep));
				x += _songFilePathHSep + _colHSep;
			} else if(currentColumn == REL_SONG_FILE_PATH_COL) {
				painter.setPen(Qt::gray);
				painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(i)->textData(song), Qt::ElideLeft, _relSongFilePathHSep));
				x += _relSongFilePathHSep + _colHSep;
			} else if(currentColumn == AUDIO_EXISTS_COL || currentColumn == COVER_EXISTS_COL || currentColumn == BACKGROUND_EXISTS_COL || currentColumn == VIDEO_EXISTS_COL || currentColumn == MEDLEY_EXISTS_COL || currentColumn == GOLDEN_NOTES_EXIST_COL) {

				if(reportDataList().at(i)->textData(song) == tr("yes")) {
					painter.setPen(Qt::darkGreen);
					painter.drawText(x, y, QString::fromUtf8(CHAR_UTF8_CHECK));
					painter.setPen(Qt::black);
				} else {
					painter.setPen(Qt::darkRed);
					painter.drawText(x, y, QString::fromUtf8(CHAR_UTF8_BALLOT));
					painter.setPen(Qt::black);
				}
				x += _booleanHSep + _colHSep;
			} else {
				painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(i)->textData(song), Qt::ElideRight, _defaultHSep));
				x += _defaultHSep + _colHSep;
			}
		}
		if(secondColumn) {
			x = painter.window().width()/2;
		} else {
			x = 0;
		}

	}

	painter.end();

	logSrv->add(tr("Adding PDF report...done."), QU::Information);
	return true;
}
