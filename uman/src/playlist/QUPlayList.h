#ifndef QUPLAYLIST_H_
#define QUPLAYLIST_H_

#include "QU.h"
#include "QUSongFile.h"

#include <QDir>
#include <QListWidget>
#include <QListWidgetItem>
#include <QSettings>

class QUPlayList: public QListWidget {
	Q_OBJECT

public:
	QUPlayList(QWidget *parent = 0);

	QDir dir() const;
	void setDir(const QDir &dir);
};

#endif /* QUPLAYLIST_H_ */
