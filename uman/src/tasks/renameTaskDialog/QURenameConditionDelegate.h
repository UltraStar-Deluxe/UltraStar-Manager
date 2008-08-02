#ifndef QRENAMECONDITIONDELEGATE_H_
#define QRENAMECONDITIONDELEGATE_H_

#include <QItemDelegate>

class QURenameConditionDelegate: public QItemDelegate {
	Q_OBJECT

public:
	QURenameConditionDelegate(QObject *parent = 0);

	QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif /*QRENAMECONDITIONDELEGATE_H_*/
