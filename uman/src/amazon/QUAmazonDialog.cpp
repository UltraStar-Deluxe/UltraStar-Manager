#include "QUAmazonDialog.h"
#include "QUProgressDialog.h"
#include "QUMessageBox.h"
#include "QUSongSupport.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>

#include <QListWidget>
#include <QListWidgetItem>

#include <QIcon>
#include <QPixmap>
#include <QLocale>

#include <QSize>
#include <QScrollBar>
#include <QSettings>

#include <QMessageBox>

QUAmazonDialog::QUAmazonDialog(const QList<QUSongItem*> &items, QWidget *parent): QDialog(parent) {
	setupUi(this);

	infoLbl->setText(tr("Choose valid local sources for the keywords <b>artist and title</b>. Only checked songs will be considered for downloading and setting. <b>Internet connection required.</b>"));

	_contentLayout = new QVBoxLayout;
    _contentLayout->setSpacing(10);
    _contentLayout->setMargin(5);
    _contentLayout->setObjectName(QString::fromUtf8("contentLayout"));

    content->setLayout(_contentLayout);
    scrollArea->setWidget(content);

    connect(getCoversBtn, SIGNAL(clicked()), this, SLOT(getCovers()));
    connect(applyBtn, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(reject()));
    connect(checkAllBtn, SIGNAL(clicked()), this, SLOT(checkAllGroups()));
    connect(uncheckAllBtn, SIGNAL(clicked()), this, SLOT(uncheckAllGroups()));

	endpointCombo->addItem(tr("United States (amazon.com)"), "http://ecs.amazonaws.com");
	endpointCombo->addItem(tr("United Kingdom (amazon.co.uk)"), "http://ecs.amazonaws.co.uk");
	endpointCombo->addItem(tr("Germany (amazon.de)"), "http://ecs.amazonaws.de");
	endpointCombo->addItem(tr("Canada (amazon.ca)"), "http://ecs.amazonaws.ca");
	endpointCombo->addItem(tr("France (amazon.fr)"), "http://ecs.amazonaws.fr");
	endpointCombo->addItem(tr("Japan (amazon.jp)"), "http://ecs.amazonaws.jp");

	if(QLocale::system().language() == QLocale::German)
		endpointCombo->setCurrentIndex(2);

	artistCombo->addItem(NONE);
	titleCombo->addItem(NONE);

	artistCombo->addItems(QUSongSupport::availableTags()); artistCombo->setCurrentIndex(artistCombo->findText(ARTIST_TAG));
	titleCombo->addItems(QUSongSupport::availableTags()); titleCombo->setCurrentIndex(titleCombo->findText(TITLE_TAG));

	limitCombo->addItems(QStringList() << "5" << "10");
	QSettings settings;

	limitCombo->setCurrentIndex(limitCombo->findText(settings.value("amazonLimit", "5").toString()));
	connect(limitCombo, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(setLimit(const QString&)));

	keepDownloadsChk->setCheckState(settings.value("keepDownloads", false).toBool() ? Qt::Checked : Qt::Unchecked);
	connect(keepDownloadsChk, SIGNAL(stateChanged(int)), this, SLOT(setKeepDownloads(int)));

	keyEdit->setText(settings.value("apaapi/key").toString());
	connect(keyEdit, SIGNAL(editingFinished()), this, SLOT(setKey()));
	connect(keyBtn, SIGNAL(clicked()), this, SLOT(showKeyHelp()));

	secretEdit->setText(settings.value("apaapi/secret").toString());
	connect(secretEdit, SIGNAL(editingFinished()), this, SLOT(setSecret()));
	connect(secretBtn, SIGNAL(clicked()), this, SLOT(showSecretHelp()));

	this->createGroups(items);
	getCoversBtn->setFocus();
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

		if(group->group->isChecked()) {
			if(deleteCurrentCoverChk->isChecked())
				group->deleteCurrentCover();

			group->copyCoverToSongPath();
		}
	}

	QDialog::accept();
}

void QUAmazonDialog::checkAllGroups() {
	foreach(QUCoverGroup *group, _groups) {
		group->group->setChecked(true);
	}
}

void QUAmazonDialog::uncheckAllGroups() {
	foreach(QUCoverGroup *group, _groups) {
		group->group->setChecked(false);
	}
}

void QUAmazonDialog::setLimit(const QString &limit) {
	QSettings settings;
	settings.setValue("amazonLimit", QVariant(limit));
}

void QUAmazonDialog::setKeepDownloads(int state) {
	QSettings settings;
	settings.setValue("keepDownloads", QVariant(state == Qt::Checked ? true : false));
}

void QUAmazonDialog::setKey() {
	QSettings settings;
	settings.setValue("apaapi/key", keyEdit->text());
}

void QUAmazonDialog::setSecret() {
	QSettings settings;
	settings.setValue("apaapi/secret", secretEdit->text());
}

void QUAmazonDialog::showKeyHelp() {
	QUMessageBox::question(this,
			tr("Access Key ID"),
			tr("<b>What is an 'Access Key ID'?</b><br><br>"
					"This public key identifies you as the party responsible for the request. It belongs to an Amazon Web Services Account. All free services require you to send this key along with the request.<br><br>"
					"You get your own key if you <a href=\"https://aws-portal.amazon.com/gp/aws/developer/registration/index.html\">create an AWS Account</a>."),
			QStringList() << ":/marks/accept.png" << "OK",
			330);
}

void QUAmazonDialog::showSecretHelp() {
	QUMessageBox::question(this,
			tr("Secret Access Key"),
			tr("<b>What is a 'Secret Access Key'?</b><br><br>"
					"This secret key is used to calculate a signature to include in requests to the Amazon Product Advertising API which requires this kind of authentication. It should be known only by you and AWS. That's why each user of a desktop application like this one needs his own key.<br><br>"
					"You get your own key if you <a href=\"https://aws-portal.amazon.com/gp/aws/developer/registration/index.html\">create an AWS Account</a>."),
			QStringList() << ":/marks/accept.png" << "OK",
			330);
}
