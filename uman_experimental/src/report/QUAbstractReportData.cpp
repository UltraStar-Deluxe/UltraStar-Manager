#include "QUAbstractReportData.h"

QUAbstractReportData::QUAbstractReportData(QObject *parent): QObject(parent), _next(0) {
	this->setIcon(QIcon());
	this->setDescription("");
	this->setToolTip("");
}
