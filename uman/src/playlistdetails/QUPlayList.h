#ifndef QUPLAYLIST_H_
#define QUPLAYLIST_H_

#include "QU.h"
#include "QUSongFile.h"
#include "QUPlaylistFile.h"
#include "QUPlayListItem.h"

#include <QDir>
#include <QListView>
#include <QKeyEvent>
#include <QDropEvent>

class QUPlayList: public QListView {
	Q_OBJECT

public:
	QUPlayList(QWidget *parent = 0);

public slots:
	void showContextMenu(const QPoint &point);

protected:
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void dropEvent(QDropEvent *event);

signals:
	void removeUnknownEntriesRequested();
	void removePlaylistEntryRequested(QUPlaylistEntry *entry);
	void orderChanged(QList<QUPlaylistEntry*> newOrder);

private slots:
	void removeSelectedItems();
	void removeUnknownEntries();
};

#endif /* QUPLAYLIST_H_ */
