#ifndef QUPLAYLISTAREA_H_
#define QUPLAYLISTAREA_H_

#include "QU.h"
#include "QUSongFile.h"
#include "QUPlaylistFile.h"

#include <QString>
#include <QWidget>
#include <QList>

#include "ui_QUPlaylistArea.h"

class QUPlaylistArea: public QWidget, private Ui::QUPlaylistArea {
	Q_OBJECT

public:
	QUPlaylistArea(QWidget *parent = 0);

	QList<QUPlaylistFile*> playlists() const { return _playlists; }
	QUPlaylistFile* currentPlaylist() const;

public slots:
	void refreshAllPlaylists();
	void disconnectPlaylists();

	void update();
	void updateAll();

	void addSongToCurrentPlaylist(QUSongFile *song);

	bool hasUnsavedChanges();
	void saveUnsavedChanges();

private slots:
	void createPlaylistFiles();
	void setCurrentPlaylist(int index);

	void updatePlaylistCombo();
	void updateCurrentPlaylistConnections();
	void updateCurrentPlaylistName(const QString &newName);

	void saveCurrentPlaylist();
	void saveCurrentPlaylistAs();

	void addPlaylist();
	void addPlaylist(const QString &filePath);

	void changePlaylistDir();

	void removeCurrentPlaylist();
	void removeCurrentPlaylistEntry(QUPlaylistEntry *entry);
	void removeUnknownEntries();

	void changeCurrentPlaylistOrder(QList<QUPlaylistEntry*> newOrder);

private:
	QList<QUPlaylistFile*>  _playlists;

	int currentPlaylistIndex(int index = -1) const;
	void setAreaEnabled(bool enabled = true);
};

#endif /* QUPLAYLISTAREA_H_ */
