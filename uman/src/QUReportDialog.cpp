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
#include <QFileInfoList>

#include <QUrl>
#include <QDesktopServices>

QUReportDialog::QUReportDialog(const QList<QUSongFile*> &allSongs, const QList<QUSongFile*> &visibleSongs, const QList<QUPlaylistFile*> &allPlaylists, QWidget *parent):
		QDialog(parent),
		_allSongs(allSongs),
		_visibleSongs(visibleSongs),
		_allPlaylists(allPlaylists)
{
	setupUi(this);

	infoTextLbl->setText(tr("Select the <b>columns</b> you want to see in the report. Drag & drop them to change their order. Songs of the <b>source</b> will be sorted alphabetically column by column.<br><br>You can <b>append and link</b> lyrics in HTML reports."));
	radioAllSongs->setText(QString(tr("All Songs (%1)")).arg(allSongs.size()));
	radioVisibleSongs->setText(QString(tr("Visible Songs (%1)")).arg(visibleSongs.size()));

	initReportList();
	initStyleCombo();
	initPlaylistCombo();

	connect(doneBtn, SIGNAL(clicked()), this, SLOT(accept()));
	connect(createHtmlBtn, SIGNAL(clicked()), this, SLOT(createHtmlReport()));
	connect(createPlainTextBtn, SIGNAL(clicked()), this, SLOT(createPlainTextReport()));

	connect(radioPlaylist, SIGNAL(toggled(bool)), this, SLOT(togglePlaylistSource(bool)));
	togglePlaylistSource(false);

	connect(useStyleChk, SIGNAL(toggled(bool)), this, SLOT(toggleStyleCombo(bool)));
	useStyleChk->setChecked(true);

	connect(appendLyricsChk, SIGNAL(toggled(bool)), this, SLOT(toggleAppendLyrics(bool)));
	toggleAppendLyrics(false);
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

	reportList->addItem(new QUReportItem(new QUSongFileData("speed")));
	reportList->addItem(new QUReportItem(new QUSongFileData("lengthTotal")));
	reportList->addItem(new QUReportItem(new QUSongFileData("path")));
	reportList->addItem(new QUReportItem(new QUSongFileData("filePath")));
	reportList->addItem(new QUReportItem(new QUSongFileData("relativeFilePath")));

	// custom tags
	foreach(QString customTag, QUSongFile::customTags()) {
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
	for(int i = 0; i < _allPlaylists.size(); i++) {
		playlistCombo->addItem(QString("%1 (%2)").arg(_allPlaylists.at(i)->name()).arg(_allPlaylists.at(i)->count()), i);
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

		fetchData();

		QUHtmlReport report(
				sortedSongs(),
				_data,
				fi,
				this->selectedOptions(),
				this->currentPlaylistName(),
				styleCombo->itemData(styleCombo->currentIndex()).toString()); // css file path is saved in user data of current combobox item
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

		this->fetchData();

		QUPlainTextReport report(
				sortedSongs(),
				_data,
				fi,
				this->selectedOptions(),
				this->currentPlaylistName());
		report.save();

		emit finished(QString(tr("Report created successfully to: \"%1\".")).arg(fi.filePath()), QU::information);

		QDesktopServices::openUrl(QUrl::fromLocalFile(fi.filePath()));
	} else {
		emit finished(tr("Report could not be created."), QU::warning);
	}
}

/*!
 * Looks for all checked report data columns.
 */
void QUReportDialog::fetchData() {
	_data.clear();

	for(int i = 0; i < reportList->count(); i++) {
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

		if(index >= 0 and index < _allPlaylists.size())
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

	if(showBaseDirChk->isEnabled() and showBaseDirChk->isChecked())
		options |= QU::reportPrependCurrentPath;
	if(showPlaylistName->isEnabled() and showPlaylistName->isChecked())
		options |= QU::reportPrependUserData;
	if(appendLyricsChk->isEnabled() and appendLyricsChk->isChecked())
		options |= QU::reportAppendLyrics;
	if(linkLyricsChk->isEnabled() and linkLyricsChk->isChecked())
		options |= QU::reportLinkLyrics;
	if(useStyleChk->isEnabled() and useStyleChk->isChecked())
		options |= QU::reportUseStyleSheet;

	return options;
}

/*!
 * \returns The name of the selected playlist.
 */
QString QUReportDialog::currentPlaylistName() const {
	int index = playlistCombo->itemData(playlistCombo->currentIndex(), Qt::UserRole).toInt();

	if(index >= 0 and index < _allPlaylists.size())
		return _allPlaylists.at(index)->name();
	else
		return "";
}

/*!
 * Enables or disables the combobox for playlists according to your choice.
 */
void QUReportDialog::togglePlaylistSource(bool checked) {
	playlistCombo->setEnabled(checked);
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
