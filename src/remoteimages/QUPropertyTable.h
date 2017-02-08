#ifndef QUPROPERTYTABLE_H
#define QUPROPERTYTABLE_H

#include <QTableWidget>
#include <QVariant>
#include <QStringList>

#include "QU.h"
#include "QUFieldDelegate.h"

class QTableWidgetItem;
class QURemoteImageSource;

class QUPropertyTable: public QTableWidget {
	Q_OBJECT

public:
	QUPropertyTable(QURemoteImageSource *src, QWidget *parent = 0);

protected:
	QURemoteImageSource* source() const { return _imageSource; }

	void initKeyColumn();
	void initValueColumn();

	void initSeparator(const QString &text, int row);
	QTableWidgetItem* keyItem(const QString &text);
	void setValueItem(int row, const QString &key, const QVariant &value, QUFieldDelegate::EditMode mode, const QStringList &allValues = QStringList(), const QString &helpText = QString());

private slots:
	void processPropertyEditing(QTableWidgetItem*);
	void skipReadOnlyCells(int, int, int, int);

private:
	QURemoteImageSource *_imageSource;
};

#endif // QUPROPERTYTABLE_H
