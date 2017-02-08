#ifndef QUTASKMODIFIERDELEGATE_H_
#define QUTASKMODIFIERDELEGATE_H_

#include <QItemDelegate>

class QUTaskModifierDelegate: public QItemDelegate {
	Q_OBJECT

public:
	QUTaskModifierDelegate(QObject *parent = 0);

	QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif /*QUTASKMODIFIERDELEGATE_H_*/
