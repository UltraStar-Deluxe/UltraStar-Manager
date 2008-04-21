#include "QUReportDialog.h"
#include "QUHtmlReport.h"

#include <QFileDialog>
#include <QSettings>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QPixmap>

#include <QUrl>
#include <QDesktopServices>

QUReportDialog::QUReportDialog(QUSongTree *songTree, QWidget *parent): QDialog(parent), _songTree(songTree) {
	setupUi(this);
	
	if(songTree->hasHiddenItems())
		infoTextLbl->setText(tr("You applied a filter to your songs. The report will only be created for the songs that are visible in the song tree."));
	else
		infoTextLbl->setText(tr("Select the columns you want to see in the report. Drag & drop them to change their order. Songs will be sorted after the first column."));

	if(songTree->topLevelItemCount() == 0) {
		infoTextLbl->setText(tr("The report will be empty because no song is visible in the song tree."));
		infoIconLbl->setPixmap(QPixmap(":/marks/error.png"));
	}
	
	connect(doneBtn, SIGNAL(clicked()), this, SLOT(accept()));
	connect(createHtmlBtn, SIGNAL(clicked()), this, SLOT(createHtmlReport()));
	
	initReportList();
}

void QUReportDialog::initReportList() {
	reportList->addItem(new QUReportItem(new QUSongTagData(ARTIST_TAG), Qt::Checked));
	reportList->addItem(new QUReportItem(new QUSongTagData(TITLE_TAG), Qt::Checked));
	reportList->addItem(new QUReportItem(new QUSongTagData(LANGUAGE_TAG)));
	reportList->addItem(new QUReportItem(new QUSongTagData(EDITION_TAG)));
	reportList->addItem(new QUReportItem(new QUSongTagData(GENRE_TAG)));
	reportList->addItem(new QUReportItem(new QUSongTagData(YEAR_TAG)));
	reportList->addItem(new QUReportItem(new QUSongTagData(CREATOR_TAG)));
	
	reportList->addItem(new QUReportItem(new QUBooleanSongData(MP3_TAG)));
	reportList->addItem(new QUReportItem(new QUBooleanSongData(COVER_TAG)));
	reportList->addItem(new QUReportItem(new QUBooleanSongData(BACKGROUND_TAG)));
	reportList->addItem(new QUReportItem(new QUBooleanSongData(VIDEO_TAG)));
}

void QUReportDialog::createHtmlReport() {
	QSettings settings;
	QFileInfo fi(QDir(settings.value("reportPath").toString()), "report.html");

	fi.setFile(QFileDialog::getSaveFileName(this, tr("Save Report"),
		fi.filePath(),
		tr("Website (*.htm *.html)")));
	
	if(!fi.fileName().isEmpty()) {
		settings.setValue("reportPath", QVariant(fi.path())); // remember folder
		
		// prepare elements for report

		QList<QUAbstractReportData*> reportData;
		QList<QUSongFile*> songFiles;

		for(int i = 0; i < reportList->count(); i++) {
			QUReportItem *item = dynamic_cast<QUReportItem*>(reportList->item(i));

			if(item && item->checkState() == Qt::Checked)
				reportData.append(item->data());
		}

		for(int i = 0; i < _songTree->topLevelItemCount(); i++) {
			QUSongItem *songItem = dynamic_cast<QUSongItem*>(_songTree->topLevelItem(i));

			if(songItem)
				songFiles.append(songItem->song());
		}
		
		if(!reportData.isEmpty())
			reportData.first()->sort(songFiles);

		QUHtmlReport report(songFiles, reportData, fi);
		report.save();

		emit finished(QString(tr("Report created successfully to: \"%1\".")).arg(fi.filePath()), QU::information);

		QDesktopServices::openUrl(QUrl::fromLocalFile(fi.filePath()));
	} else {
		emit finished(tr("Report could not be created."), QU::warning);
	}
}
