#include "QUPathsDialog.h"

#include <QFileInfo>
#include <QPixmap>
#include <QSettings>
#include <QFileDialog>
#include <QListWidgetItem>

QUPathsDialog::QUPathsDialog(bool firstTimeSetup, QWidget *parent): QDialog(parent) {
	setupUi(this);

	_firstTimeSetup = firstTimeSetup;

	connect(acceptBtn, SIGNAL(clicked()), this, SLOT(accept()));
	connect(rejectBtn, SIGNAL(clicked()), this, SLOT(reject()));

	connect(pathEditUs, SIGNAL(textChanged(QString)), this, SLOT(checkUsPath()));
	connect(pathEditPlaylist, SIGNAL(textChanged(QString)), this, SLOT(checkPlaylistPath()));
	connect(pathEditCover, SIGNAL(textChanged(QString)), this, SLOT(checkCoverPath()));
	connect(pathEditLang, SIGNAL(textChanged(QString)), this, SLOT(checkLanguagePath()));
	connect(songPathList, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(checkSongPath(QListWidgetItem*)));

	connect(browseUsBtn, SIGNAL(clicked()), this, SLOT(chooseUsPath()));
	connect(browsePlaylistBtn, SIGNAL(clicked()), this, SLOT(choosePlaylistPath()));
	connect(browseCoverBtn, SIGNAL(clicked()), this, SLOT(chooseCoverPath()));
	connect(browseLangBtn, SIGNAL(clicked()), this, SLOT(chooseLanguagePath()));
	connect(browseSongBtn, SIGNAL(clicked()), this, SLOT(chooseSongPath()));

	connect(removeSongPathBtn, SIGNAL(clicked()), this, SLOT(removeSongPath()));
	connect(autoDetectBtn, SIGNAL(clicked()), this, SLOT(autoDetectPaths()));

	QSettings s;
	pathEditUs->setText(s.value("usPath", "").toString());
	pathEditPlaylist->setText(s.value("playlistFilePath", "").toString());
	pathEditCover->setText(s.value("usCoverPath", "").toString());
	pathEditLang->setText(s.value("usLangPath", "").toString());

	songPathList->clear();
	if(s.contains("songPaths"))
		songPathList->addItems(s.value("songPaths", "").toStringList());
	for(int row = 0; row < songPathList->count(); row++)
		songPathList->item(row)->setFlags(songPathList->item(row)->flags() | Qt::ItemIsEditable);

	updateHelpText();
}

void QUPathsDialog::accept() {
	QSettings s;
	s.setValue("usPath", pathEditUs->text());
	s.setValue("playlistFilePath", pathEditPlaylist->text());
	s.setValue("usCoverPath", pathEditCover->text());
	s.setValue("usLangPath", pathEditLang->text());

	QStringList songPaths;
	for(int row = 0; row < songPathList->count(); row++)
		songPaths << songPathList->item(row)->text();
	if(!s.value("songPath", QString()).toString().isEmpty() && !songPaths.contains(QU::BaseDir.path()))
		songPaths.append(QU::BaseDir.path());
	s.setValue("songPaths", songPaths);

	if(!songPaths.isEmpty() && s.value("songPath").toString().isEmpty())
		s.setValue("songPath", songPaths.first());

	QDialog::accept();
}

void QUPathsDialog::checkUsPath() {
	QDir d(pathEditUs->text());

	if(!d.exists()) {
		showError(chkLblUs, tr("Path does not exist."));
		autoDetectBtn->hide();
	} else if(pathEditUs->text().isEmpty()) {
		showError(chkLblUs, tr("Path is empty."));
		autoDetectBtn->hide();
	} else if(!hasConfigFile(d)) {
		showProblems(chkLblUs, tr("Configuration file \"config.ini\" not found."));
		autoDetectBtn->show();
	} else if(!isUltraStar(d) && !isUltraStarDeluxe(d)) {
		showProblems(chkLblUs, tr("Could not recognize UltraStar or UltraStar Deluxe."));
		autoDetectBtn->show();
	} else {
		showOk(chkLblUs);
		autoDetectBtn->show();
	}
}

void QUPathsDialog::checkPlaylistPath() {
	QDir d(pathEditPlaylist->text());

	if(!d.exists())
		showError(chkLblPlaylist, tr("Path does not exist."));
	else if(pathEditPlaylist->text().isEmpty())
		showError(chkLblPlaylist, tr("Path is empty."));
	else
		showOk(chkLblPlaylist);
}

void QUPathsDialog::checkCoverPath() {
	QDir d(pathEditCover->text());

	if(!d.exists())
		showError(chkLblCover, tr("Path does not exist."));
	else if(pathEditCover->text().isEmpty())
		showError(chkLblCover, tr("Path is empty."));
	else if(!hasCoversFile(d))
		showProblems(chkLblCover, tr("Could not find \"covers.ini\"."));
	else
		showOk(chkLblCover);
}

void QUPathsDialog::checkLanguagePath() {
	QDir d(pathEditLang->text());

	if(!d.exists())
		showError(chkLblLang, tr("Path does not exist."));
	else if(pathEditLang->text().isEmpty())
		showError(chkLblLang, tr("Path is empty."));
	else
		showOk(chkLblLang);
}

