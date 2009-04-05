#include "QULyricsEdit.h"

#include <QFont>
#include <QSize>

QULyricsEdit::QULyricsEdit(QWidget *parent): QListWidget(parent) {
	_delegate = new QUSongLineDelegate(this);
	this->setItemDelegate(_delegate);

	QFont f("Lucida Console"); f.setPointSize(8);
	this->setFont(f);
	this->setSortingEnabled(false);

	connect(this, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(lineModified(QListWidgetItem*)));

	_showWhitespace = false;
}

void QULyricsEdit::setSong(QUSongFile *song) {
	_song = song;
	this->initLyrics();
}

void QULyricsEdit::initLyrics() {
	this->clear();
	disconnect(this, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(lineModified(QListWidgetItem*)));

	if(!_song)
		return;

	int index = 0;
	foreach(QUSongLineInterface *line, _song->loadMelody()) {
		QListWidgetItem *lineItem = new QListWidgetItem(line->toString());

		if(_showWhitespace)
			lineItem->setText(lineItem->text().replace(" ", QObject::trUtf8(CHAR_UTF8_DOT)));

		lineItem->setData(Qt::UserRole, line->syllables());
		lineItem->setData(Qt::UserRole + 1, index++); // to allow finden correct song line later
		lineItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable);
		lineItem->setSizeHint(QSize(1, 16));
		this->addItem(lineItem);
	}

	connect(this, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(lineModified(QListWidgetItem*)));
}

void QULyricsEdit::lineModified(QListWidgetItem *item) {
	QUSongLineInterface *line = _song->loadMelody().at(item->data(Qt::UserRole + 1).toInt());
	line->setSyllables(item->data(Qt::UserRole).toStringList());

	disconnect(this, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(lineModified(QListWidgetItem*)));
	item->setText(line->toString());

	if(_showWhitespace)
		item->setText(item->text().replace(" ", QObject::trUtf8(CHAR_UTF8_DOT)));
	connect(this, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(lineModified(QListWidgetItem*)));
}

void QULyricsEdit::setShowWhitespace(bool enabled) {
	_showWhitespace = enabled;

	disconnect(this, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(lineModified(QListWidgetItem*)));

	if(enabled)
		for(int row = 0; row < count(); row++)
			this->item(row)->setText(item(row)->text().replace(" ", QObject::trUtf8(CHAR_UTF8_DOT)));
	else
		for(int row = 0; row < count(); row++)
			this->item(row)->setText(item(row)->text().replace(QObject::trUtf8(CHAR_UTF8_DOT), " "));

	connect(this, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(lineModified(QListWidgetItem*)));
}

void QULyricsEdit::keyPressEvent (QKeyEvent *event) {
	if(state() == QAbstractItemView::EditingState && (event->key() == Qt::Key_Down || event->key() == Qt::Key_Up))
		event->ignore();
	else
		QListWidget::keyPressEvent(event);
}
