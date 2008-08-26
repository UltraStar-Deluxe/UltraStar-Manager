#include "QUTaskTextDelegate.h"
#include "QU.h"

#include <QString>
#include <QVariant>
#include <QLineEdit>
#include <QModelIndex>

QUTaskTextDelegate::QUTaskTextDelegate(QObject *parent): QItemDelegate(parent) {
}

QWidget* QUTaskTextDelegate::createEditor(
		QWidget *parent,
		const QStyleOptionViewItem &/*option*/,
		const QModelIndex &/*index*/) const
{
	return new QLineEdit(parent);
}

void QUTaskTextDelegate::setEditorData(
		QWidget *editor,
		const QModelIndex &index) const
{
	QString value = index.model()->data(index, Qt::DisplayRole).toString();

	QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);

	if(QString::compare(value, N_A, Qt::CaseSensitive) == 0)
		value = "";

	lineEdit->setText(value.trimmed());
	lineEdit->selectAll();
}

void QUTaskTextDelegate::setModelData(
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

void QUTaskTextDelegate::updateEditorGeometry(
		QWidget *editor,
		const QStyleOptionViewItem &option,
		const QModelIndex &/*index*/) const
{
	editor->setGeometry(option.rect);
}
