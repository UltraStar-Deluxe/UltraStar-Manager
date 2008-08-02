#include "QURenameTextDelegate.h"
#include "QUSongFile.h"

#include <QString>
#include <QVariant>
#include <QLineEdit>
#include <QModelIndex>

QURenameTextDelegate::QURenameTextDelegate(QObject *parent): QItemDelegate(parent) {
}

QWidget* QURenameTextDelegate::createEditor(
		QWidget *parent,
		const QStyleOptionViewItem &/*option*/,
		const QModelIndex &/*index*/) const
{
	return new QLineEdit(parent);
}

void QURenameTextDelegate::setEditorData(
		QWidget *editor,
		const QModelIndex &index) const
{
	QString value = index.model()->data(index, Qt::DisplayRole).toString();

	QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);

	lineEdit->setText(value.trimmed());

	if(QString::compare(value, N_A, Qt::CaseSensitive) == 0)
		value = "";

	lineEdit->selectAll();
}

void QURenameTextDelegate::setModelData(
		QWidget *editor,
		QAbstractItemModel *model,
		const QModelIndex &index) const
{
	QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);

	QString value = lineEdit->text();
	if(value == "")
		value = N_A;

	model->setData(index, QVariant(value), Qt::EditRole);
}

void QURenameTextDelegate::updateEditorGeometry(
		QWidget *editor,
		const QStyleOptionViewItem &option,
		const QModelIndex &/*index*/) const
{
	editor->setGeometry(option.rect);
}
