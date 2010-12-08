#include "QUTaskDataTable.h"

#include <QHeaderView>

QUTaskDataTable::QUTaskDataTable(QWidget *parent): QTableWidget(parent) {
	this->setColumnCount(3);

	this->setHorizontalHeaderLabels(QStringList() << tr("Condition") << tr("Source") << tr("Default"));
	this->horizontalHeader()->setResizeMode(CONDITION_COL, QHeaderView::Interactive);
	this->horizontalHeader()->setResizeMode(SOURCE_COL, QHeaderView::Interactive);
	this->horizontalHeader()->setResizeMode(DEFAULT_COL, QHeaderView::Stretch);

	this->horizontalHeaderItem(CONDITION_COL)->setToolTip(tr("Placeholder will be replaced with<br>source data if condition is fullfilled."));
	this->horizontalHeaderItem(SOURCE_COL)->setToolTip(tr("Static or dynamic source data for a placeholder."));
	this->horizontalHeaderItem(DEFAULT_COL)->setToolTip(tr("Simple text for static source data or a default value<br>if the dynamic source data is <b>not available</b>."));
}

/*!
 * Set delegates that allow a more convenient editing mode for the user.
 */
void QUTaskDataTable::setDelegates(QItemDelegate *conditionDelegate, QItemDelegate *sourceDelegate, QItemDelegate *defaultDelegate) {
	this->setItemDelegateForColumn(CONDITION_COL, conditionDelegate);
	this->setItemDelegateForColumn(SOURCE_COL, sourceDelegate);
	this->setItemDelegateForColumn(DEFAULT_COL, defaultDelegate);
}

void QUTaskDataTable::fillData(const QList<QUScriptData*> &dataList) {
	// fill the data
	foreach(QUScriptData *data, dataList) {
		this->appendRow();
		this->item(this->rowCount() - 1, CONDITION_COL)->setText(data->_if.isEmpty() ? "true" : data->_if);

		if(data->_keepSuffix) {
			this->item(this->rowCount() - 1, SOURCE_COL)->setText(KEEP_SUFFIX_SOURCE);
		} else if(!data->_text.isEmpty()) {
			this->item(this->rowCount() - 1, SOURCE_COL)->setText(TEXT_SOURCE);
			this->item(this->rowCount() - 1, DEFAULT_COL)->setText(data->_text);
		} else if(!data->_source.isEmpty()) {
			this->item(this->rowCount() - 1, SOURCE_COL)->setText(data->_source);
			this->item(this->rowCount() - 1, DEFAULT_COL)->setText(data->_default.isEmpty() ? N_A : data->_default);
		} else if(data->_keepUnknownTags) {
			this->item(this->rowCount() - 1, SOURCE_COL)->setText(UNKNOWN_TAGS_SOURCE);
		}

		if(!data->_ignoreEmpty.isEmpty()) // should only be set by audiotagtask
			this->item(this->rowCount() - 1, DEFAULT_COL)->setText(data->_ignoreEmpty);
	}
}

void QUTaskDataTable::appendRow() {
	this->setRowCount(this->rowCount() + 1);

	for(int column = 0; column < this->columnCount(); column++) {
		QTableWidgetItem *newItem = new QTableWidgetItem;

		     if(column == 0) newItem->setText(QUScriptableTask::availableConditions().first());
		else if(column == 1) newItem->setText(QUScriptableTask::availableSources().first());
		else
			newItem->setText(N_A);

		this->setItem(this->rowCount() - 1, column, newItem);
	}

	this->verticalHeader()->setResizeMode(this->rowCount() - 1, QHeaderView::Fixed);
	this->verticalHeader()->resizeSection(this->rowCount() - 1, 20);

	this->setCurrentCell(this->rowCount() - 1, this->currentColumn());
}

void QUTaskDataTable::removeLastRow() {
	this->setRowCount(this->rowCount() - 1);

	this->setCurrentCell(this->rowCount() - 1, this->currentColumn());
}

void QUTaskDataTable::moveUpCurrentRow() {
	if(this->currentRow() < 1)
		return; // always at first or empty

	for(int column = 0; column < this->columnCount(); column++) {
		QTableWidgetItem *currentItem = this->takeItem(this->currentRow(), column);
		QTableWidgetItem *upperItem = this->takeItem(this->currentRow() - 1, column);

		this->setItem(this->currentRow(), column, upperItem);
		this->setItem(this->currentRow() - 1, column, currentItem);
	}

	setCurrentCell(this->currentRow() - 1, this->currentColumn());
}

void QUTaskDataTable::moveDownCurrentRow() {
	if(this->currentRow() >= this->rowCount() - 1)
		return; // always at last

	for(int column = 0; column < this->columnCount(); column++) {
		QTableWidgetItem *currentItem = this->takeItem(this->currentRow(), column);
		QTableWidgetItem *lowerItem = this->takeItem(this->currentRow() + 1, column);

		this->setItem(this->currentRow(), column, lowerItem);
		this->setItem(this->currentRow() + 1, column, currentItem);
	}

	setCurrentCell(this->currentRow() + 1, this->currentColumn());
}
