#include "QUTagItem.h"

#include <QFont>

QUTagItem::QUTagItem(const QIcon &icon, const QString &text): QTableWidgetItem(icon, text) {
	setFlags(Qt::ItemIsEnabled);

	QFont f(font());
	f.setBold(true);
	f.setPointSize(8);
	setFont(f);
}
