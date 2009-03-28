#include "QUSlideShowDialog.h"

#include <QImage>
#include <QSize>

QUSlideShowDialog::QUSlideShowDialog(QList<QUSongItem*> items, QWidget *parent): QDialog(parent) {
	setupUi(this);

	connect(acceptBtn, SIGNAL(clicked()), this, SLOT(accept()));
	connect(rejectBtn, SIGNAL(clicked()), this, SLOT(reject()));

	_items = items;

	foreach(QUSongItem *item, _items) {
		if(!item->song()->hasCover()) {
			_items.removeAll(item);
			continue;
		}
		pictureFlow->addSlide(QImage(item->song()->coverFileInfo().filePath()));
	}

	connect(pictureFlow, SIGNAL(centerIndexChanged(int)), this, SLOT(changeSelection(int)));
	connect(unlinkChk, SIGNAL(toggled(bool)), this, SLOT(changeUnlinkFlag(bool)));

	pictureFlow->setSlideSize(QSize(270, 270));
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
					 .arg(_items.at(index)->song()->title()));

	disconnect(unlinkChk, SIGNAL(toggled(bool)), this, SLOT(changeUnlinkFlag(bool)));
	unlinkChk->setChecked(_items.at(index)->data(COVER_COLUMN, Qt::UserRole + 1).toBool());
	connect(unlinkChk, SIGNAL(toggled(bool)), this, SLOT(changeUnlinkFlag(bool)));
}

void QUSlideShowDialog::changeUnlinkFlag(bool unlink) {
	if(pictureFlow->centerIndex() < 0 or pictureFlow->centerIndex() >= _items.size())
		return;

	_items.at(pictureFlow->centerIndex())->setData(COVER_COLUMN, Qt::UserRole + 1, unlink);
}

void QUSlideShowDialog::accept() {	
	foreach(QUSongItem *item, _items) {
		if(item->data(COVER_COLUMN, Qt::UserRole + 1).toBool()) { // unlink
			item->song()->setInfo(COVER_TAG, "");
			item->song()->save();
			item->update();
		}

		item->setData(COVER_COLUMN, Qt::UserRole + 1, false);
	}

	QDialog::accept();
}

void QUSlideShowDialog::reject() {
	foreach(QUSongItem *item, _items)
		item->setData(COVER_COLUMN, Qt::UserRole + 1, false);

	QDialog::reject();
}
