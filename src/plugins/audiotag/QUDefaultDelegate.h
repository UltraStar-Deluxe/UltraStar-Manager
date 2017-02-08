#ifndef QUDEFAULTDELEGATE_H_
#define QUDEFAULTDELEGATE_H_

#include "QU.h"

#include <QItemDelegate>

class QUDefaultDelegate: public QItemDelegate {
	Q_OBJECT

public:
	QUDefaultDelegate(QObject *parent = 0);

	QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif /* QUDEFAULTDELEGATE_H_ */
