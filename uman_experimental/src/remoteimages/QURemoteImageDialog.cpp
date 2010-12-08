#include "QURemoteImageDialog.h"

#include "QUSongItem.h"
#include "QUCoverGroup.h"
#include "QUProgressDialog.h"
#include "QUPluginManager.h"
#include "QULogService.h"
#include "QUPropertyTable.h"

#include <QSettings>

QURemoteImageDialog::QURemoteImageDialog(const QList<QUSongItem*> &items, QWidget *parent): QDialog(parent) {
	setupUi(this);

	infoLbl->setText(tr("Choose a remote image source and hit the search button. If you get no results consider a configuration of the source. Only checked songs will be considered for downloading and setting. <b>Internet connection required.</b>"));

	connect(acceptBtn, SIGNAL(clicked()), this, SLOT(accept()));
	connect(searchImagesBtn, SIGNAL(clicked()), this, SLOT(getCovers()));
	connect(configurePluginBtn, SIGNAL(toggled(bool)), this, SLOT(toggleConfigurationPage(bool)));

	initImageSources();
	initResultsPage(items);

	// show last used page
	sourcesCombo->setCurrentIndex(qMax(0, qMin(QSettings().value("currentImageSource", 0).toInt(), sourcesCombo->count() - 1)));
}

void QURemoteImageDialog::initImageSources() {
	sourcesCombo->clear();
	foreach(QURemoteImageSource *src, pluginMGR->imageSourcePlugins()) {
		sourcesCombo->addItem(src->name());
		pageStack->addWidget(configurationPage(src));
	}

	if(sourcesCombo->count() > 0)
		sourcesCombo->setCurrentIndex(0);
	else {
		sourcesCombo->setEnabled(false);
		searchImagesBtn->setEnabled(false);
		configurePluginBtn->setEnabled(false);
		copyAndSetBtn->setEnabled(false);

		infoIconLbl->setPixmap(QPixmap(":/marks/error.png"));
		infoLbl->setText(tr("You need plugins to download images from the internet. Visit the <a href=\"http://sf.net/projects/uman\">project page</a> and try to get some."));
	}

	connect(sourcesCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(updateResultsPage()));
}

void QURemoteImageDialog::initResultsPage(const QList<QUSongItem*> &items) {
	// setup layout in scrolling area
	QVBoxLayout *l = new QVBoxLayout;
	l->setSpacing(10);
	l->setMargin(5);
	l->setObjectName(QString::fromUtf8("contentLayout"));
	content->setLayout(l);
	scrollArea->setWidget(content);

	// fill scrolling content with cover groups
	QUProgressDialog dlg(tr("Creating cover groups..."),items.size(), dynamic_cast<QWidget*>(this->parent()) ? dynamic_cast<QWidget*>(this->parent()) : this);
	dlg.setPixmap(":/types/cover.png");
	dlg.show();

	QList<QUSongInterface*> songs;
	foreach(QUSongItem *songItem, items)
		songs << songItem->song();

	QList<QURemoteImageCollector*> collectors;
	if(currentImageSource())
		collectors = currentImageSource()->imageCollectors(songs);
	else {
		logSrv->add(tr("No remote image sources selected."), QU::Warning);
		return;
	}

	if(collectors.size() != songs.size()) {
		logSrv->add(tr("Count of collectors and songs does not match! Plugin does not work as expected!"), QU::Error);
		return;
	} else if(songs.size() != _groups.size()) {
		if(!_groups.isEmpty()) {
			logSrv->add(tr("Size mismatch between groups and songs. Group size will be corrected."), QU::Warning);
			qDeleteAll(_groups);
			_groups.clear();
		}
	}

	bool createGroups = _groups.isEmpty();

	for(int i = 0; i < songs.size(); i++) {
		dlg.update(QString("%1 - %2").arg(songs.at(i)->artist()).arg(songs.at(i)->title()));
		if(dlg.cancelled()) break;

		if(createGroups) {
			QUCoverGroup *group = new QUCoverGroup(items.at(i), collectors.at(i), this);
			l->addWidget(group);
			_groups.append(group);
			group->showCovers();
		} else {
			_groups.at(i)->setCollector(collectors.at(i));
			_groups.at(i)->showCovers();
			_groups.at(i)->showStatus("");
		}
	}

	// create connections for control buttons
	connect(copyAndSetBtn, SIGNAL(clicked()), this, SLOT(copyAndSetCovers()));
	connect(checkAllBtn, SIGNAL(clicked()), this, SLOT(checkAllGroups()));
	connect(uncheckAllBtn, SIGNAL(clicked()), this, SLOT(uncheckAllGroups()));
}

