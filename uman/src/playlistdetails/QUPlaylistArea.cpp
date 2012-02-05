#include "QUPlaylistArea.h"

#include "QUProgressDialog.h"
#include "QUMessageBox.h"
#include "QULogService.h"
#include "QUSongDatabase.h"

#include <QFileDialog>
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <QList>
#include <QFile>
#include <QMessageBox>

#include "QUSongSupport.h"
#include "QUSongDatabase.h"
#include "QUPlaylistDatabase.h"

#include "QUPlaylistModel.h"
#include "QUPlaylistDBModel.h"

QUPlaylistArea::QUPlaylistArea(QWidget *parent): QWidget(parent) {
	setupUi(this);

	playlistCombo->view()->setTextElideMode(Qt::ElideRight);
	playlistCombo->setModel(new QUPlaylistDBModel(playlistCombo));

	connect(browseBtn, SIGNAL(clicked()), this, SLOT(browse()));

	connect(playlistDB, SIGNAL(databaseReloaded()), this, SLOT(reset()));

	connect(playlistDB, SIGNAL(currentPlaylistChanged(QUPlaylistFile*)), this, SLOT(updateCurrentPlaylistNameEdit()));
	connect(playlistDB, SIGNAL(currentPlaylistChanged(QUPlaylistFile*)), this, SLOT(updateComboboxIndex()));
	connect(playlistDB, SIGNAL(playlistChanged(QUPlaylistFile*)), this, SLOT(updateComboboxIndex()));

	connect(playlistCombo, SIGNAL(currentIndexChanged(int)), playlistDB, SLOT(setCurrentIndex(int)));
	connect(playlistEdit, SIGNAL(textEdited(const QString&)), this, SLOT(updateCurrentPlaylistName(const QString&)));

	connect(savePlaylistBtn, SIGNAL(clicked()), this, SLOT(saveCurrentPlaylist()));
	connect(savePlaylistAsBtn, SIGNAL(clicked()), this, SLOT(saveCurrentPlaylistAs()));

	connect(createPlaylistBtn, SIGNAL(clicked()), playlistDB, SLOT(createPlaylist()));

	connect(removePlaylistBtn, SIGNAL(clicked()), this, SLOT(removeCurrentPlaylist()));
}

/*!
 * Reset all UI elements according to the playlist database.
 */
void QUPlaylistArea::reset() {
	if(playlistDB->currentPlaylist())
		playlistCombo->setCurrentIndex(playlistDB->currentIndex());
	playlistEdit->clear();

	// Disable not needed controls.
	setAreaEnabled(playlistDB->size() > 0);

	// update window title of parent dock widget with current playlist path
	playlistPathLbl->setText(QString(tr("<font color=#808080>%1</font>")).arg(QDir::toNativeSeparators(playlistDB->dir().path())));
}

/*!
 * Create a new entry in the current playlist for the given song.
 */
void QUPlaylistArea::addSongToCurrentPlaylist(QUSongFile *song) {
	if(!song)
		return;

	if(!playlistDB->currentPlaylist()) {
		logSrv->add(QString(tr("Could NOT add song \"%1 - %2\" to playlist. Try to create a new playlist.")).arg(song->artist()).arg(song->title()), QU::Warning);
		return;
	}

	playlistDB->currentPlaylist()->addEntry(song);
}

void QUPlaylistArea::updateCurrentPlaylistName(const QString &newName) {
	if(!playlistDB->currentPlaylist())
		return;

	playlistDB->currentPlaylist()->setName(newName);
	updateComboboxIndex();
}

void QUPlaylistArea::updateCurrentPlaylistNameEdit() {
	if(!playlistDB->currentPlaylist())
		return;

	playlistEdit->setText(playlistDB->currentPlaylist()->name());
}

void QUPlaylistArea::updateComboboxIndex() {
	if(!playlistDB->currentPlaylist())
		return;

	disconnect(playlistCombo, SIGNAL(currentIndexChanged(int)), playlistDB, SLOT(setCurrentIndex(int)));

	playlistCombo->setCurrentIndex(playlistDB->currentIndex());

	connect(playlistCombo, SIGNAL(currentIndexChanged(int)), playlistDB, SLOT(setCurrentIndex(int)));
}

void QUPlaylistArea::saveCurrentPlaylist() {
	if(!playlistDB->currentPlaylist())
		return;

	playlistDB->currentPlaylist()->save();
}

void QUPlaylistArea::saveCurrentPlaylistAs() {
	if(!playlistDB->currentPlaylist())
		return;

	QString filePath = QFileDialog::getSaveFileName(this, tr("Save playlist as..."), playlistDB->dir().path(), QString(tr("UltraStar Playlists (%1)")).arg(QUSongSupport::allowedPlaylistFiles().join(" ")));

	if(!filePath.isEmpty()) {
		QFileInfo oldFi = playlistDB->currentPlaylist()->fileInfo();

//		if(oldFi == QFileInfo(filePath)) {
//			saveCurrentPlaylist();
//			return;
//		}

		// make current playlist to new playlist and save it
		playlistDB->currentPlaylist()->setFileInfo(QFileInfo(filePath));
		playlistDB->currentPlaylist()->save();
		this->saveCurrentPlaylist();

		// restore old playlist, not needed for new, not-existent playlists
		if(oldFi.exists())
			playlistDB->loadPlaylist(oldFi.filePath());

		updateComboboxIndex();
	}
}

/*!
 * Used to disable all controls that will not be needed when no playlists
 * are present.
 */
void QUPlaylistArea::setAreaEnabled(bool enabled) {
	playlist->setEnabled(enabled);
	playlistEdit->setEnabled(enabled);
	savePlaylistBtn->setEnabled(enabled);
	savePlaylistAsBtn->setEnabled(enabled);
	removePlaylistBtn->setEnabled(enabled);
	playlistCombo->setHidden(!enabled);
	comboLbl->setText(enabled ? tr("Active List:") : tr("No playlists found."));

	if(!enabled) {
//		playlist->clear();
		playlistEdit->clear();
	}
}

/*!
 * Select a new folder and look there for new playlists. Old ones are discarded.
 */
void QUPlaylistArea::browse() {
	if(playlistDB->hasUnsavedChanges()) {
		int result = QUMessageBox::information(this,
				tr("Change Playlist Folder"),
				tr("Playlists have been modified."),
				BTN << ":/control/save_all.png" << tr("Save all changed playlists.")
					<< ":/control/bin.png"	  << tr("Discard all changes.")
					<< ":/marks/cancel.png"	 << tr("Cancel this action."));
		if(result == 0)
			playlistDB->saveUnsavedChanges();
		else if(result == 2)
			return;
	}

	// --------------------------------------------

	QString newPath = QFileDialog::getExistingDirectory(this, tr("Select a folder for playlists"), playlistDB->dir().path());

	if(!newPath.isEmpty())
		playlistDB->setDir(newPath);
}

void QUPlaylistArea::removeCurrentPlaylist() {
	if(!playlistDB->currentPlaylist())
		return;

	int result = QUMessageBox::information(this,
			tr("Delete Playlist"),
			QString(tr("<b>\"%1 (%2)\"</b> will be deleted permanently. You cannot undo a delete operation."))
				.arg(playlistDB->currentPlaylist()->name())
				.arg(playlistDB->currentPlaylist()->fileInfo().fileName()),
			BTN << ":/control/bin.png"  << tr("Delete this playlist.")
				<< ":/marks/cancel.png" << tr("Cancel delete operation."));
	if(result == 1)
		return;

	// --------------------------------------------

	playlistDB->deletePlaylist(playlistDB->currentPlaylist());
}
