#ifndef QRENAMETEXTDELEGATE_H_
#define QRENAMETEXTDELEGATE_H_

#include <QItemDelegate>

class QURenameTextDelegate: public QItemDelegate {
	Q_OBJECT

public:
	QURenameTextDelegate(QObject *parent = 0);

	QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif /*QRENAMETEXTDELEGATE_H_*/
