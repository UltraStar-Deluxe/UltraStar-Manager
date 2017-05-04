#include "QUReportDialog.h"

#include "QUHtmlReport.h"
#include "QUPlainTextReport.h"
#include "QUPdfReport.h"
#include "QUCSVReport.h"

#include "QULogService.h"

#include "QUSongSupport.h"

#include <QFileDialog>
#include <QSettings>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QPixmap>
#include <QFileInfoList>
#include <QDateTime>
#include <QFont>
#include <QFontDialog>
#include <QColorDialog>

#include <QUrl>
#include <QDesktopServices>
#include <QPrinter>

QUReportDialog::QUReportDialog(const QList<QUSongFile*> &allSongs, const QList<QUSongFile*> &visibleSongs, const QList<QUPlaylistFile*> &allPlaylists, QWidget *parent):
		QDialog(parent),
		_allSongs(allSongs),
		_visibleSongs(visibleSongs),
		_allPlaylists(allPlaylists)
{
	_letterFnt = QFont("Verdana", 14, QFont::Black, false);
	_topLevelEntryFnt = QFont("Verdana", 7, QFont::DemiBold, false);
	_subLevelEntryFnt = QFont("Verdana", 7, QFont::Normal, false);
	_subSubLevelEntryFnt = QFont("Verdana", 6, QFont::Normal, false);
	_letterClr = Qt::darkGreen;
	_topLevelEntryClr = Qt::black;
	_subLevelEntryClr = Qt::black;
	_subSubLevelEntryClr = Qt::gray;

	setupUi(this);

	infoTextLbl->setText(tr("Select the <b>columns</b> you want to see in the report. Drag & drop them to change their order. Songs of the <b>source</b> will be sorted alphabetically column by column.<br><br>You can <b>append and link</b> lyrics and use style sheets in HTML reports."));
	radioAllSongs->setText(QString(tr("All Songs (%1)")).arg(allSongs.size()));
	radioVisibleSongs->setText(QString(tr("Visible Songs (%1)")).arg(visibleSongs.size()));

	initReportList();
	initStyleCombo();
	initPlaylistCombo();
	initPaperSizeCombo();
	initOrientationCombo();
	initSelectFntBtns();
	initSelectClrBtns();

	connect(selectLetterFntBtn, SIGNAL(clicked()), this, SLOT(selectLetterFnt()));
	connect(selectTopLevelEntryFntBtn, SIGNAL(clicked()), this, SLOT(selectTopLevelEntryFnt()));
	connect(selectSubLevelEntryFntBtn, SIGNAL(clicked()), this, SLOT(selectSubLevelEntryFnt()));
	connect(selectSubSubLevelEntryFntBtn, SIGNAL(clicked()), this, SLOT(selectSubSubLevelEntryFnt()));
	connect(selectLetterClrBtn, SIGNAL(clicked()), this, SLOT(selectLetterClr()));
	connect(selectTopLevelEntryClrBtn, SIGNAL(clicked()), this, SLOT(selectTopLevelEntryClr()));
	connect(selectSubLevelEntryClrBtn, SIGNAL(clicked()), this, SLOT(selectSubLevelEntryClr()));
	connect(selectSubSubLevelEntryClrBtn, SIGNAL(clicked()), this, SLOT(selectSubSubLevelEntryClr()));
	connect(doneBtn, SIGNAL(clicked()), this, SLOT(accept()));
	connect(createPdfBtn, SIGNAL(clicked()), this, SLOT(createPdfReport()));
	connect(createHtmlBtn, SIGNAL(clicked()), this, SLOT(createHtmlReport()));
	connect(createPlainTextBtn, SIGNAL(clicked()), this, SLOT(createPlainTextReport()));
	connect(createCSVBtn, SIGNAL(clicked()), this, SLOT(createCSVReport()));

	connect(radioPlaylist, SIGNAL(toggled(bool)), this, SLOT(togglePlaylistSource(bool)));
	togglePlaylistSource(false);

	connect(useStyleChk, SIGNAL(toggled(bool)), this, SLOT(toggleStyleCombo(bool)));
	useStyleChk->setChecked(true);

	connect(appendLyricsChk, SIGNAL(toggled(bool)), this, SLOT(toggleAppendLyrics(bool)));
	toggleAppendLyrics(false);

	// -----
	loadState();
}

