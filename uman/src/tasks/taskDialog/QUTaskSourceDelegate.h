#ifndef QUTASKSOURCEDELEGATE_H_
#define QUTASKSOURCEDELEGATE_H_

#include "QU.h"
#include "QURenameTask.h"
#include "QUAudioTagTask.h"

#include <QItemDelegate>

class QUTaskSourceDelegate: public QItemDelegate {
	Q_OBJECT

public:
	QUTaskSourceDelegate(QU::ScriptableTaskTypes type, QObject *parent = 0);

	QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
	QU::ScriptableTaskTypes _type;
};

#endif /*QUTASKSOURCEDELEGATE_H_*/
