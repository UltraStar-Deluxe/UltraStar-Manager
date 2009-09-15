#include "QUSlideShowDialog.h"

#include <QImage>
#include <QSize>
#include <QColorDialog>

QUSlideShowDialog::QUSlideShowDialog(QList<QUSongItem*> items, Modes m, QWidget *parent): QDialog(parent) {
	setupUi(this);

	connect(acceptBtn, SIGNAL(clicked()), this, SLOT(accept()));
	connect(rejectBtn, SIGNAL(clicked()), this, SLOT(reject()));

	_items = items;
	_mode = m;

	preparePictures();

	connect(pictureFlow, SIGNAL(centerIndexChanged(int)), this, SLOT(changeSelection(int)));
	connect(unlinkChk, SIGNAL(toggled(bool)), this, SLOT(changeUnlinkFlag(bool)));

	if(_mode == QUSlideShowDialog::coverflow) {
		textLbl->setText(tr("View the cover of each song and descide whether or not to <b>unlink</b> that cover from the song. Hit space. This helps you to <b>identify bad covers</b> with ease."));
		unlinkChk->setText(tr("Clear #COVER tag on OK"));

		pictureFlow->setSlideSize(QSize(270, 270));
	} else if(_mode == QUSlideShowDialog::backgroundflow) {
		textLbl->setText(tr("View the background picture of each song and descide whether or not to <b>unlink</b> that picture from the song. Hit space. This helps you to <b>identify bad ones</b> with ease."));
		unlinkChk->setText(tr("Clear #BACKGROUND tag on OK"));

		pictureFlow->setSlideSize(QSize(qRound(270 * (4/3.)), 270));
	}

	pictureFlow->setReflectionEffect(PictureFlow::PlainReflection);
	pictureFlow->showNext();
	pictureFlow->showPrevious();
	changeSelection(0);
}

void QUSlideShowDialog::keyPressEvent(QKeyEvent* event) {
	if(event->key() == Qt::Key_Space) {
		unlinkChk->toggle();
		event->accept();
	} else {
		QDialog::keyPressEvent(event);
	}
}

void QUSlideShowDialog::changeSelection(int index) {
	if(index < 0 or index >= _items.size())
		return;

	songLbl->setText(QString("%1<br><b>%2</b>")
					 .arg(_items.at(index)->song()->artist())
					 .arg(_items.at(index)->song()->titleCompact()));

	disconnect(unlinkChk, SIGNAL(toggled(bool)), this, SLOT(changeUnlinkFlag(bool)));
	unlinkChk->setChecked(_items.at(index)->data(COVER_COLUMN, Qt::UserRole + 1).toBool());
	connect(unlinkChk, SIGNAL(toggled(bool)), this, SLOT(changeUnlinkFlag(bool)));
}

void QUSlideShowDialog::changeUnlinkFlag(bool unlink) {
	if(pictureFlow->centerIndex() < 0 or pictureFlow->centerIndex() >= _items.size())
		return;

	if(_mode == QUSlideShowDialog::coverflow)
		_items.at(pictureFlow->centerIndex())->setData(COVER_COLUMN, Qt::UserRole + 1, unlink);
	else if(_mode == QUSlideShowDialog::backgroundflow)
		_items.at(pictureFlow->centerIndex())->setData(BACKGROUND_COLUMN, Qt::UserRole + 1, unlink);
}

void QUSlideShowDialog::accept() {	
	foreach(QUSongItem *item, _items) {
		if(_mode == QUSlideShowDialog::coverflow && item->data(COVER_COLUMN, Qt::UserRole + 1).toBool()) { // unlink
			item->song()->setInfo(COVER_TAG, "");
			item->song()->save();
			item->update();
		} else if(_mode == QUSlideShowDialog::backgroundflow && item->data(BACKGROUND_COLUMN, Qt::UserRole + 1).toBool()) { // unlink
			item->song()->setInfo(BACKGROUND_TAG, "");
			item->song()->save();
			item->update();
		}

		item->setData(COVER_COLUMN, Qt::UserRole + 1, false);
		item->setData(BACKGROUND_COLUMN, Qt::UserRole + 1, false);
	}

	QDialog::accept();
}

void QUSlideShowDialog::reject() {
	foreach(QUSongItem *item, _items) {
		item->setData(COVER_COLUMN, Qt::UserRole + 1, false);
		item->setData(BACKGROUND_COLUMN, Qt::UserRole + 1, false);
	}

	QDialog::reject();
}

void QUSlideShowDialog::preparePictures() {
	if(_mode == QUSlideShowDialog::coverflow) {
		foreach(QUSongItem *item, _items) {
			if(!item->song()->hasCover()) {
				_items.removeAll(item);
				continue;
			}
			pictureFlow->addSlide(QImage(item->song()->coverFileInfo().filePath()));
		}
	} else if(_mode == QUSlideShowDialog::backgroundflow) {
		foreach(QUSongItem *item, _items) {
			if(!item->song()->hasBackground()) {
				_items.removeAll(item);
				continue;
			}
			pictureFlow->addSlide(QImage(item->song()->backgroundFileInfo().filePath()));
		}
	}
}
