#include "QUPlaylistArea.h"

#include "QUProgressDialog.h"
#include "QUMessageBox.h"

#include <QFileDialog>
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <QList>
#include <QFile>
#include <QMessageBox>

QUPlaylistArea::QUPlaylistArea(QWidget *parent): QWidget(parent) {
	setupUi(this);

	connect(playlist, SIGNAL(finished(const QString&, QU::EventMessageTypes)), this, SIGNAL(finished(const QString&, QU::EventMessageTypes)));
	connect(playlist, SIGNAL(removePlaylistEntryRequested(QUPlaylistEntry*)), this, SLOT(removeCurrentPlaylistEntry(QUPlaylistEntry*)));
	connect(playlist, SIGNAL(orderChanged(QList<QUPlaylistEntry*>)), this, SLOT(changeCurrentPlaylistOrder(QList<QUPlaylistEntry*>)));

	connect(playlistCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(setCurrentPlaylist(int)));
	connect(playlistEdit, SIGNAL(textEdited(const QString&)), this, SLOT(updateCurrentPlaylistName(const QString&)));
	connect(savePlaylistBtn, SIGNAL(clicked()), this, SLOT(saveCurrentPlaylist()));
	connect(savePlaylistAsBtn, SIGNAL(clicked()), this, SLOT(saveCurrentPlaylistAs()));
	connect(createPlaylistBtn, SIGNAL(clicked()), this, SLOT(addPlaylist()));
	connect(setPlaylistFolderBtn, SIGNAL(clicked()), this, SLOT(changePlaylistDir()));
	connect(removePlaylistBtn, SIGNAL(clicked()), this, SLOT(removeCurrentPlaylist()));


	playlistCombo->view()->setTextElideMode(Qt::ElideRight);
}

QUPlaylistArea::~QUPlaylistArea() {
	_songsRef = 0;
}

void QUPlaylistArea::disconnectPlaylists() {
	foreach(QUPlaylistFile *_playlist, _playlists) {
		_playlist->disconnectSongs();
		playlist->updateItems();
		updatePlaylistCombo();
	}
}

void QUPlaylistArea::refreshAllPlaylists(QList<QUSongFile*> *songsRef) {
	// VERY IMPORTANT! Take a reference to the song list of the main window.
	// Only a reading access is intended.
	_songsRef = songsRef;

	// Clear all present playlist data.
	playlistCombo->clear();
	playlistEdit->clear();
	playlist->clear();

	qDeleteAll(_playlists);
	_playlists.clear();

	// Disable not needed controls.
	this->setAreaEnabled(false);

	// Create new content.
	createPlaylistFiles();

	playlistCombo->setCurrentIndex(0);

	// update window title of parent dock widget with current playlist path
	playlistPathLbl->setText(QString(tr("<font color=#808080>%1</font>")).arg(QUPlaylistFile::dir().path()));
}

/*!
 * Update the current playlist view and the combobox items.
 */
void QUPlaylistArea::update() {
	updatePlaylistCombo();
	updateCurrentPlaylistConnections();
}

/*!
 * Like update() but sets all song connections.
 */
void QUPlaylistArea::updateAll() {
	QUProgressDialog dlg(tr("Check playlists for new songs..."), _playlists.size(), this, false);
	dlg.setPixmap(":/control/playlist.png");
	dlg.show();

	foreach(QUPlaylistFile *_playlist, _playlists) {
		dlg.update(_playlist->name());
		_playlist->connectSongs(*_songsRef);
	}

	update();
}

/*!
 * Create a new entry in the current playlist for the given song.
 */
void QUPlaylistArea::addSongToCurrentPlaylist(QUSongFile *song) {
	if(!song)
		return;

	if(currentPlaylistIndex() < 0) {
		emit finished(QString("Could NOT add song \"%1 - %2\" to playlist. Try to create a new one.").arg(song->artist()).arg(song->title()), QU::warning);
		return;
	}

	if(!_playlists.at(currentPlaylistIndex())->addEntry(song))
		return;

	updatePlaylistCombo(); // indicate changes
	playlist->appendItem(_playlists.at(currentPlaylistIndex())->entry(_playlists.at(currentPlaylistIndex())->count() - 1));
}

/*!
 * \returns true, if one of the loaded playlists has unsaved changes
 */
bool QUPlaylistArea::hasUnsavedChanges() {
	foreach(QUPlaylistFile *list, _playlists) {
		if(list->hasUnsavedChanges())
			return true;
	}

	return false;
}

/*!
 * Save all playlists that have unsaved changes.
 */
void QUPlaylistArea::saveUnsavedChanges() {
	foreach(QUPlaylistFile *list, _playlists) {
		if(!list->hasUnsavedChanges())
			continue;

		if(list->fileInfo().exists())
			list->save();
		else {
			QString filePath = QFileDialog::getSaveFileName(this, QString(tr("Save playlist \"%1\" as...")).arg(list->name()), QUPlaylistFile::dir().path(), QString("UltraStar Playlists (%1)").arg(QU::allowedPlaylistFiles().join(" ")));

			if(!filePath.isEmpty()) {
				list->setFileInfo(QFileInfo(filePath));
				list->save();
			} else {
				emit finished(QString(tr("The new playlist \"%1\" was not saved.")).arg(list->name()), QU::information);
			}
		}
	}

}

