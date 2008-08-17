#include "QUReportDialog.h"
#include "QUHtmlReport.h"
#include "QUPlainTextReport.h"

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
		infoTextLbl->setText(tr("Select the columns you want to see in the report. Drag & drop them to change their order. Songs will be sorted alphabetically."));

	if(songTree->topLevelItemCount() == 0) {
		infoTextLbl->setText(tr("The report will be empty because no song is visible in the song tree."));
		infoIconLbl->setPixmap(QPixmap(":/marks/error.png"));
	}

	connect(doneBtn, SIGNAL(clicked()), this, SLOT(accept()));
	connect(createHtmlBtn, SIGNAL(clicked()), this, SLOT(createHtmlReport()));
	connect(createPlainTextBtn, SIGNAL(clicked()), this, SLOT(createPlainTextReport()));

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

	// custom tags
	foreach(QString customTag, QUSongFile::customTags()) {
		reportList->addItem(new QUReportItem(new QUSongTagData(customTag)));
	}
}

void QUReportDialog::createHtmlReport() {
	QSettings settings;
	QFileInfo fi(QDir(settings.value("reportPath").toString()), "report.html");

	fi.setFile(QFileDialog::getSaveFileName(this, tr("Save Report"),
		fi.filePath(),
		tr("Website (*.htm *.html)")));

	if(!fi.fileName().isEmpty()) {
		settings.setValue("reportPath", QVariant(fi.path())); // remember folder

		QList<QUAbstractReportData*> reportData;
		QList<QUSongFile*> songFiles;

		this->fetchDataAndSongs(reportData, songFiles);

		QUHtmlReport report(songFiles, reportData, fi);
		report.save();

		emit finished(QString(tr("Report created successfully to: \"%1\".")).arg(fi.filePath()), QU::information);

		QDesktopServices::openUrl(QUrl::fromLocalFile(fi.filePath()));
	} else {
		emit finished(tr("Report could not be created."), QU::warning);
	}
}

void QUReportDialog::createPlainTextReport() {
	QSettings settings;
	QFileInfo fi(QDir(settings.value("reportPath").toString()), "report.txt");

	fi.setFile(QFileDialog::getSaveFileName(this, tr("Save Report"),
		fi.filePath(),
		tr("Report (*.txt)")));

	if(!fi.fileName().isEmpty()) {
		settings.setValue("reportPath", QVariant(fi.path())); // remember folder

		QList<QUAbstractReportData*> reportData;
		QList<QUSongFile*> songFiles;

		this->fetchDataAndSongs(reportData, songFiles);

		QUPlainTextReport report(songFiles, reportData, fi);
		report.save();

		emit finished(QString(tr("Report created successfully to: \"%1\".")).arg(fi.filePath()), QU::information);

		QDesktopServices::openUrl(QUrl::fromLocalFile(fi.filePath()));
	} else {
		emit finished(tr("Report could not be created."), QU::warning);
	}
}

/*!
 * Looks for all checked report data columns and all visible songs in the song tree. Fills the results
 * in two lists.
 */
void QUReportDialog::fetchDataAndSongs(QList<QUAbstractReportData*> &data, QList<QUSongFile*> &songs) {
	for(int i = 0; i < reportList->count(); i++) {
		QUReportItem *item = dynamic_cast<QUReportItem*>(reportList->item(i));

		if(item && item->checkState() == Qt::Checked) {
			item->data()->setNext(0); // clear previous connections
			if(!data.isEmpty())
				data.last()->setNext(item->data());

			data.append(item->data());
		}
	}

	for(int i = 0; i < _songTree->topLevelItemCount(); i++) {
		QUSongItem *songItem = dynamic_cast<QUSongItem*>(_songTree->topLevelItem(i));

		if(songItem)
			songs.append(songItem->song());
	}

	if(!data.isEmpty())
		data.first()->sort(songs);
}
