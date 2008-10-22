#ifndef QUPLAYLIST_H_
#define QUPLAYLIST_H_

#include "QU.h"
#include "QUSongFile.h"
#include "QUPlaylistFile.h"
#include "QUPlayListItem.h"

#include <QDir>
#include <QListWidget>
#include <QListWidgetItem>
#include <QKeyEvent>
#include <QDropEvent>

class QUPlayList: public QListWidget {
	Q_OBJECT

public:
	QUPlayList(QWidget *parent = 0);

	void setItems(QUPlaylistFile *playlist);
	void appendItem(QUPlaylistEntry *entry);

public slots:
	void showContextMenu(const QPoint &point);
	void updateItems();

protected:
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void dropEvent(QDropEvent *event);

signals:
	void removePlaylistEntryRequested(QUPlaylistEntry *entry);
	void orderChanged(QList<QUPlaylistEntry*> newOrder);

private slots:
	void removeSelectedItems();

};

#endif /* QUPLAYLIST_H_ */
