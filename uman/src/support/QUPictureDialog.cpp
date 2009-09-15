#include "QUPictureDialog.h"

#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>
#include <QFileInfo>
#include <QSettings>
#include <QVariant>

QUPictureDialog::QUPictureDialog(const QString &filePath, QWidget *parent): QDialog(parent), _filePath(filePath) {
	setupUi(this);

	QLabel *gfx = new QLabel(this);
	gfx->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	scrollArea->setWidget(gfx);

	this->setWindowFlags(this->windowFlags() | Qt::WindowMaximizeButtonHint);

	this->setWindowIcon(QIcon(":/types/picture.png"));

	connect(fitSizeBtn, SIGNAL(clicked()), this, SLOT(fitPicture()));
	connect(fullSizeBtn, SIGNAL(clicked()), this, SLOT(fullPicture()));

	fitSizeBtn->setShortcut(Qt::CTRL + Qt::Key_1);
	fullSizeBtn->setShortcut(Qt::CTRL + Qt::Key_2);

	this->fullPicture();

	if(gfx->pixmap()) {
		double ratio = (double)gfx->pixmap()->height() / (double)gfx->pixmap()->width();
		if((ratio <= 2.0) and (ratio >= 0.5))
			this->resize(initialWidth(), (int)(ratio * initialWidth()) + EXTRA_HEIGHT);
	}

	// show file information
	QString fileInfoTxt = tr("%4 [<b>Dimensions:</b>\t%1 x %2, \n<b>Size:</b>\t%3 KiB]");
	QFileInfo fi(filePath);

	fileInfoTxt = fileInfoTxt.arg(gfx->pixmap()->width());
	fileInfoTxt = fileInfoTxt.arg(gfx->pixmap()->height());
	fileInfoTxt = fileInfoTxt.arg(fi.size() / 1024., 0, 'f', 2);
	fileInfoTxt = fileInfoTxt.arg(filePath);

	infoLbl->setText(fileInfoTxt);
}

/*!
 * Shows everything of the picture.
 */
void QUPictureDialog::fitPicture() {
	QLabel *gfx = dynamic_cast<QLabel*>(scrollArea->widget());

	if(!gfx)
		return;

	QPixmap pixmap(_filePath);

	double viewportRatio = scrollArea->maximumViewportSize().width() / scrollArea->maximumViewportSize().height();
	double gfxRatio      = pixmap.width() / pixmap.height();

	if(viewportRatio >= gfxRatio)
		gfx->setPixmap(pixmap.scaledToHeight(scrollArea->maximumViewportSize().height(), Qt::SmoothTransformation));
	else
		gfx->setPixmap(pixmap.scaledToWidth(scrollArea->maximumViewportSize().width(), Qt::SmoothTransformation));
}

void QUPictureDialog::fullPicture() {
	QLabel *gfx = dynamic_cast<QLabel*>(scrollArea->widget());

	if(!gfx)
		return;

	gfx->setPixmap(QPixmap(_filePath));
}

int QUPictureDialog::initialWidth() const {
	return QSettings().value("pictureDialogWidth", "400").toInt();
}

void QUPictureDialog::saveInitialWidth() {
	QSettings().setValue("pictureDialogWidth", width());
}

void QUPictureDialog::accept() {
	saveInitialWidth();
	QDialog::accept();
}
void QUPictureDialog::reject() {
	saveInitialWidth();
	QDialog::reject();
}
