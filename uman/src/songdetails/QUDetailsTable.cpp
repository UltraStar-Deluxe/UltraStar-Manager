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
	unsigned row = 0;
	this->initSeparator(tr("Information"), row++);
	this->setItem(row++, 0, new QUTagItem(QIcon(":/control/encoding.png"),			tr("Encoding")));
	this->setItem(row++, 0, new QUTagItem(QIcon(":/types/font.png"),				tr("Title")));
	this->setItem(row++, 0, new QUTagItem(QIcon(":/types/user.png"),				tr("Artist")));
	this->setItem(row++, 0, new QUTagItem(QIcon(":/types/language.png"),			tr("Language")));
	this->setItem(row++, 0, new QUTagItem(QIcon(":/types/edition.png"),				tr("Edition")));
	this->setItem(row++, 0, new QUTagItem(QIcon(":/types/genre.png"),				tr("Genre")));
	this->setItem(row++, 0, new QUTagItem(QIcon(":/types/date.png"),				tr("Year")));
	this->setItem(row++, 0, new QUTagItem(QIcon(":/types/creator.png"),				tr("Creator")));
	this->setItem(row++, 0, new QUTagItem(QIcon(":/types/album.png"),				tr("Album")));
	this->setItem(row++, 0, new QUTagItem(QIcon(":/types/comment.png"),				tr("Comment")));

	this->initSeparator(tr("Files"), row++);
	this->setItem(row++, 0, new QUTagItem(QIcon(":/types/music.png"),				tr("MP3")));
	this->setItem(row++, 0, new QUTagItem(QIcon(":/types/cover.png"),				tr("Cover")));
	this->setItem(row++, 0, new QUTagItem(QIcon(":/types/background.png"),			tr("Background")));
	this->setItem(row++, 0, new QUTagItem(QIcon(":/types/video.png"),				tr("Video")));

	this->initSeparator(tr("Control"), row++);
	this->setItem(row++, 0, new QUTagItem(QIcon(":/types/videogap.png"),			tr("Videogap")));
	this->setItem(row++, 0, new QUTagItem(QIcon(":/types/start.png"),				tr("Start")));
	this->setItem(row++, 0, new QUTagItem(QIcon(":/types/end.png"),					tr("End")));
	this->setItem(row++, 0, new QUTagItem(QIcon(":/bullets/bullet_black.png"),		tr("Relative")));
	this->setItem(row++, 0, new QUTagItem(QIcon(":/bullets/bullet_black.png"),		tr("PreviewStart")));
	this->setItem(row++, 0, new QUTagItem(QIcon(":/types/medley_calc.png"),			tr("CalcMedley")));
	this->setItem(row++, 0, new QUTagItem(QIcon(":/types/medley_start_beat.png"),	tr("MedleyStartBeat")));
	this->setItem(row++, 0, new QUTagItem(QIcon(":/types/medley_end_beat.png"),		tr("MedleyEndBeat")));
	this->setItem(row++, 0, new QUTagItem(QIcon(":/types/bpm.png"),					tr("BPM")));
	this->setItem(row++, 0, new QUTagItem(QIcon(":/types/gap.png"),					tr("Gap")));

	this->initSeparator(tr("Duet"), row++);
	this->setItem(row++, 0, new QUTagItem(QIcon(":/types/P1.png"),					tr("P1")));
	this->setItem(row++, 0, new QUTagItem(QIcon(":/types/P2.png"),					tr("P2")));

	this->initSeparator(tr("Sorting"), row++);
	this->setItem(row++, 0, new QUTagItem(QIcon(":/types/font.png"),				tr("Title on Sorting")));
	this->setItem(row++, 0, new QUTagItem(QIcon(":/types/user.png"),				tr("Artist on Sorting")));

	if(!QUSongSupport::availableCustomTags().isEmpty()) {
		this->initSeparator(tr("Custom"), row++);
		foreach(QString customTag, QUSongSupport::availableCustomTags()) {
			this->setItem(row++, 0, new QUTagItem(QIcon(":/bullets/bullet_star.png"), customTag));
		}
	}
}

void QUDetailsTable::initValueColumn() {
	unsigned row = 0;
	/* separator here - skip a row */
	row++;
	this->setItem(row++, 1, new QUDetailItem(ENCODING_TAG));
	this->setItem(row++, 1, new QUDetailItem(TITLE_TAG));
	this->setItem(row++, 1, new QUDetailItem(ARTIST_TAG));
	this->setItem(row++, 1, new QUDetailItem(LANGUAGE_TAG));
	this->setItem(row++, 1, new QUDetailItem(EDITION_TAG));
	this->setItem(row++, 1, new QUDetailItem(GENRE_TAG));
	this->setItem(row++, 1, new QUDetailItem(YEAR_TAG));
	this->setItem(row++, 1, new QUDetailItem(CREATOR_TAG));
	this->setItem(row++, 1, new QUDetailItem(ALBUM_TAG));
	this->setItem(row++, 1, new QUDetailItem(COMMENT_TAG));

	/* separator here - skip a row */
	row++;
	this->setItem(row++, 1, new QUDetailItem(MP3_TAG));
	this->setItem(row++, 1, new QUDetailItem(COVER_TAG));
	this->setItem(row++, 1, new QUDetailItem(BACKGROUND_TAG));
	this->setItem(row++, 1, new QUDetailItem(VIDEO_TAG));

	/* separator here - skip a row */
	row++;
	this->setItem(row++, 1, new QUDetailItem(VIDEOGAP_TAG));
	this->setItem(row++, 1, new QUDetailItem(START_TAG));
	this->setItem(row++, 1, new QUDetailItem(END_TAG));
	this->setItem(row++, 1, new QUDetailItem(RELATIVE_TAG));
	this->setItem(row++, 1, new QUDetailItem(PREVIEWSTART_TAG));
	this->setItem(row++, 1, new QUDetailItem(CALCMEDLEY_TAG));
	this->setItem(row++, 1, new QUDetailItem(MEDLEYSTARTBEAT_TAG));
	this->setItem(row++, 1, new QUDetailItem(MEDLEYENDBEAT_TAG));
	this->setItem(row++, 1, new QUDetailItem(BPM_TAG));
	this->setItem(row++, 1, new QUDetailItem(GAP_TAG));

	/* separator here - skip a row */
	row++;
	this->setItem(row++, 1, new QUDetailItem(P1_TAG));
	this->setItem(row++, 1, new QUDetailItem(P2_TAG));

	/* separator here - skip a row */
	row++;
	this->setItem(row++, 1, new QUDetailItem(TITLEONSORTING_TAG));
	this->setItem(row++, 1, new QUDetailItem(ARTISTONSORTING_TAG));

	/* separator here - skip a row */
	row++;
	foreach(QString customTag, QUSongSupport::availableCustomTags()) {
		this->setItem(row++, 1, new QUDetailItem(customTag));
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
	for(int i = 0; i < this->rowCount(); ++i) {
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
	this->setRowCount(QUSongSupport::availableTags().size() + 6 + QUSongSupport::availableCustomTags().size());

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
