#include "QUAbstractReportData.h"

QUAbstractReportData::QUAbstractReportData(QObject *parent): QObject(parent) {
	this->setIcon(QIcon());
	this->setDescription("");
	this->setToolTip("");
	this->setType(QU::text);
}
