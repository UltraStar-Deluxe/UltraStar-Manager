#ifndef QUPLAYLISTITEM_H_
#define QUPLAYLISTITEM_H_

#include "QU.h"
#include "QUPlaylistEntry.h"

#include <QListWidgetItem>

class QUPlayListItem: public QListWidgetItem {
public:
	QUPlayListItem(QUPlaylistEntry *entry, QListWidget *parent = 0);

	void updateData();
	QUPlaylistEntry* entry() const { return _entry; }

private:
	QUPlaylistEntry *_entry;
};

#endif /* QUPLAYLISTITEM_H_ */
