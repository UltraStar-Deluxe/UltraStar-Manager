#include "QUTaskConditionDelegate.h"
#include "QUScriptableTask.h"

#include <QComboBox>
#include <QString>
#include <QVariant>
#include <QTableWidgetItem>

QUTaskConditionDelegate::QUTaskConditionDelegate(QObject *parent): QItemDelegate(parent) {
}

QWidget* QUTaskConditionDelegate::createEditor(
		QWidget *parent,
		const QStyleOptionViewItem &/*option*/,
		const QModelIndex &/*index*/) const
{
	QComboBox *editor = new QComboBox(parent);
	editor->addItems(QUScriptableTask::availableConditions());
	editor->setItemData(0, Qt::darkGray, Qt::ForegroundRole); // should be the "true" entry
	editor->setEditable(false);

	return editor;
}

void QUTaskConditionDelegate::setEditorData(
		QWidget *editor,
		const QModelIndex &index) const
{
	QString value = index.model()->data(index, Qt::DisplayRole).toString();

	QComboBox *comboBox = static_cast<QComboBox*>(editor);
	comboBox->setCurrentIndex(comboBox->findText(value, Qt::MatchContains));
}

void QUTaskConditionDelegate::setModelData(
		QWidget *editor,
		QAbstractItemModel *model,
		const QModelIndex &index) const
{
	QComboBox *comboBox = static_cast<QComboBox*>(editor);

	QString value = comboBox->currentText();

	model->setData(index, QVariant(value), Qt::EditRole);
}

void QUTaskConditionDelegate::updateEditorGeometry(
		QWidget *editor,
		const QStyleOptionViewItem &option,
		const QModelIndex &/*index*/) const
{
	editor->setGeometry(option.rect);
}
