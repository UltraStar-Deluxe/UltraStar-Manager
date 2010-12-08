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

public slots:
	void reset();

	void addSongToCurrentPlaylist(QUSongFile *song);

private slots:
	void updateCurrentPlaylistName(const QString &newName);
	void updateCurrentPlaylistNameEdit();
	void updateComboboxIndex();

	void saveCurrentPlaylist();
	void saveCurrentPlaylistAs();

	void browse();

	void removeCurrentPlaylist();

private:
	void setAreaEnabled(bool enabled = true);
};

#endif /* QUPLAYLISTAREA_H_ */
