#include "QUPlaylistArea.h"

#include "QUProgressDialog.h"

#include <QFileDialog>
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <QList>

QUPlaylistArea::QUPlaylistArea(QWidget *parent): QWidget(parent) {
	setupUi(this);

	connect(playlistCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(setCurrentPlaylist(int)));
	connect(playlist, SIGNAL(finished(const QString&, QU::EventMessageTypes)), this, SIGNAL(finished(const QString&, QU::EventMessageTypes)));
	connect(playlistEdit, SIGNAL(textEdited(const QString&)), this, SLOT(updateCurrentPlaylistName(const QString&)));
	connect(savePlaylistBtn, SIGNAL(clicked()), this, SLOT(saveCurrentPlaylist()));
	connect(savePlaylistAsBtn, SIGNAL(clicked()), this, SLOT(saveCurrentPlaylistAs()));

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
	_songsRef = songsRef;

	playlistCombo->clear();
	playlistEdit->clear();
	playlist->clear();

	qDeleteAll(_playlists);
	_playlists.clear();

	createPlaylistFiles();

	playlistCombo->setCurrentIndex(0);
}

/*!
 * Update the current playlist view and the combobox items.
 */
void QUPlaylistArea::update() {
	updatePlaylistCombo();
	updateCurrentPlaylistConnections();
}

/*!
 * Open the default playlist path and create all playlist instances.
 */
void QUPlaylistArea::createPlaylistFiles() {
	QDir playlistDir(QUPlaylistFile::dir());
	QFileInfoList fiList = playlistDir.entryInfoList(QUPlaylistFile::allowedTypes(), QDir::Files);

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
			.arg(_playlists.at(currentPlaylistIndex(i))->name())
			.arg(_playlists.at(currentPlaylistIndex(i))->fileInfo().fileName());

		if(_playlists.at(currentPlaylistIndex(i))->hasUnsavedChanges())
			playlistCombo->setItemText(i, heading.arg("*"));
		else
			playlistCombo->setItemText(i, heading.arg(""));
	}

	playlistCombo->model()->sort(0);
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

	QString filePath = QFileDialog::getSaveFileName(this, tr("Save playlist as..."), QUPlaylistFile::dir().path(), QString("UltraStar Playlists (%1)").arg(QUPlaylistFile::allowedTypes().join(" ")));

	if(!filePath.isEmpty()) {
		QFileInfo oldFi = _playlists.at(currentPlaylistIndex())->fileInfo();

		// make current playlist to new playlist and save it
		_playlists.at(currentPlaylistIndex())->setFileInfo(QFileInfo(filePath));
		this->saveCurrentPlaylist();

		// restore old playlist
		this->addPlaylist(oldFi.filePath());
	}
}

/*!
 * Creates a new, empty playlist.
 */
void QUPlaylistArea::addPlaylist() {
	QString fileName = tr("unnamed%1.upl");
	int i = 0;
	QFileInfo newFi(QUPlaylistFile::dir(), fileName.arg(""));

	while(newFi.exists())
		newFi.setFile(newFi.dir(), fileName.arg(i++));

	QUPlaylistFile *newPlaylist = new QUPlaylistFile(newFi.filePath());
	newPlaylist->setName(tr("unnamed"));

	connect(newPlaylist, SIGNAL(finished(const QString&, QU::EventMessageTypes)), this, SIGNAL(finished(const QString&, QU::EventMessageTypes)));

	_playlists.append(newPlaylist);

	playlistCombo->addItem(
			QString("%1 (%2)")
				.arg(newPlaylist->name())
				.arg(newPlaylist->fileInfo().fileName()),
			_playlists.indexOf(newPlaylist)); // save the correct index, needed for sorting

	playlistCombo->setCurrentIndex(playlistCombo->count() - 1); // select this new, empty playlist
	this->updatePlaylistCombo();
}

/*!
 * Does everything to add a new playlist.
 */
void QUPlaylistArea::addPlaylist(const QString &filePath) {
	QUPlaylistFile *newPlaylist = new QUPlaylistFile(filePath);

	connect(newPlaylist, SIGNAL(finished(const QString&, QU::EventMessageTypes)), this, SIGNAL(finished(const QString&, QU::EventMessageTypes)));

	_playlists.append(newPlaylist);

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
