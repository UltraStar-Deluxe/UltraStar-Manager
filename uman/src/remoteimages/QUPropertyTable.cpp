#include "QUPropertyTable.h"
#include "QURemoteImageSourcePlugin.h"
#include "QUSongSupport.h"

#include <QHeaderView>
#include <QTableWidgetItem>
#include <QString>
#include <QStringList>
#include <QFont>

QUPropertyTable::QUPropertyTable(QURemoteImageSource *src, QWidget *parent):
		QTableWidget(parent),
		_imageSource(src)
{
	setColumnCount(3);
	setRowCount(6 + source()->songDataFields().size() + source()->customDataFields().size());

	// setup headers
	verticalHeader()->hide();
	verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);

	horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
	horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
	horizontalHeader()->setResizeMode(2, QHeaderView::ResizeToContents);
	horizontalHeader()->hide();

	connect(this, SIGNAL(currentCellChanged(int,int,int,int)), SLOT(skipReadOnlyCells(int, int, int, int)));
	connect(this, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(processPropertyEditing(QTableWidgetItem*)));

	setGridStyle(Qt::NoPen);

	initKeyColumn();
	initValueColumn();
}

void QUPropertyTable::initKeyColumn() {
	initSeparator(tr("General Information"), 0);
	setItem(1, 0, keyItem(tr("Host")));
	setItem(2, 0, keyItem(tr("Limit")));
	setItem(3, 0, keyItem(tr("Downloads")));

	initSeparator(tr("Search Request"), 4);
	int i = 5;
	foreach(QString songDataField, source()->songDataFields())
		setItem(i++, 0, keyItem(songDataField));

	initSeparator(tr("Custom Fields"), i++);
	foreach(QString customDataField, source()->customDataFields())
		setItem(i++, 0, keyItem(customDataField));
}

void QUPropertyTable::initValueColumn() {
	setValueItem(1, "host", source()->host(), QUFieldDelegate::Choice, source()->hosts());
	setValueItem(2, "limit", source()->limit(), QUFieldDelegate::Choice, QStringList() << "5" << "10" << "15");
	setValueItem(3, "downloads", source()->keepDownloads(), QUFieldDelegate::Switch, QStringList() << tr("Keep downloaded pictures.") << tr("Remove old downloads."));

	int i = 5;
	foreach(QString songDataField, source()->songDataFields())
		setValueItem(i++, songDataField, source()->songDataField(songDataField), QUFieldDelegate::Choice, QStringList() << NONE << QUSongSupport::availableTags());

	i++;
	foreach(QString customDataField, source()->customDataFields())
		setValueItem(i++, customDataField, source()->customDataField(customDataField), QUFieldDelegate::Text, QStringList(), source()->help(customDataField));
}

void QUPropertyTable::initSeparator(const QString &text, int row) {
	QTableWidgetItem *separator = new QTableWidgetItem(text);

	separator->setFlags(Qt::ItemIsEnabled);
	separator->setBackgroundColor(QColor(239, 239, 239));
	separator->setTextColor(QColor(134, 134, 134));
	separator->setTextAlignment(Qt::AlignCenter);

	QFont font(separator->font());
	font.setBold(true);
	font.setPointSize(8);
	separator->setFont(font);

	this->setItem(row, 0, separator);
	this->setSpan(row, 0, 1, 3);

	this->verticalHeader()->setResizeMode(row, QHeaderView::Fixed);
	this->verticalHeader()->resizeSection(row, 16);
}

QTableWidgetItem* QUPropertyTable::keyItem(const QString &text) {
	QTableWidgetItem *result = new QTableWidgetItem(text);

	result->setFlags(Qt::ItemIsEnabled);

	QFont f(font());
	f.setBold(true);
	f.setPointSize(8);
	result->setFont(f);

	return result;
}

void QUPropertyTable::setValueItem(
		int row,
		const QString &key,
		const QVariant &value,
		QUFieldDelegate::EditMode mode,
		const QStringList &allValues,
		const QString &helpText)
{
	QTableWidgetItem *result = new QTableWidgetItem;
	result->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);

	result->setData(QUFieldDelegate::FieldName, key);

	if(mode == QUFieldDelegate::Switch and allValues.size() == 2) {
		result->setData(Qt::DisplayRole, value.toBool() ? allValues.at(0) : allValues.at(1));
	} else {
		result->setData(Qt::DisplayRole, value);
	}

	result->setData(QUFieldDelegate::AllValues, allValues);

	setItem(row, 1, result);
	setItemDelegateForRow(row, new QUFieldDelegate(mode, this));

	// configure help button for column #2
	if(!helpText.isEmpty()) {
		QTableWidgetItem *helpItem = new QTableWidgetItem;
		helpItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
		helpItem->setData(QUFieldDelegate::HelpTopic, key);
		helpItem->setData(QUFieldDelegate::HelpText, helpText);

		setItem(row, 2, helpItem);
		openPersistentEditor(helpItem);
	} else {
		QTableWidgetItem *helpItem = new QTableWidgetItem;
		helpItem->setFlags(0);
		setItem(row, 2, helpItem);
	}
}

void QUPropertyTable::processPropertyEditing(QTableWidgetItem *item) {
	QString key(item->data(QUFieldDelegate::FieldName).toString());
	if(QString::compare(key, "host", Qt::CaseInsensitive) == 0)
		source()->setHost(item->data(Qt::EditRole).toString());
	else if(QString::compare(key, "limit", Qt::CaseInsensitive) == 0)
		source()->setLimit(item->data(Qt::EditRole).toInt());
	else if(QString::compare(key, "downloads", Qt::CaseInsensitive) == 0)
		source()->setKeepDownloads(item->data(QUFieldDelegate::SwitchValue).toBool());
	else if(source()->songDataFields().contains(key, Qt::CaseInsensitive))
		source()->setSongDataField(key, item->data(Qt::EditRole).toString());
	else if(source()->customDataFields().contains(key, Qt::CaseInsensitive))
		source()->setCustomDataField(key, item->data(Qt::EditRole).toString());
}

/*!
 * Workaround for tab-stop. Skip those cells which are not editable.
 */
void QUPropertyTable::skipReadOnlyCells(int row, int col, int, int) {
	if(currentItem() && currentItem()->flags().testFlag(Qt::ItemIsEditable))
		return;

	int nextCol = (col + 1) % columnCount();
	int nextRow = (row + (nextCol == 0 ? 1 : 0)) % rowCount();

	setCurrentCell(nextRow, nextCol);
	if(item(nextRow, nextCol) && !item(nextRow, nextCol)->flags().testFlag(Qt::ItemIsEnabled))
		skipReadOnlyCells(nextRow, nextCol, row, col);
}

