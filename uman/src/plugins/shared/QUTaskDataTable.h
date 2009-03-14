#ifndef QUTASKDATATABLE_H_
#define QUTASKDATATABLE_H_

#include "QU.h"
#include "QUScriptableTask.h"

#include <QTableWidget>
#include <QList>
#include <QItemDelegate>

#define CONDITION_COL 0
#define SOURCE_COL    1
#define DEFAULT_COL   2

class QUTaskDataTable: public QTableWidget {
	Q_OBJECT

public:
	QUTaskDataTable(QWidget *parent = 0);

	void setDelegates(QItemDelegate *conditionDelegate, QItemDelegate *sourceDelegate, QItemDelegate *defaultDelegate);
	void fillData(const QList<QUScriptData*> &dataList);

public slots:
	void appendRow();
	void removeLastRow();
	void moveUpCurrentRow();
	void moveDownCurrentRow();

};

#endif /* QUTASKDATATABLE_H_ */
