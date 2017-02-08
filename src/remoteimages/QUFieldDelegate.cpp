#include "QUFieldDelegate.h"
#include "QUHelpButton.h"

#include <QLineEdit>
#include <QComboBox>

QUFieldDelegate::QUFieldDelegate(EditMode mode, QObject *parent):
		QItemDelegate(parent),
		_mode(mode)
{}

QWidget* QUFieldDelegate::createEditor(
		QWidget *parent,
		const QStyleOptionViewItem &/*option*/,
		const QModelIndex &index) const
{
	if(index.column() == 2) // column for help button
		return new QUHelpButton(parent);

	switch(_mode) {
	case Choice:
	case Switch:
		return new QComboBox(parent);
	case Text:
	default:
		return new QLineEdit(parent);
	}
}

void QUFieldDelegate::setEditorData(
		QWidget *editor,
		const QModelIndex &index) const
{
	if(index.column() == 2) { // column for help button
		QUHelpButton *btn = qobject_cast<QUHelpButton*>(editor);
		if(btn) {
			btn->setTopic(index.model()->data(index, HelpTopic).toString());
			btn->setText(index.model()->data(index, HelpText).toString());
		}
		return;
	}

	QString value = index.model()->data(index, Qt::DisplayRole).toString();
	if(_mode == Text) {
		QLineEdit *lineEdit = qobject_cast<QLineEdit*>(editor);
		lineEdit->setText(value);
		lineEdit->selectAll();
	} else if(_mode == Choice) {
		QComboBox *comboBox = qobject_cast<QComboBox*>(editor);
		comboBox->addItems(index.model()->data(index, AllValues).toStringList());
		comboBox->setCurrentIndex(comboBox->findText(value, Qt::MatchFixedString));
	} else if(_mode == Switch) {
		QComboBox *comboBox = qobject_cast<QComboBox*>(editor);

		comboBox->addItems(index.model()->data(index, AllValues).toStringList());
		if(comboBox->count() != 2) {
			comboBox->clear();
			comboBox->addItems(QStringList() << tr("true") << tr("false"));
		}

		bool isTrue = comboBox->findText(index.model()->data(index, Qt::DisplayRole).toString(), Qt::MatchFixedString) == 0;
		comboBox->setCurrentIndex(isTrue ? 0 : 1);
	}
}

void QUFieldDelegate::setModelData(
		QWidget *editor,
		QAbstractItemModel *model,
		const QModelIndex &index) const
{
	if(index.column() == 2) // column for help button
		return;

	if(_mode == Text) {
		QLineEdit *lineEdit = qobject_cast<QLineEdit*>(editor);
		model->setData(index, lineEdit->text(), Qt::EditRole);
	} else if(_mode == Choice) {
		QComboBox *comboBox = qobject_cast<QComboBox*>(editor);
		model->setData(index, comboBox->currentText(), Qt::EditRole);
	} else if(_mode == Switch) {
		QComboBox *comboBox = qobject_cast<QComboBox*>(editor);
		model->setData(index, comboBox->currentText(), Qt::EditRole);
		model->setData(index, comboBox->currentIndex() == 0, SwitchValue);
	}
}

void QUFieldDelegate::updateEditorGeometry(
		QWidget *editor,
		const QStyleOptionViewItem &option,
		const QModelIndex &/*index*/) const
{
	editor->setGeometry(option.rect);
}
