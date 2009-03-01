#ifndef QDROPDOWNDELEGATE_H_
#define QDROPDOWNDELEGATE_H_

#include <QItemDelegate>

class QUDropDownDelegate: public QItemDelegate {
	Q_OBJECT
	
public:
	QUDropDownDelegate(QObject *parent = 0);
	
	QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif /*QDROPDOWNDELEGATE_H_*/
