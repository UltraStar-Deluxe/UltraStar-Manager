#ifndef QUFIELDDELEGATE_H
#define QUFIELDDELEGATE_H

#include <QItemDelegate>

class QUFieldDelegate: public QItemDelegate {
	Q_OBJECT

public:
	enum EditMode {
		Text, // just a free text field
		Choice, // various states
		Switch // two states
	};

	enum UserDataRole {
		FieldName = Qt::UserRole,
		AllValues = Qt::UserRole + 1,
		SwitchValue = Qt::UserRole + 2,
		HelpText = Qt::UserRole + 3,
		HelpTopic = Qt::UserRole + 4
	};

	QUFieldDelegate(EditMode mode, QObject *parent = 0);

	QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
	EditMode _mode;
};

#endif // QUFIELDDELEGATE_H
