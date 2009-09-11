#include "QURemoteImageDialog.h"

#include "QUSongItem.h"
#include "QUCoverGroup.h"
#include "QUProgressDialog.h"

QURemoteImageDialog::QURemoteImageDialog(const QList<QUSongItem*> &items, QWidget *parent): QDialog(parent) {
	setupUi(this);

	infoLbl->setText(tr("Choose a remote image source and hit the search button. If you get no results consider a configuration of the source. Only checked songs will be considered for downloading and setting. <b>Internet connection required.</b>"));

	connect(acceptBtn, SIGNAL(clicked()), this, SLOT(accept()));

	initResultsPage(items);
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

	foreach(QUSongItem *songItem, items) {
		dlg.update(QString("%1 - %2").arg(songItem->song()->artist()).arg(songItem->song()->title()));
		if(dlg.cancelled()) break;

		QUCoverGroup *group = new QUCoverGroup(songItem, this);
		l->addWidget(group);
		_groups.append(group);

		group->showCovers();
	}

	// create connections for control buttons
	connect(searchImagesBtn, SIGNAL(clicked()), this, SLOT(getCovers()));
	connect(checkAllBtn, SIGNAL(clicked()), this, SLOT(checkAllGroups()));
	connect(uncheckAllBtn, SIGNAL(clicked()), this, SLOT(uncheckAllGroups()));
}

void QURemoteImageDialog::copyAndSetCovers() {
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
}