QUReportDialog::~QUReportDialog() {
	saveState();
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

	reportList->addItem(new QUReportItem(new QUBooleanSongData(MEDLEY_TAGS)));
	reportList->addItem(new QUReportItem(new QUBooleanSongData(GOLDEN_NOTES)));
	reportList->addItem(new QUReportItem(new QUBooleanSongData(RAP_NOTES)));

	reportList->addItem(new QUReportItem(new QUSongFileData("speed")));
	reportList->addItem(new QUReportItem(new QUSongFileData("lengthTotal")));
	reportList->addItem(new QUReportItem(new QUSongFileData("path")));
	reportList->addItem(new QUReportItem(new QUSongFileData("filePath")));
	reportList->addItem(new QUReportItem(new QUSongFileData("relativeFilePath")));

	// custom tags
	foreach(QString customTag, QUSongSupport::availableCustomTags()) {
		reportList->addItem(new QUReportItem(new QUSongTagData(customTag)));
	}
}

void QUReportDialog::initStyleCombo() {
	// look for available style sheets
	QDir cssDir = QCoreApplication::applicationDirPath();
	// TODO: Make css path available through registry - not hard-coded.
	cssDir.cd("styles");

	QFileInfoList cssFiList = cssDir.entryInfoList(QStringList("*.css"), QDir::Files, QDir::Name);

	foreach(QFileInfo cssFi, cssFiList) {
		styleCombo->addItem(cssFi.fileName(), cssFi.filePath());
	}
}

void QUReportDialog::initPlaylistCombo() {
	for(int i = 0; i < _allPlaylists.size(); ++i) {
		playlistCombo->addItem(QString("%1 (%2)").arg(_allPlaylists.at(i)->name()).arg(_allPlaylists.at(i)->count()), i);
	}

	radioPlaylist->setEnabled(!_allPlaylists.isEmpty());
	playlistCombo->setEnabled(!_allPlaylists.isEmpty());
}

void QUReportDialog::initPaperSizeCombo() {
	paperSizeCombo->addItem("A4", QPrinter::A4);
	paperSizeCombo->addItem("A5", QPrinter::A5);
	paperSizeCombo->addItem("Letter", QPrinter::Letter);
}

void QUReportDialog::initOrientationCombo() {
	orientationCombo->addItem("Portrait", QPrinter::Portrait);
	orientationCombo->addItem("Landscape", QPrinter::Landscape);
}

void QUReportDialog::initSelectFntBtns() {
	selectLetterFntBtn->setFont(_letterFnt);
	selectLetterFntBtn->setText(QString("%1, %2").arg(_letterFnt.family()).arg(_letterFnt.pointSize()));
	selectTopLevelEntryFntBtn->setFont(_topLevelEntryFnt);
	selectTopLevelEntryFntBtn->setText(QString("%1, %2").arg(_topLevelEntryFnt.family()).arg(_topLevelEntryFnt.pointSize()));
	selectSubLevelEntryFntBtn->setFont(_subLevelEntryFnt);
	selectSubLevelEntryFntBtn->setText(QString("%1, %2").arg(_subLevelEntryFnt.family()).arg(_subLevelEntryFnt.pointSize()));
	selectSubSubLevelEntryFntBtn->setFont(_subSubLevelEntryFnt);
	selectSubSubLevelEntryFntBtn->setText(QString("%1, %2").arg(_subSubLevelEntryFnt.family()).arg(_subSubLevelEntryFnt.pointSize()));
}

