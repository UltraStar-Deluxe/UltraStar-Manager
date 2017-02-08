#ifndef QUSONGLINEDELEGATE_H
#define QUSONGLINEDELEGATE_H

#include <QItemDelegate>

#include "QU.h"

class QUSongLineDelegate: public QItemDelegate {
	Q_OBJECT

public:
	QUSongLineDelegate(QObject *parent = 0);

	QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
};

#endif // QUSONGLINEDELEGATE_H
