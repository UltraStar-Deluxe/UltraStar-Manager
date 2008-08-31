#ifndef QUCOVERLIST_H_
#define QUCOVERLIST_H_

#include "QUCoverModel.h"

#include <QListView>
#include <QString>

class QUCoverList: public QListView {
	Q_OBJECT

public:
	QUCoverList(QWidget *parent = 0);

	void addItem(const QString &filePath);
	void clear();

	QString currentFilePath();
};

#endif /* QUCOVERLIST_H_ */