void QUReportDialog::initSelectClrBtns() {
	selectLetterClrBtn->setStyleSheet(QString("background-color: %1").arg(_letterClr.name()));
	selectLetterClrBtn->setAutoFillBackground(true);
	selectLetterClrBtn->setFlat(true);
	selectTopLevelEntryClrBtn->setStyleSheet(QString("background-color: %1").arg(_topLevelEntryClr.name()));
	selectTopLevelEntryClrBtn->setAutoFillBackground(true);
	selectTopLevelEntryClrBtn->setFlat(true);
	selectSubLevelEntryClrBtn->setStyleSheet(QString("background-color: %1").arg(_subLevelEntryClr.name()));
	selectSubLevelEntryClrBtn->setAutoFillBackground(true);
	selectSubLevelEntryClrBtn->setFlat(true);
	selectSubSubLevelEntryClrBtn->setStyleSheet(QString("background-color: %1").arg(_subSubLevelEntryClr.name()));
	selectSubSubLevelEntryClrBtn->setAutoFillBackground(true);
	selectSubSubLevelEntryClrBtn->setFlat(true);
	this->repaint();
}

void QUReportDialog::selectLetterFnt() {
	bool ok;
	_letterFnt = QFontDialog::getFont(&ok, _letterFnt);
	selectLetterFntBtn->setFont(_letterFnt);
	selectLetterFntBtn->setText(QString("%1, %2").arg(_letterFnt.family()).arg(_letterFnt.pointSize()));
}

void QUReportDialog::selectTopLevelEntryFnt() {
	bool ok;
	_topLevelEntryFnt = QFontDialog::getFont(&ok, _topLevelEntryFnt);
	selectTopLevelEntryFntBtn->setFont(_topLevelEntryFnt);
	selectTopLevelEntryFntBtn->setText(QString("%1, %2").arg(_topLevelEntryFnt.family()).arg(_topLevelEntryFnt.pointSize()));
}

void QUReportDialog::selectSubLevelEntryFnt() {
	bool ok;
	_subLevelEntryFnt = QFontDialog::getFont(&ok, _subLevelEntryFnt);
	selectSubLevelEntryFntBtn->setFont(_subLevelEntryFnt);
	selectSubLevelEntryFntBtn->setText(QString("%1, %2").arg(_subLevelEntryFnt.family()).arg(_subLevelEntryFnt.pointSize()));
}

void QUReportDialog::selectSubSubLevelEntryFnt() {
	bool ok;
	_subSubLevelEntryFnt = QFontDialog::getFont(&ok, _subSubLevelEntryFnt);
	selectSubSubLevelEntryFntBtn->setFont(_subSubLevelEntryFnt);
	selectSubSubLevelEntryFntBtn->setText(QString("%1, %2").arg(_subSubLevelEntryFnt.family()).arg(_subSubLevelEntryFnt.pointSize()));
}

void QUReportDialog::selectLetterClr() {
	_letterClr = QColorDialog::getColor(_letterClr);
	selectLetterClrBtn->setStyleSheet(QString("background-color: %1").arg(_letterClr.name()));
	selectLetterClrBtn->setAutoFillBackground(true);
	selectLetterClrBtn->setFlat(true);
}

void QUReportDialog::selectTopLevelEntryClr() {
	_topLevelEntryClr = QColorDialog::getColor(_topLevelEntryClr);
	selectTopLevelEntryClrBtn->setStyleSheet(QString("background-color: %1").arg(_topLevelEntryClr.name()));
	selectTopLevelEntryClrBtn->setAutoFillBackground(true);
	selectTopLevelEntryClrBtn->setFlat(true);
}

void QUReportDialog::selectSubLevelEntryClr() {
	_subLevelEntryClr = QColorDialog::getColor(_subLevelEntryClr);
	selectSubLevelEntryClrBtn->setStyleSheet(QString("background-color: %1").arg(_subLevelEntryClr.name()));
	selectSubLevelEntryClrBtn->setAutoFillBackground(true);
	selectSubLevelEntryClrBtn->setFlat(true);
}

