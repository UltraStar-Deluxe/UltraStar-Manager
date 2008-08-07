#include "QUPlayList.h"
#include "QUMainWindow.h"

QUPlayList::QUPlayList(QWidget *parent): QListWidget(parent) {
}

/*!
 * \returns directory of the current playlist location
 */
QDir QUPlayList::dir() const {
	QSettings settings;

	QDir defaultDir = QUMainWindow::BaseDir;
	defaultDir.cdUp();
	defaultDir.cd("playlists"); // default location of playlists, relative to song dir

	return QDir(settings.value("playlistPath", defaultDir.path()).toString());
}

void QUPlayList::setDir(const QDir &dir) {
	QSettings settings;

	settings.setValue("playlistPath", dir.path());
}
