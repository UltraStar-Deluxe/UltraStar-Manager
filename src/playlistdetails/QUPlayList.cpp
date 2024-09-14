#include "QUPlayList.h"
#include "QULogService.h"

#include <QMenu>
#include <QMessageBox>
#include <QList>

#include "QUPlaylistModel.h"
#include "playlist/QUPlaylistDatabase.h"

QUPlayList::QUPlayList(QWidget *parent): QListView(parent) {
	setAlternatingRowColors(true);
	setModel(new QUPlaylistModel(this));

//	setDragDropMode(QAbstractItemView::InternalMove);

	setSelectionMode(QAbstractItemView::SingleSelection);
	//setSelectionMode(QAbstractItemView::ExtendedSelection);
	setMovement(QListView::Free);
	setDragEnabled(true);
	viewport()->setAcceptDrops(true);
	setDropIndicatorShown(true);
	setDragDropMode(QAbstractItemView::InternalMove);

	//setAcceptDrops(true);

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
void QUPlayList::dropEvent(QDropEvent *event) {
	QListView::dropEvent(event);

	// create a list with the new QUPlaylistEntry order
//	QList<QUPlaylistEntry*> newOrder;
//	for(int row = 0; row < this->count(); ++row) {
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

	menu.addAction(QIcon(":/types/music.png"), tr("Game mode 'Normal'"), this, SLOT(setGameModeNormal()));
	menu.addAction(QIcon(":/types/time_mp3.png"), tr("Game mode 'Short'"), this, SLOT(setGameModeShort()));
	if((playlistDB->currentPlaylist()->entry(indexAt(point).row())->song()->hasMedley()) || (selectionModel()->selectedRows().count() > 1)) {
		menu.addAction(QIcon(":/types/medley.png"), tr("Game mode 'Medley'"), this, SLOT(setGameModeMedley()));
	}
	if((playlistDB->currentPlaylist()->entry(indexAt(point).row())->song()->isDuet()) || (selectionModel()->selectedRows().count() > 1)) {
		menu.addAction(QIcon(":/types/duet.png"), tr("Game mode 'Duet'"), this, SLOT(setGameModeDuet()));
	}
	menu.addSeparator();
	menu.addAction(QIcon(":/marks/delete.png"), tr("Remove from list"), QKeySequence::fromString("Del"), this, SLOT(removeSelectedItems()));
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

void QUPlayList::setGameModeNormal() {
	foreach(QModelIndex index, selectionModel()->selectedRows()) {
		playlistDB->currentPlaylist()->entry(index.row())->setGameMode(GAMEMODE_NORMAL);
	}
	update();
}

void QUPlayList::setGameModeShort() {
	foreach(QModelIndex index, selectionModel()->selectedRows()) {
		playlistDB->currentPlaylist()->entry(index.row())->setGameMode(GAMEMODE_SHORT);
	}
	update();
}

void QUPlayList::setGameModeMedley() {
	foreach(QModelIndex index, selectionModel()->selectedRows()) {
		QUPlaylistEntry *entry(playlistDB->currentPlaylist()->entry(index.row()));
		if(entry->song()->hasMedley()) {
			entry->setGameMode(GAMEMODE_MEDLEY);
		} else {
			logSrv->add(QString(tr("Unable to set game mode to 'Medley'. The playlist entry \"%1 - %2\" does NOT have a medley section!")).arg(entry->artistLink(), entry->titleLink()), QU::Warning);
		}
	}
	update();
}

void QUPlayList::setGameModeDuet() {
	foreach(QModelIndex index, selectionModel()->selectedRows()) {
		QUPlaylistEntry *entry(playlistDB->currentPlaylist()->entry(index.row()));
		if(entry->song()->isDuet()) {
			entry->setGameMode(GAMEMODE_DUET);
		} else {
			logSrv->add(QString(tr("Unable to set game mode to 'Duet'. The playlist entry \"%1 - %2\" is NOT a duet!")).arg(entry->artistLink(), entry->titleLink()), QU::Warning);
		}
	}
	update();
}
