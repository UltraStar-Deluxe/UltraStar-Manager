#include "QUPlaylistDatabase.h"
#include "QUSongDatabase.h"
#include "QUProgressDialog.h"
#include "QULogService.h"
#include "QUSongSupport.h"

#include <QSettings>
#include <QFileDialog>

QUPlaylistDatabase::QUPlaylistDatabase(QObject *parent): QObject(parent) {
	connect(songDB, SIGNAL(databaseCleared()), this, SLOT(disconnectAllPlaylists()));
	connect(songDB, SIGNAL(databaseReloaded()), this, SLOT(connectAllPlaylists()));
}

QUPlaylistDatabase* QUPlaylistDatabase::_instance = 0;
QUPlaylistDatabase* QUPlaylistDatabase::instance() {
	if(_instance == 0)
		_instance = new QUPlaylistDatabase(0);

	return _instance;
}

void QUPlaylistDatabase::signalPlaylistChanged() {
	emit playlistChanged(qobject_cast<QUPlaylistFile*>(sender()));
}

/*!
 * \returns directory of the current playlistFile location
 */
QDir QUPlaylistDatabase::dir() {
	QSettings settings;

	if(!settings.contains("playlistFilePath")) {
		QDir defaultDir = QU::BaseDir;
		defaultDir.cdUp();
		defaultDir.cd("playlists"); // default location of playlistFiles, relative to song dir

		settings.setValue("playlistFilePath", defaultDir.path());
	}

	return QDir(settings.value("playlistFilePath").toString());
}

void QUPlaylistDatabase::setDir(const QDir &dir) {
	QSettings().setValue("playlistFilePath", dir.path());
	logSrv->add(QString("Folder for playlists changed to: \"%1\"").arg(dir.path()), QU::Information);
	reload();
}

void QUPlaylistDatabase::createPlaylist() {
	_playlists << new QUPlaylistFile(this);
	connect(_playlists.last(), SIGNAL(dataChanged()), this, SLOT(signalSongChanged()));
	emit playlistAdded(_playlists.last());
}

void QUPlaylistDatabase::connectAllPlaylists() {
	QUProgressDialog dlg(tr("Check playlists for new songs..."), _playlists.size(), parentWidget(), false);
	dlg.setPixmap(":/control/playlist.png");
	dlg.show();

	foreach(QUPlaylistFile *playlist, _playlists) {
		dlg.update(QString("%1 (%2)").arg(playlist->name()).arg(playlist->fileInfo().fileName()));
		playlist->connectSongs();
	}

	emit databaseConnected();
}

void QUPlaylistDatabase::disconnectAllPlaylists() {
	QUProgressDialog dlg(tr("Disconnect playlists from songs..."), _playlists.size(), parentWidget(), false);
	dlg.setPixmap(":/control/playlist.png");
	dlg.show();

	foreach(QUPlaylistFile *playlist, _playlists) {
		dlg.update(QString("%1 (%2)").arg(playlist->name()).arg(playlist->fileInfo().fileName()));
		playlist->disconnectSongs();
	}

	emit databaseDisconected();
}


/*!
 * Open the default playlist path and create all playlist instances.
 */
void QUPlaylistDatabase::reload() {
	clear();

	QFileInfoList fiList = dir().entryInfoList(QUSongSupport::allowedPlaylistFiles(), QDir::Files);

	QUProgressDialog dlg(tr("Reading playlist files..."), fiList.size(), parentWidget());
	dlg.setPixmap(":/control/playlist.png");
	dlg.show();

	foreach(QFileInfo fi, fiList) {
		dlg.update(fi.fileName());
		if(dlg.cancelled()) break;

		loadPlaylist(fi.filePath());
	}

	emit databaseReloaded();
}

void QUPlaylistDatabase::clear() {
	emit databaseCleared();
	qDeleteAll(_playlists);
	_playlists.clear();
}

/*!
 * Save all unsaved changes in playlists.
 */
void QUPlaylistDatabase::save() {
	foreach(QUPlaylistFile *playlist, playlists()) {
		if(!playlist->hasUnsavedChanges())
			continue;

		if(playlist->fileInfo().exists())
			playlist->save();
		else {
			QString filePath = QFileDialog::getSaveFileName(parentWidget(),
					QString(tr("Save playlist \"%1\" as...")).arg(playlist->name()), dir().path(), QString("UltraStar Playlists (%1)").arg(QUSongSupport::allowedPlaylistFiles().join(" ")));

			if(!filePath.isEmpty()) {
				playlist->setFileInfo(QFileInfo(filePath));
				playlist->save();
			} else {
				logSrv->add(QString(tr("The new playlist \"%1\" was not saved.")).arg(playlist->name()), QU::Warning);
			}
		}
	}
}

void QUPlaylistDatabase::deletePlaylist(QUPlaylistFile *playlist) {
	QString name = playlist->name();

	if(!QFile::remove(playlist->fileInfo().filePath())) {
		logSrv->add(QString(tr("Could NOT delete playlist file: \"%1\"")).arg(playlist->fileInfo().filePath()), QU::Error);
		return;
	}

	_playlists.removeAll(playlist);
	emit playlistDeleted(playlist);
	delete playlist;

	logSrv->add(QString(tr("Playlist was deleted successfully: \"%1\"")).arg(name), QU::Information);
}

QUPlaylistFile* QUPlaylistDatabase::at(int index) const {
	if(index < 0 or index > playlists().size() - 1)
		return 0;
	return playlists().at(index);
}

/*!
 * \returns true, if one of the loaded playlists has unsaved changes
 */
bool QUPlaylistDatabase::hasUnsavedChanges() const {
	QUProgressDialog dlg(tr("Looking for unsaved playlists..."), playlists().size(), parentWidget(), false);
	dlg.setPixmap(":/control/playlist.png");
	dlg.show();

	foreach(QUPlaylistFile *playlist, playlists()) {
		dlg.update(QString("%1 (%2)").arg(playlist->name()).arg(playlist->fileInfo().fileName()));

		if(playlist->hasUnsavedChanges())
			return true;
	}

	return false;
}

void QUPlaylistDatabase::saveUnsavedChanges() {
	QUProgressDialog dlg(tr("Saving changed laylists..."), playlists().size(), parentWidget(), false);
	dlg.setPixmap(":/marks/disk.png");
	dlg.show();

	foreach(QUPlaylistFile *playlist, playlists()) {
		dlg.update(QString("%1 (%2)").arg(playlist->name()).arg(playlist->fileInfo().fileName()));

		if(playlist->hasUnsavedChanges())
			playlist->save();
	}
}

void QUPlaylistDatabase::loadPlaylist(const QString &filePath) {
	_playlists << new QUPlaylistFile(filePath, this);
	connect(_playlists.last(), SIGNAL(dataChanged()), this, SLOT(signalPlaylistChanged()));
	emit playlistAdded(_playlists.last());
}