void QUReportDialog::selectSubSubLevelEntryClr() {
	_subSubLevelEntryClr = QColorDialog::getColor(_subSubLevelEntryClr);
	selectSubSubLevelEntryClrBtn->setStyleSheet(QString("background-color: %1").arg(_subSubLevelEntryClr.name()));
	selectSubSubLevelEntryClrBtn->setAutoFillBackground(true);
	selectSubSubLevelEntryClrBtn->setFlat(true);
}

void QUReportDialog::createPdfReport() {
	QSettings settings;
	QString reportFileName = tr("Songlist_%1.pdf").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd"));
	QFileInfo fi(QDir(settings.value("reportPath").toString()), reportFileName);

	fi.setFile(QFileDialog::getSaveFileName(this, tr("Save PDF Report"),
		fi.filePath(),
		tr("PDF files (*.pdf)")));

	if(!fi.fileName().isEmpty()) {
		settings.setValue("reportPath", QVariant(fi.path())); // remember folder

		fetchData();

		QUPdfReport report(
				sortedSongs(),
				_data,
				fi,
				this->selectedOptions(),
				this->currentPlaylistName(),
				static_cast<QPrinter::PaperSize>(paperSizeCombo->itemData(paperSizeCombo->currentIndex()).toInt()),
				static_cast<QPrinter::Orientation>(orientationCombo->itemData(orientationCombo->currentIndex()).toInt()),
				layoutColumnsCombo->currentText().toInt(),
				leftMargin->value(),
				rightMargin->value(),
				topMargin->value(),
				bottomMargin->value(),
				_letterFnt,
				_topLevelEntryFnt,
				_subLevelEntryFnt,
				_subSubLevelEntryFnt,
				_letterClr,
				_topLevelEntryClr,
				_subLevelEntryClr,
				_subSubLevelEntryClr);

		if(report.save()) {
			logSrv->add(QString(tr("PDF report created successfully to: \"%1\".")).arg(fi.filePath()), QU::Information);
			QDesktopServices::openUrl(QUrl::fromLocalFile(fi.filePath()));
		} else {
			logSrv->add(tr("PDF report could not be created."), QU::Error);
		}
	}
}

void QUReportDialog::createHtmlReport() {
	QSettings settings;
	QString reportFileName = tr("Songlist_%1.html").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd"));
	QFileInfo fi(QDir(settings.value("reportPath").toString()), reportFileName);

	fi.setFile(QFileDialog::getSaveFileName(this, tr("Save HTML Report"),
		fi.filePath(),
		tr("Website (*.htm *.html)")));

	if(!fi.fileName().isEmpty()) {
		settings.setValue("reportPath", QVariant(fi.path())); // remember folder

		fetchData();

		QUHtmlReport report(
				sortedSongs(),
				_data,
				fi,
				this->selectedOptions(),
				this->currentPlaylistName(),
				styleCombo->itemData(styleCombo->currentIndex()).toString()); // css file path is saved in user data of current combobox item

		if(report.save()) {
			logSrv->add(QString(tr("HTML report created successfully to: \"%1\".")).arg(fi.filePath()), QU::Information);
			QDesktopServices::openUrl(QUrl::fromLocalFile(fi.filePath()));
		} else {
			logSrv->add(tr("HTML report could not be created."), QU::Error);
		}
	}
}

