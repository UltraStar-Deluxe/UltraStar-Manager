#ifndef QUTASKCONDITIONDELEGATE_H_
#define QUTASKCONDITIONDELEGATE_H_

#include <QItemDelegate>

class QUTaskConditionDelegate: public QItemDelegate {
	Q_OBJECT

public:
	QUTaskConditionDelegate(QObject *parent = 0);

	QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif /*QUTASKCONDITIONDELEGATE_H_*/
