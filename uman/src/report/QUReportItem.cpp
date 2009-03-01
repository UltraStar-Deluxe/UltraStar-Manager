#include "QUReportItem.h"

QUReportItem::QUReportItem(QUAbstractReportData *data, Qt::CheckState state):
	QListWidgetItem(),
	_data(data)
{
	this->setIcon(data->icon());
	this->setText(data->description());
	this->setToolTip(data->toolTip());
	
	this->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
	this->setCheckState(state);
}

QUReportItem::~QUReportItem() {
	delete _data;
}
