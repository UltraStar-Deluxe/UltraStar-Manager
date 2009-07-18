#ifndef QUPLAYLISTITEM_H_
#define QUPLAYLISTITEM_H_

#include "QU.h"
#include "QUPlaylistEntry.h"

#include <QListWidgetItem>
#include <QObject>

class QUPlayListItem: public QObject, public QListWidgetItem {
	Q_OBJECT

public:
	QUPlayListItem(QUPlaylistEntry *entry, QListWidget *parent = 0);

	QUPlaylistEntry* entry() const { return _entry; }

public slots:
	void updateData();

private:
	QUPlaylistEntry *_entry;
};

#endif /* QUPLAYLISTITEM_H_ */
