#include "QUPlayList.h"

QUPlayList::QUPlayList(QWidget *parent): QListWidget(parent) {
	this->setAlternatingRowColors(true);
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

void QUPlayList::updateItems() {
	for(int i = 0; i < this->count(); i++) {
		dynamic_cast<QUPlayListItem*>(this->item(i))->updateData();
	}
}
