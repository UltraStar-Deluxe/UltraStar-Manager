#include "QUDetailItem.h"
#include <QBrush>

QUDetailItem::QUDetailItem(const QString &text, const QString &tag, QUSongFile *song): 
	QTableWidgetItem(text),
	_tag(tag),
	_song(song) 
{
}

QUDetailItem::~QUDetailItem() {
	
}

void QUDetailItem::setFontColor(const QColor &color) {
	QBrush brush = foreground();
	brush.setColor(color);
	setForeground(brush);
}
