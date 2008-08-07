#ifndef QUPLAYLISTITEM_H_
#define QUPLAYLISTITEM_H_

#include "QU.h"
#include "QUSongFile.h"

#include <QListWidget>
#include <QListWidgetItem>

class QUPlayListItem: public QListWidgetItem {
public:
	QUPlayListItem(QListWidget *parent = 0);
};

#endif /* QUPLAYLISTITEM_H_ */