/*!
 * Open the default playlist path and create all playlist instances.
 */
void QUPlaylistArea::createPlaylistFiles() {
	QDir playlistDir(QUPlaylistFile::dir());
	QFileInfoList fiList = playlistDir.entryInfoList(QU::allowedPlaylistFiles(), QDir::Files);

	QUProgressDialog dlg(tr("Reading playlist files..."), fiList.size(), this);
	dlg.setPixmap(":/control/playlist.png");
	dlg.show();

	foreach(QFileInfo fi, fiList) {
		dlg.update(fi.fileName());
		if(dlg.cancelled()) break;

		this->addPlaylist(fi.filePath());
	}
}

/*!
 * Shows the playlist with the given index in the playlist widget.
 */
void QUPlaylistArea::setCurrentPlaylist(int index) {
	if(currentPlaylistIndex(index) < 0)
		return;

	_playlists.at(currentPlaylistIndex(index))->connectSongs(*_songsRef);

	playlist->setItems(_playlists.at(currentPlaylistIndex(index)));
	playlistEdit->setText(_playlists.at(currentPlaylistIndex(index))->name());

	// for "new" playlists, disable the "save" button -> user should use "save as" instead
	savePlaylistBtn->setEnabled( _playlists.at(currentPlaylistIndex(index))->fileInfo().exists() );
}

/*!
 * Looks for changes in the playlists and updates the combo box.
 */
void QUPlaylistArea::updatePlaylistCombo() {
	for(int i = 0; i < playlistCombo->count(); i++) {
		if(currentPlaylistIndex(i) < 0)
			continue;

		QString heading = "%1%3 (%2)";
		heading = heading
			.arg( _playlists.at(currentPlaylistIndex(i))->name() )
			.arg( QUPlaylistFile::dir().relativeFilePath(_playlists.at(currentPlaylistIndex(i))->fileInfo().filePath()) );

		if(_playlists.at(currentPlaylistIndex(i))->hasUnsavedChanges())
			playlistCombo->setItemText(i, heading.arg("*"));
		else
			playlistCombo->setItemText(i, heading.arg(""));
	}

	// TOFIX: Sorting switches current combobox index - why?
	//playlistCombo->model()->sort(0);
}

/*!
 * Looks for missing song connections in the current, visible playlist.
 * \sa connectSongs()
 */
void QUPlaylistArea::updateCurrentPlaylistConnections() {
	if(currentPlaylistIndex() < 0)
		return;

	// connect songs with playlist entries
	_playlists.at(currentPlaylistIndex())->connectSongs(*_songsRef);

	// update view
	playlist->updateItems();
}

void QUPlaylistArea::updateCurrentPlaylistName(const QString &newName) {
	if(currentPlaylistIndex() < 0)
		return;

	_playlists.at(currentPlaylistIndex())->setName(newName);
	updatePlaylistCombo();
}

void QUPlaylistArea::saveCurrentPlaylist() {
	if(currentPlaylistIndex() < 0)
		return;

	_playlists.at(currentPlaylistIndex())->save();

	updatePlaylistCombo();
	playlist->updateItems();
}

void QUPlaylistArea::saveCurrentPlaylistAs() {
	if(currentPlaylistIndex() < 0)
		return;

	QString filePath = QFileDialog::getSaveFileName(this, tr("Save playlist as..."), QUPlaylistFile::dir().path(), QString("UltraStar Playlists (%1)").arg(QU::allowedPlaylistFiles().join(" ")));

	if(!filePath.isEmpty()) {
		QFileInfo oldFi = _playlists.at(currentPlaylistIndex())->fileInfo();

		// make current playlist to new playlist and save it
		_playlists.at(currentPlaylistIndex())->setFileInfo(QFileInfo(filePath));
		this->saveCurrentPlaylist();

		// restore old playlist, not needed for new, not-existent playlists
		if(oldFi.exists())
			this->addPlaylist(oldFi.filePath());

		// file should exists now
		savePlaylistBtn->setEnabled(true);
	}
}

/*!
 * Creates a new, empty playlist.
 */
void QUPlaylistArea::addPlaylist() {
	QUPlaylistFile *newPlaylist = new QUPlaylistFile(QFileInfo(QUPlaylistFile::dir(), "???").filePath());
	newPlaylist->setName(tr("New Playlist"));

	connect(newPlaylist, SIGNAL(finished(const QString&, QU::EventMessageTypes)), this, SIGNAL(finished(const QString&, QU::EventMessageTypes)));

	_playlists.append(newPlaylist);
	this->setAreaEnabled(true);

	playlistCombo->addItem(
			QString("%1 (%2)")
				.arg(newPlaylist->name())
				.arg(newPlaylist->fileInfo().fileName()),
			_playlists.indexOf(newPlaylist)); // save the correct index, needed for sorting

	playlistCombo->setCurrentIndex(playlistCombo->count() - 1); // select this new, empty playlist
	this->updatePlaylistCombo();

	// Enable the user to start editing the name of the new playlist.
	playlistEdit->setFocus();
	playlistEdit->selectAll();
}

