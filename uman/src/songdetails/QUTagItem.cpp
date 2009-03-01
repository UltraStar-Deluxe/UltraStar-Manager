#include "QUTagItem.h"

QUTagItem::QUTagItem(const QIcon &icon, const QString &text): QTableWidgetItem(icon, text) {
	this->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}
