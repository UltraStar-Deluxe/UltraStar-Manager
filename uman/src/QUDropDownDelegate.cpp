#include "QUDropDownDelegate.h"
#include "QUDetailItem.h"

#include <QComboBox>
#include <QString>
#include <QVariant>
#include <QTableWidgetItem>
#include <QLineEdit>
#include <QCompleter>
#include <QSettings>

QUDropDownDelegate::QUDropDownDelegate(QObject *parent): QItemDelegate(parent) {
}

QWidget* QUDropDownDelegate::createEditor(
		QWidget *parent, 
		const QStyleOptionViewItem &/*option*/, 
		const QModelIndex &/*index*/) const 
{
	QComboBox *editor = new QComboBox(parent);
	
	editor->setEditable(true);
	editor->completer()->setCompletionMode(QCompleter::PopupCompletion);
	
	QSettings settings;
	if(settings.value("caseSensitiveAutoCompletion", QVariant(false)).toBool())
		editor->completer()->setCaseSensitivity(Qt::CaseSensitive);
	else
		editor->completer()->setCaseSensitivity(Qt::CaseInsensitive);
	
	return editor;
}

void QUDropDownDelegate::setEditorData(
		QWidget *editor, 
		const QModelIndex &index) const 
{
	QString value = index.model()->data(index, Qt::DisplayRole).toString();

	QComboBox *comboBox = static_cast<QComboBox*>(editor);
	
	comboBox->addItems(index.model()->data(index, Qt::UserRole).toStringList());
	
	if(value == "n/a")
		value = "";
	
	comboBox->setEditText(value);
	comboBox->lineEdit()->selectAll();
}

void QUDropDownDelegate::setModelData(
		QWidget *editor, 
		QAbstractItemModel *model, 
		const QModelIndex &index) const 
{
	QComboBox *comboBox = static_cast<QComboBox*>(editor);
	
	QString value = comboBox->currentText();

	model->setData(index, QVariant(value), Qt::EditRole);
}

void QUDropDownDelegate::updateEditorGeometry(
		QWidget *editor, 
		const QStyleOptionViewItem &option, 
		const QModelIndex &/*index*/) const 
{
	editor->setGeometry(option.rect);
}
