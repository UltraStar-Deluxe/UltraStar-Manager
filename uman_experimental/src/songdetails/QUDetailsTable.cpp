#include "main.h"
#include "QUDetailsTable.h"

#include "QUDropDownDelegate.h"
#include "QUTagItem.h"
#include "QUDetailItem.h"

#include <QStringList>
#include <QHeaderView>
#include <QFont>
#include <QMessageBox>
#include <QColor>

#include "QUSongSupport.h"

QUDetailsTable::QUDetailsTable(QWidget *parent): QTableWidget(parent) {
	this->setColumnCount(2);

	// setup headers
	this->verticalHeader()->hide();
	this->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);

	this->setHorizontalHeaderLabels(QStringList() << tr("Tag") << tr("Value"));
	this->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
	this->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
	this->horizontalHeader()->hide();

	// set value editor
	QUDropDownDelegate *comboDelegate = new QUDropDownDelegate(this);
	this->setItemDelegateForColumn(1, comboDelegate);

	// fix tab-stops
	connect(this, SIGNAL(currentCellChanged(int,int,int,int)), SLOT(skipReadOnlyCells(int, int, int, int)));

	// setup tag & value column
	this->reset();
}

void QUDetailsTable::initTagColumn() {
	this->initSeparator(tr("Information"), 0);
	this->setItem(1, 0, new QUTagItem(QIcon(":/types/font.png"), tr("Title")));
	this->setItem(2, 0, new QUTagItem(QIcon(":/types/user.png"), tr("Artist")));
	this->setItem(3, 0, new QUTagItem(QIcon(":/types/language.png"), tr("Language")));
	this->setItem(4, 0, new QUTagItem(QIcon(":/types/edition.png"), tr("Edition")));
	this->setItem(5, 0, new QUTagItem(QIcon(":/types/genre.png"), tr("Genre")));
	this->setItem(6, 0, new QUTagItem(QIcon(":/types/date.png"), tr("Year")));
	this->setItem(7, 0, new QUTagItem(QIcon(":/types/creator.png"), tr("Creator")));

	this->initSeparator(tr("Files"), 8);
	this->setItem(9, 0, new QUTagItem(QIcon(":/types/music.png"), tr("MP3")));
	this->setItem(10, 0, new QUTagItem(QIcon(":/types/cover.png"), tr("Cover")));
	this->setItem(11, 0, new QUTagItem(QIcon(":/types/background.png"), tr("Background")));
	this->setItem(12, 0, new QUTagItem(QIcon(":/types/film.png"), tr("Video")));

	this->initSeparator(tr("Control"), 13);
	this->setItem(14, 0, new QUTagItem(QIcon(":/types/videogap.png"), tr("Videogap")));
	this->setItem(15, 0, new QUTagItem(QIcon(":/types/start.png"), tr("Start")));
	this->setItem(16, 0, new QUTagItem(QIcon(":/types/end.png"), tr("End")));
	this->setItem(17, 0, new QUTagItem(QIcon(":/bullets/bullet_black.png"), tr("Relative")));
	this->setItem(18, 0, new QUTagItem(QIcon(":/bullets/bullet_black.png"), tr("BPM")));
	this->setItem(19, 0, new QUTagItem(QIcon(":/bullets/bullet_black.png"), tr("Gap")));

	this->initSeparator(tr("Custom"), 20);
	//this->setItem(21, 0, new QUTagItem(QIcon(":/types/comment.png"), tr("Comment")));
	int i = 0;
	foreach(QString customTag, QUSongSupport::availableCustomTags()) {
		this->setItem(21 + (i++), 0, new QUTagItem(QIcon(":/bullets/bullet_star.png"), customTag));
	}
}

void QUDetailsTable::initValueColumn() {
	/* separator here - skip a row */
	this->setItem(1, 1, new QUDetailItem(TITLE_TAG));
	this->setItem(2, 1, new QUDetailItem(ARTIST_TAG));
	this->setItem(3, 1, new QUDetailItem(LANGUAGE_TAG));
	this->setItem(4, 1, new QUDetailItem(EDITION_TAG));
	this->setItem(5, 1, new QUDetailItem(GENRE_TAG));
	this->setItem(6, 1, new QUDetailItem(YEAR_TAG));
	this->setItem(7, 1, new QUDetailItem(CREATOR_TAG));

	/* separator here - skip a row */
	this->setItem(9, 1, new QUDetailItem(MP3_TAG));
	this->setItem(10, 1, new QUDetailItem(COVER_TAG));
	this->setItem(11, 1, new QUDetailItem(BACKGROUND_TAG));
	this->setItem(12, 1, new QUDetailItem(VIDEO_TAG));

	/* separator here - skip a row */
	this->setItem(14, 1, new QUDetailItem(VIDEOGAP_TAG));
	this->setItem(15, 1, new QUDetailItem(START_TAG));
	this->setItem(16, 1, new QUDetailItem(END_TAG));
	this->setItem(17, 1, new QUDetailItem(RELATIVE_TAG));
	this->setItem(18, 1, new QUDetailItem(BPM_TAG));
	this->setItem(19, 1, new QUDetailItem(GAP_TAG));

	/* separator here - skip a row */
	int i = 0;
	foreach(QString customTag, QUSongSupport::availableCustomTags()) {
		this->setItem(21 + (i++), 1, new QUDetailItem(customTag));
	}
}

void QUDetailsTable::initSeparator(const QString &text, int row) {
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
	this->setSpan(row, 0, 1, 2);

	this->verticalHeader()->setResizeMode(row, QHeaderView::Fixed);
	this->verticalHeader()->resizeSection(row, 16);

}

void QUDetailsTable::updateValueColumn(const QList<QUSongItem*> &songItems) {
	for(int i = 0; i < this->rowCount(); i++) {
		QUDetailItem *detailItem = dynamic_cast<QUDetailItem*>(this->item(i, 1));

		if(detailItem)
			detailItem->setSongItems(songItems);
	}

	// disable user interaction if no song is selected
	this->setEnabled(!songItems.isEmpty());
}

/*!
 * Re-initializes all rows according to the actual custom tags.
 */
void QUDetailsTable::reset() {
	this->setRowCount(21 + QUSongSupport::availableCustomTags().size());

	// setup tag & value column
	this->initTagColumn();
	this->initValueColumn();
}

/*!
 * Workaround for tab-stop. Skip those cells which are not editable.
 */
void QUDetailsTable::skipReadOnlyCells(int row, int col, int, int) {
	if(currentItem() && currentItem()->flags().testFlag(Qt::ItemIsEditable))
		return;

	int nextCol = (col + 1) % columnCount();
	int nextRow = (row + (nextCol == 0 ? 1 : 0)) % rowCount();

	setCurrentCell(nextRow, nextCol);
	if(item(nextRow, nextCol) && !item(nextRow, nextCol)->flags().testFlag(Qt::ItemIsEnabled))
		skipReadOnlyCells(nextRow, nextCol, row, col);
}
