#ifndef QUTAGITEM_H_
#define QUTAGITEM_H_

#include <QTableWidgetItem>
#include <QString>
#include <QIcon>

class QUTagItem: public QTableWidgetItem {
public:
	QUTagItem(const QIcon &icon, const QString &text);
};

#endif /*QUTAGITEM_H_*/