void QUReportDialog::createPlainTextReport() {
	QSettings settings;
	QString reportFileName = tr("Songlist_%1.txt").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd"));
	QFileInfo fi(QDir(settings.value("reportPath").toString()), reportFileName);

	fi.setFile(QFileDialog::getSaveFileName(this, tr("Save Text Report"),
		fi.filePath(),
		tr("Report (*.txt)")));

	if(!fi.fileName().isEmpty()) {
		settings.setValue("reportPath", QVariant(fi.path())); // remember folder

		this->fetchData();

		QUPlainTextReport report(
				sortedSongs(),
				_data,
				fi,
				this->selectedOptions(),
				this->currentPlaylistName());

		if(report.save()) {
			logSrv->add(QString(tr("Text report created successfully to: \"%1\".")).arg(fi.filePath()), QU::Information);
			QDesktopServices::openUrl(QUrl::fromLocalFile(fi.filePath()));
		} else {
			logSrv->add(tr("Text report could not be created."), QU::Error);
		}
	}
}

void QUReportDialog::createCSVReport() {
	QSettings settings;
	QString reportFileName = tr("Songlist_%1.csv").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd"));
	QFileInfo fi(QDir(settings.value("reportPath").toString()), reportFileName);

	fi.setFile(QFileDialog::getSaveFileName(this, tr("Save CSV Report"),
		fi.filePath(),
		tr("CSV files (*.csv)")));

	if(!fi.fileName().isEmpty()) {
		settings.setValue("reportPath", QVariant(fi.path())); // remember folder

		this->fetchData();

		QUCSVReport report(
				sortedSongs(),
				_data,
				fi,
				this->selectedOptions(),
				this->currentPlaylistName());

		if(report.save()) {
			logSrv->add(QString(tr("CSV report created successfully to: \"%1\".")).arg(fi.filePath()), QU::Information);
			QDesktopServices::openUrl(QUrl::fromLocalFile(fi.filePath()));
		} else {
			logSrv->add(tr("CSV report could not be created."), QU::Error);
		}
	}
}

/*!
 * Looks for all checked report data columns.
 */
void QUReportDialog::fetchData() {
	_data.clear();

	for(int i = 0; i < reportList->count(); ++i) {
		QUReportItem *item = dynamic_cast<QUReportItem*>(reportList->item(i));

		if(item && item->checkState() == Qt::Checked) {
			item->data()->setNext(0); // clear previous connections
			if(!_data.isEmpty())
				_data.last()->setNext(item->data());

			_data.append(item->data());
		}
	}
}

/*!
 * Sort the given songs according to the fetched song data. You need to call fetchData() first.
 *
 * \sa fetchData()
 */
QList<QUSongFile*> QUReportDialog::sortedSongs() {
	QList<QUSongFile*> songs;

	if(radioAllSongs->isChecked()) {
		songs = _allSongs;
	} else if(radioVisibleSongs->isChecked()) {
		songs = _visibleSongs;
	} else if(radioPlaylist->isChecked()) {
		int index = playlistCombo->itemData(playlistCombo->currentIndex(), Qt::UserRole).toInt();

		if(index >= 0 && index < _allPlaylists.size())
			songs << _allPlaylists.at(index)->connectedSongs();
	}

	if(!_data.isEmpty())
		_data.first()->sort(songs);

	return songs;
}

/*!
 * \returns The selected options according to the checkboxes in the dialog.
 */
QU::ReportOptions QUReportDialog::selectedOptions() const {
	QU::ReportOptions options;

	if(showBaseDirChk->isEnabled() && showBaseDirChk->isChecked())
		options |= QU::reportPrependCurrentPath;
	if(showPlaylistName->isEnabled() && showPlaylistName->isChecked())
		options |= QU::reportPrependUserData;
	if(appendLyricsChk->isEnabled() && appendLyricsChk->isChecked())
		options |= QU::reportAppendLyrics;
	if(linkLyricsChk->isEnabled() && linkLyricsChk->isChecked())
		options |= QU::reportLinkLyrics;
	if(useStyleChk->isEnabled() && useStyleChk->isChecked())
		options |= QU::reportUseStyleSheet;

	return options;
}

/*!
 * \returns The name of the selected playlist.
 */
QString QUReportDialog::currentPlaylistName() const {
	int index = playlistCombo->itemData(playlistCombo->currentIndex(), Qt::UserRole).toInt();

	if(index >= 0 && index < _allPlaylists.size())
		return _allPlaylists.at(index)->name();
	else
		return "";
}

