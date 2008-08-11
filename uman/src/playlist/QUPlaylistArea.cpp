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
	// TODO: save unsaved playlists
	_songsRef = songsRef;

	playlistCombo->clear();
	playlistEdit->clear();
	playlist->clear();

	qDeleteAll(_playlists);
	_playlists.clear();

	createPlaylistFiles();

	foreach(QUPlaylistFile *playlist, _playlists) {
		playlistCombo->addItem(QString("%1 (%2)").arg(playlist->name()).arg(playlist->fileInfo().fileName()));
	}

	playlistCombo->setCurrentIndex(0);
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

		_playlists.append(new QUPlaylistFile(fi.filePath()));

		// enable event log
		connect(_playlists.last(), SIGNAL(finished(const QString&, QU::EventMessageTypes)), this, SIGNAL(finished(const QString&, QU::EventMessageTypes)));
	}
}

/*!
 * Shows the playlist with the given index in the playlist widget.
 */
void QUPlaylistArea::setCurrentPlaylist(int index) {
	if(index < 0 or index >= _playlists.size())
		return;

	// connect songs with playlist entries
	_playlists.at(index)->connectSongs(*_songsRef);

	playlist->setItems(_playlists.at(index));
	playlistEdit->setText(_playlists.at(index)->name());
}

/*!
 * Looks for changes in the playlists and updates the combo box.
 */
void QUPlaylistArea::updatePlaylistCombo() {
	for(int i = 0; i < _playlists.size(); i++) {
		if(_playlists.at(i)->hasUnsavedChanges())
			playlistCombo->setItemText(i, QString("%1* (%2)").arg(_playlists.at(i)->name()).arg(_playlists.at(i)->fileInfo().fileName()));
		else
			playlistCombo->setItemText(i, QString("%1 (%2)").arg(_playlists.at(i)->name()).arg(_playlists.at(i)->fileInfo().fileName()));
	}
}

void QUPlaylistArea::updateCurrentPlaylistConnections() {
	if(playlistCombo->currentIndex() < 0 or playlistCombo->currentIndex() >= _playlists.size())
		return;

	// connect songs with playlist entries
	_playlists.at(playlistCombo->currentIndex())->connectSongs(*_songsRef);

	playlist->updateItems();
}

void QUPlaylistArea::updateCurrentPlaylistName(const QString &newName) {
	if(playlistCombo->currentIndex() < 0 or playlistCombo->currentIndex() >= _playlists.size())
		return;

	// connect songs with playlist entries
	_playlists.at(playlistCombo->currentIndex())->setName(newName);

	updatePlaylistCombo();
}

void QUPlaylistArea::updatePlaylistItems() {
	playlist->updateItems();
}

void QUPlaylistArea::saveCurrentPlaylist() {
	if(playlistCombo->currentIndex() < 0 or playlistCombo->currentIndex() >= _playlists.size())
		return;

	_playlists.at(playlistCombo->currentIndex())->save();

	updatePlaylistCombo();
	playlist->updateItems();
}

void QUPlaylistArea::saveCurrentPlaylistAs() {
	if(playlistCombo->currentIndex() < 0 or playlistCombo->currentIndex() >= _playlists.size())
		return;

	QString filePath = QFileDialog::getSaveFileName(this, tr("Save playlist as..."), QUPlaylistFile::dir().path(), QString("UltraStar Playlists (%1)").arg(QUPlaylistFile::allowedTypes().join(" ")));

	if(!filePath.isEmpty()) {
		QFileInfo oldFi = _playlists.at(playlistCombo->currentIndex())->fileInfo();

		// make current playlist to new playlist and save it
		_playlists.at(playlistCombo->currentIndex())->setFileInfo(QFileInfo(filePath));
		this->saveCurrentPlaylist();

		// restore old playlist
		_playlists.append(new QUPlaylistFile(oldFi.filePath()));
		connect(_playlists.last(), SIGNAL(finished(const QString&, QU::EventMessageTypes)), this, SLOT(addLogMsg(const QString&, QU::EventMessageTypes)));

		playlistCombo->addItem(QString("%1 (%2)").arg(_playlists.last()->name()).arg(_playlists.last()->fileInfo().fileName()));
	}
}
