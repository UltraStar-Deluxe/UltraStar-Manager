#include "QUTaskSourceDelegate.h"

#include <QString>
#include <QVariant>
#include <QLineEdit>

QUTaskSourceDelegate::QUTaskSourceDelegate(QObject *parent): QItemDelegate(parent) {}

QWidget* QUTaskSourceDelegate::createEditor(
		QWidget *parent,
		const QStyleOptionViewItem &/*option*/,
		const QModelIndex &/*index*/) const
{
	QComboBox *editor = new QComboBox(parent);
	this->setItems(editor);
	editor->setEditable(false);

	return editor;
}

void QUTaskSourceDelegate::setEditorData(
		QWidget *editor,
		const QModelIndex &index) const
{
	QString value = index.model()->data(index, Qt::DisplayRole).toString();

	QComboBox *comboBox = static_cast<QComboBox*>(editor);

	comboBox->setCurrentIndex(comboBox->findText(value, Qt::MatchContains));
}

void QUTaskSourceDelegate::setModelData(
		QWidget *editor,
		QAbstractItemModel *model,
		const QModelIndex &index) const
{
	QComboBox *comboBox = static_cast<QComboBox*>(editor);
	model->setData(index, comboBox->currentText(), Qt::EditRole);
}

void QUTaskSourceDelegate::updateEditorGeometry(
		QWidget *editor,
		const QStyleOptionViewItem &option,
		const QModelIndex &/*index*/) const
{
	editor->setGeometry(option.rect);
}
