#include "QUPluginDelegate.h"
#include "QUPluginModel.h"

#include <QPainter>
#include <QFontMetrics>
#include <QPixmap>
#include <QIcon>
#include <QPoint>

QUPluginDelegate::QUPluginDelegate(QObject *parent): QItemDelegate(parent) {
	_fontForName.setFamily("Verdana");
	_fontForName.setPixelSize(14);
	_fontForName.setBold(true);

	_fontForVersion.setFamily("Verdana");
	_fontForVersion.setPixelSize(10);

	_fontForDescription.setFamily("Verdana");
	_fontForDescription.setPixelSize(11);

	_margin = 5;
	_height = QFontMetrics(_fontForName).height() + QFontMetrics(_fontForDescription).height() + _margin + _margin;
}


void QUPluginDelegate::paint(
		QPainter *painter,
		const QStyleOptionViewItem &option,
		const QModelIndex &index) const
{
	if(!index.isValid())
		return;

	QPixmap p(index.model()->data(index, QUPluginModel::IconRole).value<QIcon>().pixmap(16, 16));
	painter->drawPixmap(option.rect.left() + _margin, option.rect.top() + _margin, p.width(), p.height(), p);

	setupPainterForName(painter);
	painter->drawText(
			option.rect.adjusted(_margin + p.width() + 5, _margin, 0, 0),
			Qt::AlignTop,
			index.model()->data(index, QUPluginModel::NameRole).toString());

	setupPainterForVersion(painter);
	painter->drawText(
			option.rect.adjusted(0, _margin, -_margin, 0),
			Qt::AlignRight,
			index.model()->data(index, QUPluginModel::VersionRole).toString());

	setupPainterForDescription(painter);
	painter->drawText(
			option.rect.adjusted(_margin + p.width() + 5, 0, 0, -_margin),
			Qt::AlignBottom,
			index.model()->data(index, QUPluginModel::DescriptionRole).toString());
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
