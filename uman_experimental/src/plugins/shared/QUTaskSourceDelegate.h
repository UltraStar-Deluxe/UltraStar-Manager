#ifndef QUTASKSOURCEDELEGATE_H_
#define QUTASKSOURCEDELEGATE_H_

#include "QU.h"

#include <QItemDelegate>
#include <QComboBox>

class QUTaskSourceDelegate: public QItemDelegate {
	Q_OBJECT

public:
	QUTaskSourceDelegate(QObject *parent = 0);

	QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
	virtual void setItems(QComboBox *comboBox) const = 0;
};

#endif /*QUTASKSOURCEDELEGATE_H_*/
