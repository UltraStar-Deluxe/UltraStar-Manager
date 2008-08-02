#ifndef QRENAMESOURCEDELEGATE_H_
#define QRENAMESOURCEDELEGATE_H_

#include <QItemDelegate>

class QURenameSourceDelegate: public QItemDelegate {
	Q_OBJECT

public:
	QURenameSourceDelegate(QObject *parent = 0);

	QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif /*QRENAMESOURCEDELEGATE_H_*/
