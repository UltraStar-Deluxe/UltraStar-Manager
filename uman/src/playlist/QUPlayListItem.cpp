#include "QUPlayListItem.h"

QUPlayListItem::QUPlayListItem(QUSongFile *song, QListWidget *parent): QListWidgetItem(parent) {
	_textBuffer = QObject::tr("Item information not available.");
	_song = song;

	updateData();
}

/*!
 * Call this function to update all displayed information of this item.
 */
void QUPlayListItem::updateData() {
	if(_song) {
		_textBuffer = QString("%1 - %2").arg(_song->artist()).arg(_song->title());
		this->setTextColor(Qt::black);
	} else {
		this->setTextColor(Qt::gray);
	}

	this->setText(_textBuffer);
}
