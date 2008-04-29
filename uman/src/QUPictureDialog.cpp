#include "QUPictureDialog.h"

#include <QPixmap>
#include <QIcon>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

QUPictureDialog::QUPictureDialog(const QString &file, QWidget *parent): QDialog(parent) {
	setupUi(this);
	
	QGraphicsScene *scene = new QGraphicsScene(gfx);
	scene->addPixmap(QPixmap(file));
	
	gfx->setScene(scene);
	gfx->centerOn(0, 0);
	
	this->setWindowFlags(this->windowFlags() | Qt::WindowMaximizeButtonHint);
	this->resize(300, 300);
	
	this->setWindowIcon(QIcon(":/types/picture.png"));
	this->setWindowTitle(file);
}
