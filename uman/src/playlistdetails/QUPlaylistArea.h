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

	QUPlaylistFile* currentPlaylist() const;

public slots:
	void reset();
	void update();

	void addSongToCurrentPlaylist(QUSongFile *song);

private slots:
	void integratePlaylist(QUPlaylistFile *playlist);
	void setCurrentPlaylist(int index);

	void updatePlaylistCombo();
	void updateCurrentPlaylistConnections();
	void updateCurrentPlaylistName(const QString &newName);

	void saveCurrentPlaylist();
	void saveCurrentPlaylistAs();

	void browse();

	void removeCurrentPlaylist();
	void removeCurrentPlaylistEntry(QUPlaylistEntry *entry);
	void removeUnknownEntries();

	void changeCurrentPlaylistOrder(QList<QUPlaylistEntry*> newOrder);

private:
	int currentPlaylistIndex(int index = -1) const;
	void setAreaEnabled(bool enabled = true);
};

#endif /* QUPLAYLISTAREA_H_ */
