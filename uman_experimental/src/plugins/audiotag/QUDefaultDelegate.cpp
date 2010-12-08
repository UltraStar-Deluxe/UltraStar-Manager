#include "QUDefaultDelegate.h"
#include "QUAudioTagTask.h"

#include <QComboBox>
#include <QString>
#include <QVariant>
#include <QLineEdit>

QUDefaultDelegate::QUDefaultDelegate(QObject *parent): QItemDelegate(parent) {
}

QWidget* QUDefaultDelegate::createEditor(
		QWidget *parent,
		const QStyleOptionViewItem &/*option*/,
		const QModelIndex &/*index*/) const
{
	QComboBox *editor = new QComboBox(parent);

	editor->addItems(QUAudioTagTask::availableSpecialSources());
	for(int i = 0; i < editor->count(); i++)
		editor->setItemData(i, Qt::darkGray, Qt::ForegroundRole);

	editor->setEditable(true);

	return editor;
}

void QUDefaultDelegate::setEditorData(
		QWidget *editor,
		const QModelIndex &index) const
{
	QString value = index.model()->data(index, Qt::DisplayRole).toString();

	QComboBox *comboBox = static_cast<QComboBox*>(editor);

	if(QString::compare(value, N_A, Qt::CaseSensitive) == 0)
		value = "";

	comboBox->setEditText(value);
	comboBox->lineEdit()->selectAll();
}

void QUDefaultDelegate::setModelData(
		QWidget *editor,
		QAbstractItemModel *model,
		const QModelIndex &index) const
{
	QComboBox *comboBox = static_cast<QComboBox*>(editor);

	QString value = comboBox->currentText();

	if(value == "")
		value = N_A;

	model->setData(index, QVariant(value), Qt::EditRole);
}

void QUDefaultDelegate::updateEditorGeometry(
		QWidget *editor,
		const QStyleOptionViewItem &option,
		const QModelIndex &/*index*/) const
{
	editor->setGeometry(option.rect);
}
