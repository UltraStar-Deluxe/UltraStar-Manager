#ifndef QUAMAZONAREA_H_
#define QUAMAZONAREA_H_

#include "QU.h"
#include "QUSongItem.h"
#include "QUCoverGroup.h"

#include <QWidget>
#include <QList>
#include <QString>
#include <QVBoxLayout>

#include "ui_QUAmazonArea.h"

class QUAmazonArea: public QWidget, private Ui::QUAmazonArea {
	Q_OBJECT

public:
	QUAmazonArea(QWidget *parent = 0);

	void setSongItems(const QList<QUSongItem*> &items);

signals:
	void finished(const QString &message, QU::EventMessageTypes type);

private:
	QVBoxLayout         *_contentLayout;
	QList<QUCoverGroup*> _groups;
	QList<QUSongItem*>   _songItems;
};

#endif /* QUAMAZONAREA_H_ */
