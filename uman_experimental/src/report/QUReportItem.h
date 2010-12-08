#ifndef QUREPORTITEM_H_
#define QUREPORTITEM_H_

#include "QUAbstractReportData.h"

#include <QListWidgetItem>

class QUReportItem: public QListWidgetItem {
public:
	QUReportItem(QUAbstractReportData *data, Qt::CheckState state = Qt::Unchecked);
	~QUReportItem();
	
	QUAbstractReportData* data() const { return _data; }
	
private:
	QUAbstractReportData *_data;
};

#endif /*QUREPORTITEM_H_*/
