#include "QUPathsDialog.h"

#include "playlist/QUPlaylistDatabase.h"

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

	connect(pathEditPlaylist, SIGNAL(textChanged(QString)), this, SLOT(checkPlaylistPath()));
	connect(songPathList, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(checkSongPath(QListWidgetItem*)));

	connect(browsePlaylistBtn, SIGNAL(clicked()), this, SLOT(choosePlaylistPath()));
	connect(browseSongBtn, SIGNAL(clicked()), this, SLOT(chooseSongPath()));

	connect(removeSongPathBtn, SIGNAL(clicked()), this, SLOT(removeSongPath()));

	QSettings s;
	pathEditPlaylist->setText(s.value("playlistFilePath", "").toString());

	songPathList->clear();
	if(s.contains("songPaths"))
		songPathList->addItems(s.value("songPaths", "").toStringList());
	for(int row = 0; row < songPathList->count(); ++row)
		songPathList->item(row)->setFlags(songPathList->item(row)->flags() | Qt::ItemIsEditable);

	updateHelpText();
}

void QUPathsDialog::accept() {
	QSettings s;

	s.setValue("playlistFilePath", pathEditPlaylist->text());
	playlistDB->setDir(pathEditPlaylist->text());

	QStringList songPaths;
	for(int row = 0; row < songPathList->count(); ++row)
		songPaths << songPathList->item(row)->text();
	if(!s.value("songPath", QString()).toString().isEmpty() && !songPaths.contains(QDir::toNativeSeparators(QU::BaseDir.path())))
		songPaths.append(QDir::toNativeSeparators(QU::BaseDir.path()));
	s.setValue("songPaths", songPaths);

	if(!songPaths.isEmpty() && (s.value("songPath").toString().isEmpty() || !songPaths.contains(s.value("songPath").toString())))
		s.setValue("songPath", songPaths.first());

	QDialog::accept();
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

void QUPathsDialog::checkSongPath(QListWidgetItem *item) {
	if(!item)
		return;

	disconnect(songPathList, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(checkSongPath(QListWidgetItem*)));

	QDir d(item->text());

	if(!d.exists()) {
		item->setIcon(QIcon(":/marks/path_error.png"));
		item->setToolTip(tr("Path does not exist."));
		item->setBackground(Qt::white);
	} else if(item->text().isEmpty()) {
		item->setIcon(QIcon(":/marks/path_error.png"));
		item->setToolTip(tr("Path is empty."));
		item->setBackground(Qt::white);
	} else {
		item->setIcon(QIcon(":/marks/path_ok.png"));
		if(QU::BaseDir == d) {
			item->setToolTip(tr("This song path is active. Songs were loaded."));
			item->setBackground(Qt::yellow);
		} else {
			item->setToolTip(tr(""));
			item->setBackground(Qt::white);
		}
	}

	connect(songPathList, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(checkSongPath(QListWidgetItem*)));
}

void QUPathsDialog::choosePlaylistPath() {
	QString path = QFileDialog::getExistingDirectory(this, tr("Select folder for playlists"), pathEditPlaylist->text());
	if(!path.isEmpty())
		pathEditPlaylist->setText(QDir::toNativeSeparators(path));
}

void QUPathsDialog::chooseSongPath() {
	QString path = QFileDialog::getExistingDirectory(this, tr("Choose your UltraStar song directory"), QU::BaseDir.absolutePath());

	if(path.isEmpty())
		return;

	QListWidgetItem *pathItem = new QListWidgetItem(QDir::toNativeSeparators(path));
	pathItem->setFlags(pathItem->flags() | Qt::ItemIsEditable);
	songPathList->addItem(pathItem);
	checkSongPath(pathItem);

	updateHelpText();
}

void QUPathsDialog::removeSongPath() {
	qDeleteAll(songPathList->selectedItems());
	updateHelpText();
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
		textLbl->setText(tr("<b>First time path setup:</b><br><br>Choose at least <b>one song folder</b>. If you set more than one folder for songs, the first one will be used for now. Optionally, choose a folder for <b>Playlists</b>."));
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
