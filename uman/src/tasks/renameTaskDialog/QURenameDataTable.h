#ifndef QURENAMEDATATABLE_H_
#define QURENAMEDATATABLE_H_

#include "QU.h"

#include <QTableWidget>
#include <QList>

class QURenameDataTable: public QTableWidget {
	Q_OBJECT

public:
	QURenameDataTable(QWidget *parent = 0);

public slots:
	void appendRow();
	void removeLastRow();
};

#endif /* QURENAMEDATATABLE_H_ */
