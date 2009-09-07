#include "QUPluginDelegate.h"

#include <QPainter>
#include <QFontMetrics>

QUPluginDelegate::QUPluginDelegate(QObject *parent): QItemDelegate(parent) {
	_fontForName.setFamily("Verdana");
	_fontForName.setPixelSize(14);
	_fontForName.setBold(true);

	_fontForVersion.setFamily("Verdana");
	_fontForVersion.setPixelSize(12);

	_fontForDescription.setFamily("Verdana");
	_fontForDescription.setPixelSize(12);

	_height = QFontMetrics(_fontForName).height() + QFontMetrics(_fontForDescription).height() + 5;
}


void QUPluginDelegate::paint(
		QPainter *painter,
		const QStyleOptionViewItem &option,
		const QModelIndex &index) const
{
	if(!index.isValid())
		return;

	setupPainterForName(painter);
	painter->drawText(option.rect, Qt::AlignTop, index.model()->data(index, Qt::DisplayRole).toString());

	setupPainterForVersion(painter);
	painter->drawText(option.rect, Qt::AlignRight, "1.0.0");

	setupPainterForDescription(painter);
	painter->drawText(option.rect.adjusted(0, -5, 0, -5), Qt::AlignBottom, "Description");

	drawFocus(painter, option, option.rect);
}

QSize QUPluginDelegate::sizeHint(
		const QStyleOptionViewItem &option,
		const QModelIndex &index ) const
{
	return QSize(0, _height);
}

void QUPluginDelegate::setupPainterForName(QPainter *painter) const {
	painter->setFont(_fontForName);
	painter->setBrush(Qt::black);
}

void QUPluginDelegate::setupPainterForVersion(QPainter *painter) const {
	painter->setFont(_fontForVersion);
	painter->setBrush(Qt::red);
}

void QUPluginDelegate::setupPainterForDescription(QPainter *painter) const {
	painter->setFont(_fontForDescription);
	painter->setBrush(Qt::black);
}
