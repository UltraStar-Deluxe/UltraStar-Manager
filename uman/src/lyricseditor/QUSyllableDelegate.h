#ifndef QUSYLLABLEDELEGATE_H
#define QUSYLLABLEDELEGATE_H

#include "QU.h"
#include <QItemDelegate>

class QUSyllableDelegate: public QItemDelegate {
	Q_OBJECT

public:
	QUSyllableDelegate(QObject *parent = 0);

	QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
};

#endif // QUSYLLABLEDELEGATE_H
