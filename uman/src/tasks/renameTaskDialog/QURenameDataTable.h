#ifndef QURENAMEDATATABLE_H_
#define QURENAMEDATATABLE_H_

#include "QU.h"
#include "QURenameTask.h"

#include <QTableWidget>
#include <QList>

class QURenameDataTable: public QTableWidget {
	Q_OBJECT

public:
	QURenameDataTable(QWidget *parent = 0);

public slots:
	void fillData(const QList<QURenameData*> &dataList);

	void appendRow();
	void removeLastRow();
	void moveUpCurrentRow();
	void moveDownCurrentRow();

};

#endif /* QURENAMEDATATABLE_H_ */