void QURemoteImageDialog::updateResultsPage() {
	// fill scrolling content with cover groups
	QUProgressDialog dlg(tr("Updating cover groups..."),_groups.size(), dynamic_cast<QWidget*>(this->parent()) ? dynamic_cast<QWidget*>(this->parent()) : this);
	dlg.setPixmap(":/types/cover.png");
	dlg.show();

	QList<QUSongInterface*> songs;
	foreach(QUCoverGroup *group, _groups)
		songs << group->songItem()->song();

	QList<QURemoteImageCollector*> collectors;
	if(currentImageSource()) {
		collectors = currentImageSource()->imageCollectors(songs);
	} else {
		logSrv->add(tr("No remote image sources selected."), QU::Warning);
		return;
	}

	if(collectors.size() != songs.size()) {
		logSrv->add(tr("Count of collectors and songs does not match! Plugin does not work as expected!"), QU::Error);
		return;
	}

	for(int i = 0; i < songs.size(); i++) {
		dlg.update(QString("%1 - %2").arg(songs.at(i)->artist()).arg(songs.at(i)->title()));
		if(dlg.cancelled()) break;

		_groups.at(i)->setCollector(collectors.at(i));
		_groups.at(i)->showCovers();
		_groups.at(i)->showStatus("");
	}

	// switch to proper configuration page if needed
	toggleConfigurationPage(configurePluginBtn->isChecked());

	// remember this choice for next use
	QSettings().setValue("currentImageSource", sourcesCombo->currentIndex());
}

void QURemoteImageDialog::copyAndSetCovers() {
	QUProgressDialog dlg(tr("Use covers for songs..."), _groups.size(), this, true);
	dlg.setPixmap(":/types/cover.png");
	dlg.show();

	foreach(QUCoverGroup *group, _groups) {
		dlg.update(group->group->title());
		if(dlg.cancelled()) break;

		if(group->group->isChecked()) {
			group->copyCoverToSongPath(deleteCurrentCoverChk->isChecked());
		}
	}
}

void QURemoteImageDialog::checkAllGroups() {
	foreach(QUCoverGroup *group, _groups)
		group->group->setChecked(true);
}

void QURemoteImageDialog::uncheckAllGroups() {
	foreach(QUCoverGroup *group, _groups)
		group->group->setChecked(false);
}

void QURemoteImageDialog::getCovers() {
	configurePluginBtn->setChecked(false); // show the results page
	foreach(QUCoverGroup *group, _groups)
		group->getCovers();
}

QURemoteImageSource* QURemoteImageDialog::currentImageSource() const {
	if(sourcesCombo->currentIndex() < 0 or sourcesCombo->currentIndex() >= pluginMGR->imageSourcePlugins().size())
		return 0;

	return pluginMGR->imageSourcePlugins().at(sourcesCombo->currentIndex());
}

void QURemoteImageDialog::toggleConfigurationPage(bool enabled) {
	if(enabled)
		pageStack->setCurrentIndex(sourcesCombo->currentIndex() + 1);
	else
		pageStack->setCurrentIndex(0);
}

QWidget* QURemoteImageDialog::configurationPage(QURemoteImageSource *src) {
	return new QUPropertyTable(src);
}
