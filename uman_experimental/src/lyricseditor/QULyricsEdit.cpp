#include "QULyricsEdit.h"

#include <QFont>
#include <QSize>
#include <QHeaderView>
#include <QFontMetrics>

QULyricsEdit::QULyricsEdit(QWidget *parent): QTableWidget(parent) {
	_delegate = new QUSongLineDelegate(this);

	setColumnCount(2);
	setItemDelegateForColumn(1, _delegate);

	QFont f("Lucida Console"); f.setPointSize(8);
	this->setFont(f);
	this->setSortingEnabled(false);
	this->setShowGrid(false);

	connect(this, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(lineModified(QTableWidgetItem*)));

	// fix tab-stops
	connect(this, SIGNAL(currentCellChanged(int,int,int,int)), SLOT(skipReadOnlyCells(int, int, int, int)));

	_showWhitespace = false;

	verticalHeader()->hide();
	horizontalHeader()->hide();

//	horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
	setContentsMargins(0, 0, 0, 0);
	horizontalHeader()->setStretchLastSection(true);
//	horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
}

void QULyricsEdit::setSong(QUSongFile *song) {
	_song = song;
	this->initLyrics();
}

void QULyricsEdit::initLyrics() {
	disconnect(this, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(lineModified(QTableWidgetItem*)));
	this->clear();

	if(!_song)
		return;

	if(_song->length() >= 600)
		setColumnWidth(0, fontMetrics().width("19:59") + 8);
	else
		setColumnWidth(0, fontMetrics().width("9:59") + 8);

	this->setRowCount(_song->loadMelody().size());

	double bpm = QVariant(_song->bpm().replace(",", ".")).toDouble();
	double gap = QVariant(_song->gap().replace(",", ".")).toDouble() / 1000;

	int index = 0;
	int beats = 0;
	foreach(QUSongLineInterface *line, _song->loadMelody()) {
		QTableWidgetItem *lineItem = new QTableWidgetItem(line->toString());

		if(_showWhitespace)
			lineItem->setText(lineItem->text().replace(" ", QObject::trUtf8(CHAR_UTF8_DOT)));

		lineItem->setData(Qt::UserRole, line->syllables());
		lineItem->setData(Qt::UserRole + 1, index); // to allow find correct song line later
		lineItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable);
		lineItem->setSizeHint(QSize(1, 16));

		setItem(index, 1, lineItem);

		QTableWidgetItem *timeItem = new QTableWidgetItem();
		timeItem->setFlags(0);
		timeItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
		setItem(index, 0, timeItem);
		setRowHeight(index, 16);

		if(!line->notes().isEmpty() && bpm != 0.0) { // show timestamps
			if(!_song->isRelative()) {
				beats = line->notes().first()->timestamp();
			} else {
				beats += line->notes().first()->timestamp();
			}

			int seconds = qMax(0, (int)((beats / (bpm * 4)) * 60 + gap));
			timeItem->setText(QString("%1:%2")
				.arg(seconds / 60)
				.arg((int)(seconds % 60), 2, 10, QChar('0')));

			if(_song->isRelative())
				beats += line->inTime() - line->notes().first()->timestamp();
		}

		index++;
	}

	connect(this, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(lineModified(QTableWidgetItem*)));
}

void QULyricsEdit::lineModified(QTableWidgetItem *item) {
	QUSongLineInterface *line = _song->loadMelody().at(item->data(Qt::UserRole + 1).toInt());
	line->setSyllables(item->data(Qt::UserRole).toStringList());

	disconnect(this, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(lineModified(QTableWidgetItem*)));
	item->setText(line->toString());

	if(_showWhitespace)
		item->setText(item->text().replace(" ", QObject::trUtf8(CHAR_UTF8_DOT)));
	connect(this, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(lineModified(QTableWidgetItem*)));
}

void QULyricsEdit::setShowWhitespace(bool enabled) {
	_showWhitespace = enabled;

	disconnect(this, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(lineModified(QTableWidgetItem*)));

	if(enabled)
		for(int row = 0; row < rowCount(); row++)
			this->item(row, 1)->setText(item(row, 1)->text().replace(" ", QObject::trUtf8(CHAR_UTF8_DOT)));
	else
		for(int row = 0; row < rowCount(); row++)
			this->item(row, 1)->setText(item(row, 1)->text().replace(QObject::trUtf8(CHAR_UTF8_DOT), " "));

	connect(this, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(lineModified(QTableWidgetItem*)));
}

void QULyricsEdit::keyPressEvent (QKeyEvent *event) {
	if(state() == QAbstractItemView::EditingState && (event->key() == Qt::Key_Down || event->key() == Qt::Key_Up))
		event->ignore();
	else
		QTableWidget::keyPressEvent(event);
}

/*!
 * Workaround for tab-stop. Skip those cells which are not editable.
 */
void QULyricsEdit::skipReadOnlyCells(int row, int col, int, int) {
	if(currentItem() && currentItem()->flags().testFlag(Qt::ItemIsEditable))
		return;

	int nextCol = (col + 1) % columnCount();
	int nextRow = (row + (nextCol == 0 ? 1 : 0)) % rowCount();

	setCurrentCell(nextRow, nextCol);
	if(item(nextRow, nextCol) && !item(nextRow, nextCol)->flags().testFlag(Qt::ItemIsEnabled))
		skipReadOnlyCells(nextRow, nextCol, row, col);
}
