#ifndef QUPLAYLIST_H_
#define QUPLAYLIST_H_

#include "QU.h"
#include "QUSongFile.h"
#include "QUPlaylistFile.h"
#include "QUPlayListItem.h"

#include <QDir>
#include <QListWidget>
#include <QListWidgetItem>

class QUPlayList: public QListWidget {
	Q_OBJECT

public:
	QUPlayList(QWidget *parent = 0);

	void setItems(QUPlaylistFile *playlist);
	void appendItem(QUPlaylistEntry *entry);

public slots:
	void updateItems();

signals:
	void finished(const QString &message, QU::EventMessageTypes type);

};

#endif /* QUPLAYLIST_H_ */
