#include "QUAmazonDialog.h"
#include "QUProgressDialog.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>

#include <QListWidget>
#include <QListWidgetItem>

#include <QIcon>
#include <QPixmap>
#include <QLocale>

#include <QSize>
#include <QScrollBar>

#include <QMessageBox>

QUAmazonDialog::QUAmazonDialog(const QList<QUSongItem*> &items, QWidget *parent): QDialog(parent) {
	setupUi(this);

	_contentLayout = new QVBoxLayout;
    _contentLayout->setSpacing(5);
    _contentLayout->setMargin(5);
    _contentLayout->setObjectName(QString::fromUtf8("contentLayout"));

    content->setLayout(_contentLayout);
    scrollArea->setWidget(content);

    connect(getCoversBtn, SIGNAL(clicked()), this, SLOT(getCovers()));
    connect(applyBtn, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(reject()));

    endpointCombo->addItem(tr("United States (amazon.com)"), "http://ecs.amazonaws.com/onca/xml");
	endpointCombo->addItem(tr("United Kingdom (amazon.co.uk)"), "http://ecs.amazonaws.co.uk/onca/xml");
    endpointCombo->addItem(tr("Germany (amazon.de)"), "http://ecs.amazonaws.de/onca/xml");
	endpointCombo->addItem(tr("Canada (amazon.ca)"), "http://ecs.amazonaws.ca/onca/xml");
    endpointCombo->addItem(tr("France (amazon.fr)"), "http://ecs.amazonaws.fr/onca/xml");
	endpointCombo->addItem(tr("Japan (amazon.jp)"), "http://ecs.amazonaws.jp/onca/xml");

	if(QLocale::system().name() == "de_DE")
		endpointCombo->setCurrentIndex(2);

	artistCombo->addItem(NONE);
	titleCombo->addItem(NONE);

	artistCombo->addItems(QUSongFile::tags()); artistCombo->setCurrentIndex(artistCombo->findText(ARTIST_TAG));
	titleCombo->addItems(QUSongFile::tags()); titleCombo->setCurrentIndex(titleCombo->findText(TITLE_TAG));

	this->createGroups(items);
}

void QUAmazonDialog::createGroups(const QList<QUSongItem*> &items) {
	qDeleteAll(_groups);
	_groups.clear();

	QUProgressDialog dlg(tr("Creating cover groups..."),items.size(), dynamic_cast<QWidget*>(this->parent()) ? dynamic_cast<QWidget*>(this->parent()) : this);
	dlg.setPixmap(":/types/cover.png");
	dlg.show();

	foreach(QUSongItem *songItem, items) {
		dlg.update(QString("%1 - %2").arg(songItem->song()->artist()).arg(songItem->song()->title()));
		if(dlg.cancelled()) break;

		QUCoverGroup *group = new QUCoverGroup(songItem, this);
		_contentLayout->addWidget(group);
		_groups.append(group);

		connect(group, SIGNAL(finished(const QString&, QU::EventMessageTypes)), this, SIGNAL(finished(const QString&, QU::EventMessageTypes)));
		group->showCovers();
	}
}

void QUAmazonDialog::getCovers() {
	foreach(QUCoverGroup *group, _groups) {
		if(group->group->isChecked())
			group->getCovers(
					endpointCombo->itemData(endpointCombo->currentIndex()).toString(),
					artistCombo->currentText(),
					titleCombo->currentText());
	}
}

void QUAmazonDialog::accept() {
	QUProgressDialog dlg(tr("Use covers for songs..."), _groups.size(), this, true);
	dlg.setPixmap(":/types/cover.png");
	dlg.show();

	foreach(QUCoverGroup *group, _groups) {
		dlg.update(group->group->title());
		if(dlg.cancelled()) break;

		if(group->group->isChecked())
			group->copyCoverToSongPath();
	}

	QDialog::accept();
}
