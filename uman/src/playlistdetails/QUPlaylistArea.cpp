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

//	connect(playlist, SIGNAL(removePlaylistEntryRequested(QUPlaylistEntry*)), this, SLOT(removeCurrentPlaylistEntry(QUPlaylistEntry*)));
//	connect(playlist, SIGNAL(orderChanged(QList<QUPlaylistEntry*>)), this, SLOT(changeCurrentPlaylistOrder(QList<QUPlaylistEntry*>)));
//	connect(playlist, SIGNAL(removeUnknownEntriesRequested()), this, SLOT(removeUnknownEntries()));

	connect(playlistCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(setCurrentPlaylist(int)));
	connect(playlistEdit, SIGNAL(textEdited(const QString&)), this, SLOT(updateCurrentPlaylistName(const QString&)));
//	connect(savePlaylistBtn, SIGNAL(clicked()), this, SLOT(saveCurrentPlaylist()));
//	connect(savePlaylistAsBtn, SIGNAL(clicked()), this, SLOT(saveCurrentPlaylistAs()));
//	connect(createPlaylistBtn, SIGNAL(clicked()), this, SLOT(addPlaylist()));
	connect(browseBtn, SIGNAL(clicked()), this, SLOT(browse()));
//	connect(removePlaylistBtn, SIGNAL(clicked()), this, SLOT(removeCurrentPlaylist()));

//	connect(playlistDB, SIGNAL(databaseDisconnected()), this, SLOT(update())); // really needed ?! may be slow...
//	connect(playlistDB, SIGNAL(databaseConnected()), this, SLOT(update()));
//	connect(playlistDB, SIGNAL(databaseCleared()), this, SLOT(reset()));
	connect(playlistDB, SIGNAL(databaseReloaded()), this, SLOT(reset()));
	connect(playlistDB, SIGNAL(playlistAdded(QUPlaylistFile*)), this, SLOT(integratePlaylist(QUPlaylistFile*)));

//	connect(songDB, SIGNAL(songChanged(QUSongFile*)), this, SLOT(update()));

	playlistCombo->view()->setTextElideMode(Qt::ElideRight);
	playlistCombo->setModel(new QUPlaylistDBModel(playlistCombo));
}

QUPlaylistFile* QUPlaylistArea::currentPlaylist() const {
	if(playlistDB->playlists().isEmpty())
		return 0;

	return playlistDB->playlists().at(this->currentPlaylistIndex());
}

/*!
 * Reset all UI elements according to the playlist database.
 */
void QUPlaylistArea::reset() {
	playlistCombo->setCurrentIndex(0);
	playlistEdit->clear();

	// Disable not needed controls.
	setAreaEnabled(false);

	// update window title of parent dock widget with current playlist path
	playlistPathLbl->setText(QString(tr("<font color=#808080>%1</font>")).arg(playlistDB->dir().path()));
}

/*!
 * Update the current playlist view and the combobox items.
 */
void QUPlaylistArea::update() {
	updatePlaylistCombo();
	updateCurrentPlaylistConnections();
}

/*!
 * Create a new entry in the current playlist for the given song.
 */
void QUPlaylistArea::addSongToCurrentPlaylist(QUSongFile *song) {
	if(!song)
		return;

	if(!currentPlaylist()) {
		logSrv->add(QString("Could NOT add song \"%1 - %2\" to playlist. Try to create a new playlist.").arg(song->artist()).arg(song->title()), QU::Warning);
		return;
	}

	currentPlaylist()->addEntry(song);

//	updatePlaylistCombo(); // indicate changes
//	playlist->appendItem(currentPlaylist()->last());
}

/*!
 * Shows the playlist with the given index in the playlist widget.
 */
