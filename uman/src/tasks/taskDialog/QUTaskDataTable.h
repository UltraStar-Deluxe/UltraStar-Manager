#ifndef QUTASKDATATABLE_H_
#define QUTASKDATATABLE_H_

#include "QU.h"

#include "QURenameTask.h"
#include "QUAudioTagTask.h"

#include <QTableWidget>
#include <QList>

class QUTaskDataTable: public QTableWidget {
	Q_OBJECT

public:
	QUTaskDataTable(QWidget *parent = 0);

public slots:
	void fillData(const QList<QUScriptData*> &dataList, QU::ScriptableTaskTypes type);

	void appendRow();
	void removeLastRow();
	void moveUpCurrentRow();
	void moveDownCurrentRow();

};

#endif /* QUTASKDATATABLE_H_ */
