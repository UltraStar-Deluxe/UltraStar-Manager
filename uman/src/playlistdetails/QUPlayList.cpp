#include "QUPlayList.h"

#include <QMenu>
#include <QMessageBox>
#include <QList>

#include "QUPlaylistModel.h"
#include "playlist/QUPlaylistDatabase.h"

QUPlayList::QUPlayList(QWidget *parent): QListView(parent) {
	setAlternatingRowColors(true);
	setModel(new QUPlaylistModel(this));

//	setDragDropMode(QAbstractItemView::InternalMove);

	// context menu
	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));
}

void QUPlayList::keyPressEvent(QKeyEvent *event) {
	if(event->key() == Qt::Key_Delete) {
		this->removeSelectedItems();
	} else {
		QListView::keyPressEvent(event);
	}
}

/*!
 * Internal Move is enabled. We need to apply all changes of the order of the entries
 * to the underlying playlist.
 */
void QUPlayList::dropEvent (QDropEvent *event) {
	QListView::dropEvent(event);

	// create a list with the new QUPlaylistEntry order
//	QList<QUPlaylistEntry*> newOrder;
//	for(int row = 0; row < this->count(); row++) {
//		QUPlayListItem *pItem = dynamic_cast<QUPlayListItem*>(item(row));
//
//		if(!pItem)
//			continue;
//
//		newOrder.append(pItem->entry());
//	}
//	emit orderChanged(newOrder);
}

/*!
 * Shows a context menu with actions for selected playlist items.
 */
void QUPlayList::showContextMenu(const QPoint &point) {
	if(!this->indexAt(point).isValid())
		return; // no item clicked

	QMenu menu(this);

	menu.addAction(QIcon(":/marks/delete.png"), tr("Remove from list"), this, SLOT(removeSelectedItems()), QKeySequence::fromString("Del"));
	menu.addAction(tr("Remove unknown entries"), this, SLOT(removeUnknownEntries()));

	menu.exec(this->mapToGlobal(point));
}

/*!
 * Remove selected items and trigger a removal for the underlying playlist file.
 */
void QUPlayList::removeSelectedItems() {
	foreach(QModelIndex index, selectionModel()->selectedRows()) {
		playlistDB->currentPlaylist()->removeEntry(playlistDB->currentPlaylist()->entry(index.row()));
	}
	update();
}

void QUPlayList::removeUnknownEntries() {
	playlistDB->currentPlaylist()->removeDisconnectedEntries();
	update();
}