void QUPlaylistArea::setCurrentPlaylist(int index) {
	if(currentPlaylistIndex(index) < 0)
		return;

	QUPlaylistFile *list = playlistDB->at(currentPlaylistIndex(index));
	list->connectSongs();

	qobject_cast<QUPlaylistModel*>(playlist->model())->setIndex(index);
	playlistEdit->setText(list->name());

	// for "new" playlists, disable the "save" button -> user should use "save as" instead
	savePlaylistBtn->setEnabled( list->fileInfo().exists() );
}

/*!
 * Looks for changes in the playlists and updates the combo box.
 */
void QUPlaylistArea::updatePlaylistCombo() {
	if(playlistDB->size() > 0 && playlistCombo->currentIndex() < 0)
		playlistCombo->setCurrentIndex(0);
//	QUProgressDialog dlg(tr("Update playlists..."), playlistDB->size(), this, false);
//	dlg.setPixmap(":/control/playlist.png");
//	dlg.show();
//
//	for(int i = 0; i < playlistCombo->count(); i++) {
//		if(currentPlaylistIndex(i) < 0)
//			continue;
//
//		QString heading = "%1%3 (%2)";
//		heading = heading
//			.arg( playlistDB->at(currentPlaylistIndex(i))->name() )
//			.arg( QUPlaylistFile::dir().relativeFilePath(_playlists.at(currentPlaylistIndex(i))->fileInfo().filePath()) );
//
//		dlg.update(heading.arg(""));
//
//		if(playlistDB.at(currentPlaylistIndex(i))->hasUnsavedChanges())
//			playlistCombo->setItemText(i, heading.arg("*"));
//		else
//			playlistCombo->setItemText(i, heading.arg(""));
//	}

	// TOFIX: Sorting switches current combobox index - why?
	//playlistCombo->model()->sort(0);
}

/*!
 * Looks for missing song connections in the current, visible playlist.
 * \sa connectSongs()
 */
void QUPlaylistArea::updateCurrentPlaylistConnections() {
//	if(currentPlaylistIndex() < 0)
//		return;
//
//	// connect songs with playlist entries
//	playlistDB->at(currentPlaylistIndex())->connectSongs();
//
//	// update view
//	playlist->updateItems();
}

void QUPlaylistArea::updateCurrentPlaylistName(const QString &newName) {
	if(currentPlaylistIndex() < 0)
		return;

	playlistDB->at(currentPlaylistIndex())->setName(newName);
}

void QUPlaylistArea::saveCurrentPlaylist() {
	if(currentPlaylistIndex() < 0)
		return;

	playlistDB->at(currentPlaylistIndex())->save();

	updatePlaylistCombo();
//	playlist->updateItems();
}

void QUPlaylistArea::saveCurrentPlaylistAs() {
	if(currentPlaylistIndex() < 0)
		return;

	QString filePath = QFileDialog::getSaveFileName(this, tr("Save playlist as..."), playlistDB->dir().path(), QString("UltraStar Playlists (%1)").arg(QUSongSupport::allowedPlaylistFiles().join(" ")));

	if(!filePath.isEmpty()) {
		QFileInfo oldFi = playlistDB->at(currentPlaylistIndex())->fileInfo();

		// make current playlist to new playlist and save it
		playlistDB->at(currentPlaylistIndex())->setFileInfo(QFileInfo(filePath));
		this->saveCurrentPlaylist();

		// restore old playlist, not needed for new, not-existent playlists
//		if(oldFi.exists())
//			this->addPlaylist(oldFi.filePath());

		// file should exists now
//		savePlaylistBtn->setEnabled(true);
	}
}

/*!
 * Creates a new, empty playlist.
 */
void QUPlaylistArea::integratePlaylist(QUPlaylistFile *playlist) {
	setAreaEnabled(true);

//	playlistCombo->addItem(
//			QString("%1 (%2)")
//				.arg(playlist->name())
//				.arg(playlist->fileInfo().fileName()),
//			playlistDB->indexOf(newPlaylist)); // save the correct index, needed for sorting
//
//	playlistCombo->setCurrentIndex(playlistCombo->count() - 1); // select this new, empty playlist
//	updatePlaylistCombo();
//
//	playlistCombo->model()->sort(0);
//
//	// Enable the user to start editing the name of the new playlist.
//	playlistEdit->setFocus();
//	playlistEdit->selectAll();
}