/*!
 * Enables or disables the combobox for playlists according to your choice.
 */
void QUReportDialog::togglePlaylistSource(bool checked) {
	playlistCombo->setEnabled(checked & !_allPlaylists.isEmpty());
	showPlaylistName->setEnabled(checked);
}

/*!
 * Enables or disables the combobox for style sheets according to your choice.
 */
void QUReportDialog::toggleStyleCombo(bool checked) {
	styleCombo->setEnabled(checked);
}

/*!
 * Enables or disables the checkbox for linking lyrics according to your choice.
 */
void QUReportDialog::toggleAppendLyrics(bool checked) {
	linkLyricsChk->setEnabled(checked);
}

/*!
 * Saves the current dialog state (checked elements a.s.o.) to registry.
 */
void QUReportDialog::saveState() {
	int reportColumns = 0;
	for(int row = 0; row < reportList->count(); ++row) {
		if(reportList->item(row)->checkState() == Qt::Checked)
			reportColumns |= 1 << row;
	}

	int otherOptions = 0;

	// current source
	if(radioAllSongs->isChecked())	 otherOptions |= 1 << 0;
	if(radioVisibleSongs->isChecked()) otherOptions |= 1 << 1;
	if(radioPlaylist->isChecked())	 otherOptions |= 1 << 2;

	// additional options
	if(showBaseDirChk->isChecked())   otherOptions |= 1 << 3;
	if(showPlaylistName->isChecked()) otherOptions |= 1 << 4;
	if(appendLyricsChk->isChecked())  otherOptions |= 1 << 5;
	if(linkLyricsChk->isChecked())	otherOptions |= 1 << 6;
	if(useStyleChk->isChecked())	  otherOptions |= 1 << 7;

	// current combo box items
	int playlistComboIndex = playlistCombo->currentIndex();
	int styleComboIndex	= styleCombo->currentIndex();

	// store values into registry
	QSettings settings;
	settings.setValue("reportColumns", reportColumns);
	settings.setValue("reportOtherOptions", otherOptions);
	settings.setValue("reportPlaylistComboIndex", playlistComboIndex);
	settings.setValue("reportStyleComboIndex", styleComboIndex);
}

/*!
 * Loads a dialog state (checked elements a.s.o.) from registry.
 */
void QUReportDialog::loadState() {
	QSettings settings;
	int reportColumns = settings.value("reportColumns", 3).toInt();

	for(int row = 0; row < reportList->count(); ++row) {
		if(reportColumns & (1 << row))
			reportList->item(row)->setCheckState(Qt::Checked);
		else
			reportList->item(row)->setCheckState(Qt::Unchecked);
	}

	int otherOptions = settings.value("reportOtherOptions", 1).toInt();;

	// current source
		radioAllSongs->setChecked(otherOptions & (1 << 0));
	radioVisibleSongs->setChecked(otherOptions & (1 << 1));
		radioPlaylist->setChecked(otherOptions & (1 << 2));

	// additional options
	  showBaseDirChk->setChecked(otherOptions & (1 << 3));
	showPlaylistName->setChecked(otherOptions & (1 << 4));
	 appendLyricsChk->setChecked(otherOptions & (1 << 5));
	   linkLyricsChk->setChecked(otherOptions & (1 << 6));
		 useStyleChk->setChecked(otherOptions & (1 << 7));

	// current combo box items
	int playlistComboIndex = settings.value("reportPlaylistComboIndex", 0).toInt();
	int styleComboIndex	= settings.value("reportStyleComboIndex", 0).toInt();
	playlistCombo->setCurrentIndex(playlistComboIndex >= playlistCombo->count() ? 0 : playlistComboIndex);
	styleCombo->setCurrentIndex(styleComboIndex >= styleCombo->count() ? 0 : styleComboIndex);
}
