#include "QUPlayList.h"

#include <QMenu>

QUPlayList::QUPlayList(QWidget *parent): QListWidget(parent) {
	this->setAlternatingRowColors(true);

	// context menu
	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));
}

/*!
 * Creates new items for the list according to the given playlist object.
 */
void QUPlayList::setItems(QUPlaylistFile *playlist) {
	this->clear();

	for(int i = 0; i < playlist->count(); i++) {
		this->addItem(new QUPlayListItem(playlist->entry(i), this));
	}
}

void QUPlayList::appendItem(QUPlaylistEntry *entry) {
	this->addItem(new QUPlayListItem(entry, this));
}

void QUPlayList::updateItems() {
	for(int i = 0; i < this->count(); i++) {
		dynamic_cast<QUPlayListItem*>(this->item(i))->updateData();
	}
}

void QUPlayList::keyPressEvent(QKeyEvent *event) {
	if(event->key() == Qt::Key_Delete) {
		this->removeSelectedItems();
	} else {
		QListWidget::keyPressEvent(event);
	}
}

/*!
 * Shows a context menu with actions for selected playlist items.
 */
void QUPlayList::showContextMenu(const QPoint &point) {
	if(!this->itemAt(point))
		return; // no item clicked

	QMenu menu(this);

	menu.addAction(QIcon(":/control/bin.png"), tr("Remove from list"), this, SLOT(removeSelectedItems()), QKeySequence::fromString("Del"));

	menu.exec(this->mapToGlobal(point));
}

/*!
 * Remove selected items and trigger a removal for the underlying playlist file.
 */
void QUPlayList::removeSelectedItems() {
	int firstIndex = 0;

	foreach(QListWidgetItem *item, this->selectedItems()) {
		QUPlayListItem *pItem = dynamic_cast<QUPlayListItem*>(item);

		if(!pItem)
			continue;

		firstIndex = row(item);

		emit removePlaylistEntryRequested(pItem->entry());
		delete item;
	}

	updateItems(); // update the running number

	this->setCurrentItem(this->item( qMin(firstIndex, this->count() - 1) ));
	if(this->currentItem())
		this->currentItem()->setSelected(true);
}
