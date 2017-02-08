#ifndef QUPLUGINDELEGATE_H
#define QUPLUGINDELEGATE_H

#include <QItemDelegate>
#include <QFont>

class QPainter;

class QUPluginDelegate: public QItemDelegate {
	Q_OBJECT

public:
	QUPluginDelegate(QObject *parent = 0);
	virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index ) const;

private:
	QFont _fontForName;
	QFont _fontForVersion;
	QFont _fontForDescription;
	int _height;
	int _margin;

	void setupPainterForName(QPainter*) const;
	void setupPainterForVersion(QPainter*) const;
	void setupPainterForDescription(QPainter*) const;
};

#endif // QUPLUGINDELEGATE_H
