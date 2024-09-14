#include "QUPlayListItem.h"

#include <QFont>

QUPlayListItem::QUPlayListItem(QUPlaylistEntry *entry, QListWidget *parent): QListWidgetItem(parent) {
	_entry = entry;
	updateData();

	connect(entry, SIGNAL(disconnected()), this, SLOT(updateData()));
}

/*!
 * Call this function to update all displayed information of this item.
 */
void QUPlayListItem::updateData() {
	// remove visible "data changed" state
	QFont f(this->font());
	f.setBold(false);

	if(entry()->song()) {
		this->setText(QString("%1. %2 - %3").arg(listWidget()->row(this) + 1).arg(entry()->song()->artist(), entry()->song()->title()));
		this->setForeground(Qt::black);

		if(entry()->hasUnsavedChanges())
			f.setBold(true);

	} else {
		this->setText(QString("%1. %2 - %3 (not found)").arg(listWidget()->row(this) + 1).arg(entry()->artistLink(), entry()->titleLink()));
		this->setForeground(Qt::gray);
	}

	this->setFont(f);
}
