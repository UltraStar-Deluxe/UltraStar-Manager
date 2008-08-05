#include "QURenameDataTable.h"
#include "QUSongFile.h"

#include "QURenameConditionDelegate.h"
#include "QURenameSourceDelegate.h"
#include "QURenameTextDelegate.h"

#include <QHeaderView>

QURenameDataTable::QURenameDataTable(QWidget *parent): QTableWidget(parent) {
	this->setColumnCount(3);

	this->setHorizontalHeaderLabels(QStringList() << tr("Condition") << tr("Source") << tr("Default"));
	this->horizontalHeader()->setResizeMode(0, QHeaderView::Interactive);
	this->horizontalHeader()->setResizeMode(1, QHeaderView::Interactive);
	this->horizontalHeader()->setResizeMode(2, QHeaderView::Stretch);

	this->horizontalHeaderItem(0)->setToolTip(tr("Placeholder will be replaced with<br>source data if condition is fullfilled."));
	this->horizontalHeaderItem(1)->setToolTip(tr("Static or dynamic source data for a placeholder."));
	this->horizontalHeaderItem(2)->setToolTip(tr("Simple text for static source data or a default value<br>if the dynamic source data is <b>not available</b>."));

	this->setItemDelegateForColumn(0, new QURenameConditionDelegate(this));
	this->setItemDelegateForColumn(1, new QURenameSourceDelegate(this));
	this->setItemDelegateForColumn(2, new QURenameTextDelegate(this));
}

void QURenameDataTable::appendRow() {
	this->setRowCount(this->rowCount() + 1);

	for(int column = 0; column < this->columnCount(); column++) {
		QTableWidgetItem *newItem = new QTableWidgetItem;

		     if(column == 0) newItem->setText(QUSongFile::availableConditions().first());
		else if(column == 1) newItem->setText(QUSongFile::availableSources().first());
		else
			newItem->setText(N_A);

		this->setItem(this->rowCount() - 1, column, newItem);
	}

	this->verticalHeader()->setResizeMode(this->rowCount() - 1, QHeaderView::Fixed);
	this->verticalHeader()->resizeSection(this->rowCount() - 1, 20);
}

void QURenameDataTable::removeLastRow() {
	this->setRowCount(this->rowCount() - 1);
}