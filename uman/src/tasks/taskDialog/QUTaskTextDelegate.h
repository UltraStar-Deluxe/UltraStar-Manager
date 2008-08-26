#ifndef QUTASKTEXTDELEGATE_H_
#define QUTASKTEXTDELEGATE_H_

#include <QItemDelegate>

class QUTaskTextDelegate: public QItemDelegate {
	Q_OBJECT

public:
	QUTaskTextDelegate(QObject *parent = 0);

	QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif /*QUTASKTEXTDELEGATE_H_*/