/*!
 * For each index in the combobox there is a number saved in the userdata which maps
 * to an index in the playlist list.
 * \param index An index in the playlistCombo
 * \returns An index in the playlist list. -1 on error.
 */
int QUPlaylistArea::currentPlaylistIndex(int index) const {
	if(index == -1)
		index = playlistCombo->currentIndex();

	index = playlistCombo->itemData(index).toInt();

	if(index < 0 or index >= playlistDB->playlists().size())
		return -1;

	return index;
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
	comboLbl->setText(enabled ? tr("Active List:") : tr("No playlists found. Try another folder:"));

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
				tr("Change Playlist Directory"),
				tr("Playlists have been modified."),
				BTN << ":/control/save_all.png" << tr("Save all changed playlists.")
				    << ":/control/bin.png"      << tr("Discard all changes.")
				    << ":/marks/cancel.png"     << tr("Cancel this action."));
		if(result == 0)
			playlistDB->saveUnsavedChanges();
		else if(result == 2)
			return;
	}

	// --------------------------------------------

	QString newPath = QFileDialog::getExistingDirectory(this, tr("Select a location for playlists"), playlistDB->dir().path());

	if(!newPath.isEmpty())
		playlistDB->setDir(newPath);
}

void QUPlaylistArea::removeCurrentPlaylist() {
	if(!currentPlaylist())
		return;

	int result = QUMessageBox::information(this,
			tr("Delete Playlist"),
			QString(tr("<b>\"%1 (%2)\"</b> will be deleted permanently. You cannot undo a delete operation."))
				.arg(currentPlaylist()->name())
				.arg(currentPlaylist()->fileInfo().fileName()),
			BTN << ":/control/bin.png"  << tr("Delete this playlist.")
			    << ":/marks/cancel.png" << tr("Cancel delete operation."));
	if(result == 1)
		return;

	// --------------------------------------------

	playlistDB->deletePlaylist(currentPlaylist());

	int tmpIndex = currentPlaylistIndex();
	QString tmpName = currentPlaylist()->name();

	// update the index references in the userdata of each playlistCombo item
	for(int i = 0; i < playlistCombo->count(); i++) {
		int ref = playlistCombo->itemData(i).toInt();
		if( ref > tmpIndex ) // There should be NO double indices! >=?
			playlistCombo->setItemData(i, ref - 1);
	}

	// remove data structures
	playlistCombo->removeItem(playlistCombo->currentIndex());

	if(playlistCombo->count() == 0)
		this->setAreaEnabled(false);

	logSrv->add(QString(tr("The playlist \"%1\" was removed successfully.")).arg(tmpName), QU::Information);
}

void QUPlaylistArea::removeCurrentPlaylistEntry(QUPlaylistEntry *entry) {
	if(currentPlaylistIndex() < 0)
		return;

//	_playlists.at(currentPlaylistIndex())->removeEntry(entry);

	updatePlaylistCombo();
}

void QUPlaylistArea::removeUnknownEntries() {
	if(currentPlaylistIndex() < 0)
		return;

//	_playlists.at(currentPlaylistIndex())->removeDisconnectedEntries();

	updatePlaylistCombo();
}

/*!
 * Due to internal drag&drop move.
 */
void QUPlaylistArea::changeCurrentPlaylistOrder(QList<QUPlaylistEntry*> newOrder) {
	if(currentPlaylistIndex() < 0)
		return;

//	_playlists.at(currentPlaylistIndex())->changeOrder(newOrder);

//	playlist->updateItems();
	updatePlaylistCombo();
}
