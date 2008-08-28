#include "QUAmazonArea.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>

#include <QListWidget>
#include <QListWidgetItem>

#include <QTreeView>
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include <QIcon>
#include <QPixmap>

#include <QVBoxLayout>
#include <QSize>
#include <QScrollBar>

#include <QMessageBox>

QUAmazonArea::QUAmazonArea(QWidget *parent): QWidget(parent) {
	setupUi(this);

	group1 = new QUCoverGroup("one", this);
	QUCoverGroup *group2 = new QUCoverGroup("two", this);
	QUCoverGroup *group3 = new QUCoverGroup("three", this);
	QUCoverGroup *group4 = new QUCoverGroup("four", this);

	QVBoxLayout *contentLayout = new QVBoxLayout;
    contentLayout->setSpacing(5);
    contentLayout->setMargin(5);
    contentLayout->setObjectName(QString::fromUtf8("contentLayout"));
    contentLayout->addWidget(group1);
    contentLayout->addWidget(group2);
    contentLayout->addWidget(group3);
    contentLayout->addWidget(group4);

    content->setLayout(contentLayout);

    scrollArea->setWidget(content);
}

void QUAmazonArea::blubb() {
	emit finished(QString("Viewport: %1 x %2")
			.arg(group1->list->viewport()->width())
			.arg(group1->list->viewport()->height()), QU::information);

	emit finished(QString("maximumViewportSize: %1 x %2")
			.arg(group1->list->maximumViewportSize().width())
			.arg(group1->list->maximumViewportSize().height()), QU::information);

	emit finished(QString("verticalScrollBar: max = %1 step = %2")
			.arg(group1->list->verticalScrollBar()->maximum())
			.arg(group1->list->verticalScrollBar()->singleStep()), QU::information);

	emit finished(QString("viewport.height + verticalScrollBar.maximum = %1")
			.arg(group1->list->viewport()->height() + group1->list->verticalScrollBar()->maximum()), QU::information);

}

void QUAmazonArea::foobar() {
	QDir covers(QCoreApplication::applicationDirPath()); covers.mkdir("covers"); covers.cd("covers");

	QFileInfoList picFiList = covers.entryInfoList(QU::allowedPictureFiles(), QDir::Files, QDir::Name);

	foreach(QFileInfo pic, picFiList) {
		QPixmap pixmap(pic.filePath());
		QListWidgetItem *newItem = new QListWidgetItem(QIcon(pixmap.scaledToWidth(COVER_ICON_WIDTH, Qt::SmoothTransformation)), QString("%1 x %2\nNew Line").arg(pixmap.width()).arg(pixmap.height()));

		newItem->setData(Qt::UserRole, pic.filePath());

		group1->list->addItem(newItem);
//		group2->addItem(new QListWidgetItem(*newItem));
//		group3->addItem(new QListWidgetItem(*newItem));
//		group4->addItem(new QListWidgetItem(*newItem));
	}
}