void QUPathsDialog::checkSongPath(QListWidgetItem *item) {
	if(!item)
		return;

	disconnect(songPathList, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(checkSongPath(QListWidgetItem*)));

	QDir d(item->text());

	if(!d.exists()) {
		item->setIcon(QIcon(":/marks/path_error.png"));
		item->setToolTip(tr("Path does not exist."));
		item->setBackgroundColor(Qt::white);
	} else if(item->text().isEmpty()) {
		item->setIcon(QIcon(":/marks/path_error.png"));
		item->setToolTip(tr("Path is empty."));
		item->setBackgroundColor(Qt::white);
	} else {
		item->setIcon(QIcon(":/marks/path_ok.png"));
		if(QU::BaseDir == d) {
			item->setToolTip(tr("This song path is active. Songs were loaded."));
			item->setBackgroundColor(Qt::yellow);
		} else {
			item->setToolTip(tr(""));
			item->setBackgroundColor(Qt::white);
		}
	}

	connect(songPathList, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(checkSongPath(QListWidgetItem*)));
}

void QUPathsDialog::chooseUsPath() {
	QString path = QFileDialog::getExistingDirectory(this, tr("Select folder of UltraStar (Deluxe)"), pathEditUs->text());
	if(!path.isEmpty())
		pathEditUs->setText(path);
}

void QUPathsDialog::choosePlaylistPath() {
	QString path = QFileDialog::getExistingDirectory(this, tr("Select folder for playlists"), pathEditPlaylist->text());
	if(!path.isEmpty())
		pathEditPlaylist->setText(path);
}

void QUPathsDialog::chooseCoverPath() {
	QString path = QFileDialog::getExistingDirectory(this, tr("Select folder for UltraStar cover pictures"), pathEditCover->text());
	if(!path.isEmpty())
		pathEditCover->setText(path);
}

void QUPathsDialog::chooseLanguagePath() {
	QString path = QFileDialog::getExistingDirectory(this, tr("Select folder of UltraStar language files"), pathEditLang->text());
	if(!path.isEmpty())
		pathEditLang->setText(path);
}

void QUPathsDialog::chooseSongPath() {
	QString path = QFileDialog::getExistingDirectory(this, tr("Choose your UltraStar song directory"), QU::BaseDir.absolutePath());

	if(path.isEmpty())
		return;

	QListWidgetItem *pathItem = new QListWidgetItem(path);
	pathItem->setFlags(pathItem->flags() | Qt::ItemIsEditable);
	songPathList->addItem(pathItem);
	checkSongPath(pathItem);

	updateHelpText();
}

void QUPathsDialog::removeSongPath() {
	qDeleteAll(songPathList->selectedItems());
	updateHelpText();
}

void QUPathsDialog::autoDetectPaths() {
	QDir d(pathEditUs->text());

	if(QFileInfo(d, "Playlists").exists())
		pathEditPlaylist->setText(QFileInfo(d, "Playlists").filePath());
	if(QFileInfo(d, "Covers").exists())
		pathEditCover->setText(QFileInfo(d, "Covers").filePath());
	if(QFileInfo(d, "Languages").exists())
		pathEditLang->setText(QFileInfo(d, "Languages").filePath());
	if(QFileInfo(d, "Songs").exists() && songPathList->findItems(QFileInfo(d, "Songs").filePath(), Qt::MatchFixedString).isEmpty()) {
		QListWidgetItem *pathItem = new QListWidgetItem(QFileInfo(d, "Songs").filePath());
		pathItem->setFlags(pathItem->flags() | Qt::ItemIsEditable);
		songPathList->addItem(pathItem);
		checkSongPath(pathItem);
		updateHelpText();
	}
}

bool QUPathsDialog::isUltraStar(const QDir &d) const {
	return QFileInfo(d, "UltraStar.exe").exists();
}

bool QUPathsDialog::isUltraStarDeluxe(const QDir &d) const {
	return QFileInfo(d, "USdx.exe").exists();
}

bool QUPathsDialog::hasConfigFile(const QDir &d) const {
	return QFileInfo(d, "config.ini").exists();
}

bool QUPathsDialog::hasCoversFile(const QDir &d) const {
	return QFileInfo(d, "covers.ini").exists();
}

void QUPathsDialog::showOk(QLabel *lbl) {
	lbl->setPixmap(QPixmap(":/marks/path_ok.png"));
	lbl->setToolTip("");
}

void QUPathsDialog::showError(QLabel *lbl, const QString &tooltip) {
	lbl->setPixmap(QPixmap(":/marks/path_error.png"));
	lbl->setToolTip(tooltip);
}

void QUPathsDialog::showProblems(QLabel *lbl, const QString &tooltip) {
	lbl->setPixmap(QPixmap(":/marks/path_unknown.png"));
	lbl->setToolTip(tooltip);
}

void QUPathsDialog::updateHelpText() {
	removeSongPathBtn->setEnabled(songPathList->count() > 0);

	if(_firstTimeSetup) {
		rejectBtn->hide();
		acceptBtn->setEnabled(songPathList->count() > 0);
		infoIconLbl->setPixmap(QPixmap(":/marks/information.png"));
		textLbl->setText(tr("<b>First time path setup:</b><br><br>Choose your UltraStar folder and click on the <b>magic wand</b>, which appears then, to auto-detect the other folders. You will need at least <b>one song folder</b>. If you set more than one folder for songs, the first one will be used for now."));
		return;
	}

	if(songPathList->count() == 0) {
		infoIconLbl->setPixmap(QPixmap(":/marks/error.png"));
		textLbl->setText(tr("Please choose at least <b>one song path</b>. Otherwise the main functionality of this program will not be available."));
	} else {
		infoIconLbl->setPixmap(QPixmap(":/marks/information.png"));
		textLbl->setText(tr("Set all paths so that a <b>green tick</b> appears in front of each one. This allows all program features to work properly."));
	}
}
