#include "QUAmazonArea.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>

#include <QListWidget>
#include <QListWidgetItem>

#include <QIcon>
#include <QPixmap>


#include <QSize>
#include <QScrollBar>

#include <QMessageBox>

QUAmazonArea::QUAmazonArea(QWidget *parent): QWidget(parent) {
	setupUi(this);

	_contentLayout = new QVBoxLayout;
    _contentLayout->setSpacing(5);
    _contentLayout->setMargin(5);
    _contentLayout->setObjectName(QString::fromUtf8("contentLayout"));

    content->setLayout(_contentLayout);
    scrollArea->setWidget(content);
}

void QUAmazonArea::setSongItems(const QList<QUSongItem*> &items) {
	_songItems = items;

	if(!_groups.isEmpty()) {
		qDeleteAll(_groups);
		_groups.clear();
	}

	foreach(QUSongItem *songItem, _songItems) {
		QUCoverGroup *group = new QUCoverGroup(QString("%1 - %2").arg(songItem->song()->artist()).arg(songItem->song()->title()), this);
		group->hide();
		_contentLayout->addWidget(group);
		_groups.append(group);
	}
}

//void QUAmazonArea::foobar() {
//	QDir covers(QCoreApplication::applicationDirPath()); covers.mkdir("covers"); covers.cd("covers");
//
//	QFileInfoList picFiList = covers.entryInfoList(QU::allowedPictureFiles(), QDir::Files, QDir::Name);
//
//	foreach(QFileInfo pic, picFiList) {
//		QPixmap pixmap(pic.filePath());
//		QListWidgetItem *newItem = new QListWidgetItem(QIcon(pixmap.scaledToWidth(COVER_ICON_WIDTH, Qt::SmoothTransformation)), QString("%1 x %2\nNew Line").arg(pixmap.width()).arg(pixmap.height()));
//
//		newItem->setData(Qt::UserRole, pic.filePath());
//
//		group1->list->addItem(newItem);
////		group2->addItem(new QListWidgetItem(*newItem));
////		group3->addItem(new QListWidgetItem(*newItem));
////		group4->addItem(new QListWidgetItem(*newItem));
//	}
//}
