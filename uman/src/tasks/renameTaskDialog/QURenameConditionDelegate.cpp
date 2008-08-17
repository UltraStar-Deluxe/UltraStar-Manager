#include "QURenameConditionDelegate.h"
#include "QUSongFile.h"

#include <QComboBox>
#include <QString>
#include <QVariant>
#include <QTableWidgetItem>

QURenameConditionDelegate::QURenameConditionDelegate(QObject *parent): QItemDelegate(parent) {
}

QWidget* QURenameConditionDelegate::createEditor(
		QWidget *parent,
		const QStyleOptionViewItem &/*option*/,
		const QModelIndex &/*index*/) const
{
	QComboBox *editor = new QComboBox(parent);

	editor->setEditable(false);

	return editor;
}

void QURenameConditionDelegate::setEditorData(
		QWidget *editor,
		const QModelIndex &index) const
{
	QString value = index.model()->data(index, Qt::DisplayRole).toString();

	QComboBox *comboBox = static_cast<QComboBox*>(editor);

	comboBox->addItems(QUSongFile::availableConditions());
	comboBox->setItemData(0, Qt::darkGray, Qt::ForegroundRole); // should be the "true" entry

	comboBox->setCurrentIndex(comboBox->findText(value, Qt::MatchContains));
}

void QURenameConditionDelegate::setModelData(
		QWidget *editor,
		QAbstractItemModel *model,
		const QModelIndex &index) const
{
	QComboBox *comboBox = static_cast<QComboBox*>(editor);

	QString value = comboBox->currentText();

	model->setData(index, QVariant(value), Qt::EditRole);
}

void QURenameConditionDelegate::updateEditorGeometry(
		QWidget *editor,
		const QStyleOptionViewItem &option,
		const QModelIndex &/*index*/) const
{
	editor->setGeometry(option.rect);
}
