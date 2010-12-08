#ifndef QUPLAYLISTDATABASE_H
#define QUPLAYLISTDATABASE_H

#define playlistDB (QUPlaylistDatabase::instance())

#include "QU.h"
#include "QUPlaylistFile.h"

#include <QObject>
#include <QWidget>
#include <QList>
#include <QDir>

class QUPlaylistDatabase: public QObject {
	Q_OBJECT

public:
	void setParentWidget(QWidget *w) { _parent = w; }
	QWidget* parentWidget() const { return _parent; }

	QDir dir();
	void setDir(const QDir &dir);

	void reload();
	void clear();
	void save();

	void deletePlaylist(QUPlaylistFile *playlist);

	const QList<QUPlaylistFile*>& playlists() const { return _playlists; }
	QUPlaylistFile* at(int index) const;

	bool hasUnsavedChanges() const;
	void saveUnsavedChanges();

	int size() const { return _playlists.size(); }

	int currentIndex() const { return _currentIndex; }
	QUPlaylistFile* currentPlaylist() const;

	void loadPlaylist(const QString &filePath);

public slots:
	void createPlaylist();
	void connectAllPlaylists();
	void disconnectAllPlaylists();

	void setCurrentIndex(int i) { _currentIndex = i; emit currentPlaylistChanged(currentPlaylist()); }

protected slots:
	void signalPlaylistChanged();

signals:
	void playlistAdded(QUPlaylistFile*);
	void playlistDeleted(QUPlaylistFile*); // DO NOT de-reference this pointer!
	void playlistChanged(QUPlaylistFile*);
	void databaseCleared();
	void databaseReloaded();
	void databaseDisconnected();
	void databaseConnected();

	// a new current playlist was selected - may pass a null pointer
	void currentPlaylistChanged(QUPlaylistFile*);

private:
	QList<QUPlaylistFile*>  _playlists;
	int                     _currentIndex;
	QWidget                *_parent; // used for dialogs like progress bars

/*!
 * Use the singleton pattern because we need only one
 * database at a time.
 */
public:
	static QUPlaylistDatabase* instance();
protected:
	QUPlaylistDatabase(QObject *parent = 0);
private:
	static QUPlaylistDatabase *_instance;
};

#endif // QUPLAYLISTDATABASE_H
