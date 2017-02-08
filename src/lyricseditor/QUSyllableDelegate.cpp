#include "QUSyllableDelegate.h"

#include <QLineEdit>

QUSyllableDelegate::QUSyllableDelegate(QObject *parent): QItemDelegate(parent) {}

QWidget* QUSyllableDelegate::createEditor(
		QWidget *parent,
		const QStyleOptionViewItem &/*option*/,
		const QModelIndex &index) const
{

	QLineEdit *editor = new QLineEdit(parent);
	QFont f("Lucida Console"); f.setPointSize(8);
	editor->setFont(f);

	editor->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	editor->setStyleSheet("QLineEdit {border: 0px; background-color: yellow}");

	return editor;
}

void QUSyllableDelegate::setEditorData(
		QWidget *editor,
		const QModelIndex &index) const
{
	QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);

	lineEdit->setText(index.data(Qt::EditRole).toString().replace(QObject::trUtf8(CHAR_UTF8_DOT), " "));
}

void QUSyllableDelegate::setModelData(
		QWidget *editor,
		QAbstractItemModel *model,
		const QModelIndex &index) const
{
	QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
	model->setData(index, lineEdit->text().replace(" ", QObject::trUtf8(CHAR_UTF8_DOT)), Qt::EditRole);
}
