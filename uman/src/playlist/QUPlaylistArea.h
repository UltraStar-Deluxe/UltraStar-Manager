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
	~QUPlaylistArea();

public slots:
	void refreshAllPlaylists(QList<QUSongFile*> *songRef);
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

	void changeCurrentPlaylistOrder(QList<QUPlaylistEntry*> newOrder);

signals:
	void finished(const QString &message, QU::EventMessageTypes type);

private:
	QList<QUPlaylistFile*>  _playlists;
	QList<QUSongFile*>     *_songsRef;

	int currentPlaylistIndex(int index = -1) const;
	void setAreaEnabled(bool enabled = true);
};

#endif /* QUPLAYLISTAREA_H_ */
