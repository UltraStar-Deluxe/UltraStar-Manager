#include "QUSongItem.h"
#include <QBrush>

QUSongItem::QUSongItem(QUSongFile *song): QTreeWidgetItem(), _song(song) {
}

QUSongItem::~QUSongItem() {
	
}

void QUSongItem::setFontColor(const QColor &color) {
	QBrush brush = foreground(0);
	brush.setColor(color);
	setForeground(0, brush);
}
