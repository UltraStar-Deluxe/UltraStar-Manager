#include "QUPdfReport.h"
#include "QUProgressDialog.h"
#include "QULogService.h"

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
	_beforeLetterVSep		= 200;
	_afterLetterVSep		= 150;
	_beforeTopLevelEntryVSep	= 50;
	_afterTopLevelEntryVSep		= 200;
	_afterSubLevelEntryVSep		= 200;

	// horizontal spacing
	_subLevelEntryHIndent	= 200;
	_colHSep		= 50;

	_artistHSep		= 3500;
	_titleHSep		= 3500;
	_languageHSep		= 700;
	_editionHSep		= 2000;
	_genreHSep		= 1000;
	_yearHSep		= 400;
	_creatorHSep		= 800;
	_audioExistsHSep	= 300;
	_coverExistsHSep	= 300;
	_backgroundExistsHSep	= 300;
	_videoExistsHSep	= 300;
	_speedHSep		= 400;
	_lengthHSep		= 400;
	_songPathHSep		= 400;
	_songFilePathHSep	= 400;
	_relSongFilePathHSep	= 400;
	_defaultHSep		= 400;

	_letterFnt		= QFont("Verdana", 14, QFont::Black, false);
	_topLevelEntryFnt	= QFont("Verdana", 7, QFont::DemiBold, false);
	_subLevelEntryFnt	= QFont("Verdana", 7, QFont::Normal, false);
	_subSubLevelEntryFnt	= QFont("Verdana", 6, QFont::Normal, false);
	_numberFnt		= QFont("Verdana", 6, QFont::Normal, true);
}

bool QUPdfReport::save() {
	QPrinter printer(QPrinter::HighResolution);
	printer.setOutputFormat(QPrinter::PdfFormat);
	printer.setOutputFileName(fileInfo().filePath());
	printer.setPaperSize(QPrinter::A4);
	printer.setPageMargins(10, 10, 10, 10, QPrinter::Millimeter);

	QPainter painter;
	painter.begin(&printer);

	QUProgressDialog pDlg(tr("Creating PDF report..."), songs().size());
	pDlg.setPixmap(":/types/folder.png");
	pDlg.show();

	//int rn = 1;
	//int max = QVariant(songs().size()).toString().length();

	QString previousTopLevelEntry("Zyxel");
	painter.save();

	int pageHeight = painter.window().height() - 2*200;
	int x = 0;
	int y = 0;

	painter.setFont(_letterFnt);
	int letterHeight = painter.fontMetrics().height();
	painter.setFont(_topLevelEntryFnt);
	int topLevelEntryHeight = painter.fontMetrics().height();
	painter.setFont(_subLevelEntryFnt);
	int subLevelEntryHeight = painter.fontMetrics().height();

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

		if(reportDataList().at(0)->textData(song) != previousTopLevelEntry) {
			x = 0;
			if(reportDataList().at(0)->textData(song).at(0).toUpper() != previousTopLevelEntry.at(0).toUpper()) {
				if(y + letterHeight + topLevelEntryHeight + subLevelEntryHeight > pageHeight) {
					printer.newPage();
					y = 0;
				} else {
					y += _beforeLetterVSep;
				}
				painter.setPen(Qt::darkGreen);
				painter.setFont(_letterFnt);
				painter.drawText(x, y, QString(reportDataList().at(0)->textData(song).at(0)));
				y += _afterLetterVSep;
			}

			if(y + topLevelEntryHeight + subLevelEntryHeight > pageHeight) {
				printer.newPage();
				y = 0;
			}
			y += _beforeTopLevelEntryVSep;
			painter.setPen(Qt::black);
			painter.setFont(_topLevelEntryFnt);
			painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(0)->textData(song), Qt::ElideRight, 20000));
			y += _afterTopLevelEntryVSep;
		}

		painter.save();

		previousTopLevelEntry = reportDataList().at(0)->textData(song);
		x += _subLevelEntryHIndent;

		if(y + subLevelEntryHeight > pageHeight) {
			printer.newPage();
			y = 0;
		}

		for(int i = 1; i < reportDataList().size(); i++) {
			if(i == 1) {
				painter.setFont(_subLevelEntryFnt);
			} else {
				painter.setFont(_subSubLevelEntryFnt);
			}
			QString currentColumn = reportDataList().at(i)->headerTextData();

			if(currentColumn == "Artist") {
				painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(i)->textData(song), Qt::ElideRight, _artistHSep));
				x += _artistHSep + _colHSep;
			} else if(currentColumn == "Title") {
				painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(i)->textData(song), Qt::ElideRight, _titleHSep));
				x += _titleHSep + _colHSep;
			} else if(currentColumn == "Language") {
				painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(i)->textData(song), Qt::ElideRight, _languageHSep));
				x += _languageHSep + _colHSep;
			} else if(currentColumn == "Edition") {
				painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(i)->textData(song), Qt::ElideRight, _editionHSep));
				x += _editionHSep + _colHSep;
			} else if(currentColumn == "Genre") {
				painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(i)->textData(song), Qt::ElideRight, _genreHSep));
				x +=_genreHSep + _colHSep;
			} else if(currentColumn == "Year") {
				painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(i)->textData(song), Qt::ElideRight, _yearHSep));
				x += _yearHSep + _colHSep;
			} else if(currentColumn == "Creator") {
				painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(i)->textData(song), Qt::ElideRight, _creatorHSep));
				x += _creatorHSep + _colHSep;
			} else if(currentColumn == "Audio file exists?") {
				painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(i)->textData(song), Qt::ElideRight, _audioExistsHSep));
				x += _audioExistsHSep + _colHSep;
			} else if(currentColumn == "Cover file exists?") {
				painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(i)->textData(song), Qt::ElideRight, _coverExistsHSep));
				x += _coverExistsHSep + _colHSep;
			} else if(currentColumn == "Background file exists?") {
				painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(i)->textData(song), Qt::ElideRight, _backgroundExistsHSep));
				x += _backgroundExistsHSep + _colHSep;
			} else if(currentColumn == "Video file exists?") {
				painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(i)->textData(song), Qt::ElideRight, _videoExistsHSep));
				x += _videoExistsHSep + _colHSep;
			} else if(currentColumn == "Speed") {
				painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(i)->textData(song), Qt::ElideRight, _speedHSep));
				x += _speedHSep + _colHSep;
			} else if(currentColumn == "Length") {
				painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(i)->textData(song), Qt::ElideRight, _lengthHSep));
				x += _lengthHSep + _colHSep;
			} else if(currentColumn == "Song Path") {
				painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(i)->textData(song), Qt::ElideRight, _songPathHSep));
				x += _songPathHSep + _colHSep;
			} else if(currentColumn == "Song File Path") {
				painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(i)->textData(song), Qt::ElideRight, _songFilePathHSep));
				x += _songFilePathHSep + _colHSep;
			} else if(currentColumn == "Relative Song File Path") {
				painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(i)->textData(song), Qt::ElideRight, _relSongFilePathHSep));
				x += _relSongFilePathHSep + _colHSep;
			} else {
				painter.drawText(x, y, painter.fontMetrics().elidedText(reportDataList().at(i)->textData(song), Qt::ElideRight, _defaultHSep));
				x += _defaultHSep + _colHSep;
			}
		}
		x = 0;
		y += _afterSubLevelEntryVSep;
	}

	painter.end();

	logSrv->add(tr("Adding PDF report...done."), QU::Information);
	return true;
}