/*!
 * Does everything to add a new playlist.
 */
void QUPlaylistArea::addPlaylist(const QString &filePath) {
	QUPlaylistFile *newPlaylist = new QUPlaylistFile(filePath);

	connect(newPlaylist, SIGNAL(finished(const QString&, QU::EventMessageTypes)), this, SIGNAL(finished(const QString&, QU::EventMessageTypes)));

	_playlists.append(newPlaylist);
	this->setAreaEnabled(true);

	playlistCombo->addItem(
			QString("%1 (%2)")
				.arg(newPlaylist->name())
				.arg(newPlaylist->fileInfo().fileName()),
			_playlists.indexOf(newPlaylist)); // save the correct index, needed for sorting

	playlistCombo->model()->sort(0);

	// set song connections for the first time of this playlist
	newPlaylist->connectSongs(*_songsRef);
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

	if(index < 0 or index >= _playlists.size())
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
		playlist->clear();
		playlistEdit->clear();
	}
}

/*!
 * Select a new folder and look there for new playlists. Old ones are discarded.
 */
void QUPlaylistArea::changePlaylistDir() {
	if(this->hasUnsavedChanges()) {
		QUMessageBox::Results result = QUMessageBox::ask(this,
				tr("Change Playlist Directory"),
				tr("Playlists have been modified."),
				":/control/save_all.png", tr("Save all changed playlists."),
				":/control/bin.png", tr("Discard all changes."),
				":/marks/cancel.png", tr("Cancel this action."));
		if(result == QUMessageBox::first)
			this->saveUnsavedChanges();
		else if(result == QUMessageBox::third)
			return;
	}

	// --------------------------------------------

	QString newPath = QFileDialog::getExistingDirectory(this, tr("Select a location for playlists"), QUPlaylistFile::dir().path());

	if(!newPath.isEmpty()) {
		QUPlaylistFile::setDir(QDir(newPath));

		refreshAllPlaylists(_songsRef);

		emit finished(QString("Folder for playlists changed to: \"%1\"").arg(newPath), QU::information);
	}
}

void QUPlaylistArea::removeCurrentPlaylist() {
	if(currentPlaylistIndex() < 0)
		return;

	QUMessageBox::Results result = QUMessageBox::ask(this,
			tr("Delete Playlist"),
			QString(tr("<b>\"%1 (%2)\"</b> will be deleted permanently. You cannot undo a delete operation."))
				.arg(_playlists.at(currentPlaylistIndex())->name())
				.arg(_playlists.at(currentPlaylistIndex())->fileInfo().fileName()),
			":/control/bin.png", tr("Delete this playlist."),
			":/marks/cancel.png", tr("Cancel delete operation."));
	if(result == QUMessageBox::second)
		return;

	// --------------------------------------------

	if( QFile::remove(_playlists.at(currentPlaylistIndex())->fileInfo().filePath()) )
		emit finished(QString(tr("The playlist file \"%1\" was removed successfully.")).arg(_playlists.at(currentPlaylistIndex())->fileInfo().filePath()), QU::information);
	else
		emit finished(QString(tr("The playlist file \"%1\" could NOT be removed. Does it exist?")).arg(_playlists.at(currentPlaylistIndex())->fileInfo().filePath()), QU::warning);

	int tmpIndex = currentPlaylistIndex();
	QString tmpName = _playlists.at(currentPlaylistIndex())->name();

	// update the index references in the userdata of each playlistCombo item
	for(int i = 0; i < playlistCombo->count(); i++) {
		int ref = playlistCombo->itemData(i).toInt();
		if( ref > tmpIndex ) // There should be NO double indices! >=?
			playlistCombo->setItemData(i, ref - 1);
	}

	// remove data structures
	playlistCombo->removeItem(playlistCombo->currentIndex());

	delete _playlists.at(tmpIndex);
	_playlists.removeAt(tmpIndex);

	if(playlistCombo->count() == 0)
		this->setAreaEnabled(false);

	emit finished(QString(tr("The playlist \"%1\" was removed successfully.")).arg(tmpName), QU::information);
}

void QUPlaylistArea::removeCurrentPlaylistEntry(QUPlaylistEntry *entry) {
	if(currentPlaylistIndex() < 0)
		return;

	_playlists.at(currentPlaylistIndex())->removeEntry(entry);

	updatePlaylistCombo();
}

/*!
 * Due to internal drag&drop move.
 */
void QUPlaylistArea::changeCurrentPlaylistOrder(QList<QUPlaylistEntry*> newOrder) {
	if(currentPlaylistIndex() < 0)
		return;

	_playlists.at(currentPlaylistIndex())->changeOrder(newOrder);

	playlist->updateItems();
	updatePlaylistCombo();
}
