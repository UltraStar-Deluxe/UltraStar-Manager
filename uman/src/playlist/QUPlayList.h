#ifndef QUPLAYLIST_H_
#define QUPLAYLIST_H_

#include "QU.h"
#include "QUSongFile.h"
#include "QUPlayListItem.h"

#include <QDir>
#include <QListWidget>
#include <QListWidgetItem>

class QUPlayList: public QListWidget {
	Q_OBJECT

public:
	QUPlayList(QWidget *parent = 0);
};

#endif /* QUPLAYLIST_H_ */
