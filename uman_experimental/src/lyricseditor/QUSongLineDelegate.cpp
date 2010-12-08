#include "QUSongLineDelegate.h"
#include "QUSyllableDelegate.h"

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QFont>
#include <QSize>

#include <QTableView>

QUSongLineDelegate::QUSongLineDelegate(QObject *parent): QItemDelegate(parent) {}

QWidget* QUSongLineDelegate::createEditor(
		QWidget *parent,
		const QStyleOptionViewItem &/*option*/,
		const QModelIndex &index) const
{
	int cols = index.model()->data(index, Qt::UserRole).toStringList().size();

	QTableWidget *editor = new QTableWidget(1, cols, parent);
	for(int i = 0; i < cols; i++) {
		QTableWidgetItem *item = new QTableWidgetItem();
		QFont f("Lucida Console"); f.setPointSize(8);
		item->setFont(f);
		item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		editor->setItem(0, i, item);
	}

	editor->horizontalHeader()->hide();
	editor->verticalHeader()->hide();

	editor->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	editor->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	editor->verticalHeader()->setResizeMode(0, QHeaderView::Fixed);
	editor->verticalHeader()->resizeSection(0, 16);
	editor->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

	editor->setItemDelegate(new QUSyllableDelegate(editor));

	editor->setShowGrid(false);
	editor->setStyleSheet("QTableView {border: 0px; background-color: #cccccc}");

	return editor;
}

void QUSongLineDelegate::setEditorData(
		QWidget *editor,
		const QModelIndex &index) const
{
	QTableWidget *table = static_cast<QTableWidget*>(editor);

	int i = 0;
	foreach(QString syllable, index.model()->data(index, Qt::UserRole).toStringList())
		table->item(0, i++)->setText(syllable.replace(" ", QObject::trUtf8(CHAR_UTF8_DOT)));
}

void QUSongLineDelegate::setModelData(
		QWidget *editor,
		QAbstractItemModel *model,
		const QModelIndex &index) const
{
	QTableWidget *table = static_cast<QTableWidget*>(editor);

	QStringList syllables;
	for(int i = 0; i < table->columnCount(); i++)
		syllables << table->item(0, i)->text().replace(QObject::trUtf8(CHAR_UTF8_DOT), " ");

	model->setData(index, syllables, Qt::UserRole);
}
