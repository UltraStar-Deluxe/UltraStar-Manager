#ifndef QUCOVERLIST_H_
#define QUCOVERLIST_H_

#include <QListWidget>
#include <QResizeEvent>

class QUCoverList: public QListWidget {
	Q_OBJECT

public:
	QUCoverList(QWidget *parent = 0);

	void addItem(QListWidgetItem *item);

private:
	void adjustMinimumHeight();
};

#endif /* QUCOVERLIST_H_ */
