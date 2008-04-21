#include "QUMonty.h"

#include <QFile>
#include <QSettings>
#include <QPixmap>
#include <QFileInfo>

QUMonty::QUMonty() {
	initMessages();
	initGenres();
	initLanguages();
}

void QUMonty::initMessages() {
	QFile f(":/txt/hints");
	
	if(f.open(QIODevice::ReadOnly | QIODevice::Text)) {
		messages = QString(f.readAll()).split("\n");
		f.close();
	}	
}

void QUMonty::initGenres() {
	QFile f(":/txt/genres");
	
	if(f.open(QIODevice::ReadOnly | QIODevice::Text)) {
		_genres = QString(f.readAll()).split("\n");
		f.close();
	}	
}

void QUMonty::initLanguages() {
	QFile f(":/txt/languages");
	
	if(f.open(QIODevice::ReadOnly | QIODevice::Text)) {
		_languages = QString(f.readAll()).split("\n");
		f.close();
	}	
}

QUMonty* QUMonty::_instance = 0;
QUMonty* QUMonty::instance() {
	if(_instance == 0)
		_instance = new QUMonty();
	
	return _instance;
}

QPixmap QUMonty::pic(QUMonty::Status status) {
	switch(status) {
	case QUMonty::normal:
		return QPixmap(QString(":/monty/normal.png"));
	case QUMonty::back:
		return QPixmap(QString(":/monty/back.png"));
	case QUMonty::happy:
		return QPixmap(QString(":/monty/happy.png"));
	case QUMonty::seated:
		return QPixmap(QString(":/monty/seated.png"));
	default:
		return QPixmap();
	}
}

QString QUMonty::welcomeMsg(int numberOfSongs) {
	if(numberOfSongs >= 0)
		songCount = numberOfSongs;
	
	QString welcomeStr("Hello! I am Monty the Mammoth. I will tell you some hints from time to time. Just press the button below and I will disappear for now.<br>"
				"<br>"
				"You have a nice collection of <b>%1 songs</b> there. Are they managed well yet?");
	
	return welcomeStr.arg(songCount);
}

void QUMonty::talk(QLabel *montyLbl, QLabel *msgLbl) {
	montyLbl->setPixmap(pic((QUMonty::Status)(qrand() % 4)));
	msgLbl->setText(messages[qrand() % messages.size()].arg(songCount));
}

bool QUMonty::autoSaveEnabled() const {
	QSettings settings;
	return settings.value("autoSave", QVariant(true)).toBool();
}

/*!
 * Extract a resource (e.g. an image/icon) to a subdirectory of the report output
 * path and return a relative path to that resource.
 * \param item The resource to extract (e.g. ":/accept.png")
 * \param dest Directory of the report, NOT the subdirectory for the image
 * \returns Relative local path to the resource.
 */
QString QUMonty::useImageFromResource(const QString &item, QDir dest) {
	if(!dest.cd("images")) {
		dest.mkdir("images");
		if(!dest.cd("images")) {
//			emit finished(tr("Subdirectory for images could not be created!"), QU::warning);
			return QString();
		}
	}
	
	QPixmap pixmap(item);
	QFileInfo fi(dest, QFileInfo(item).fileName());
	
	if(fi.exists()) {
		dest.cdUp();
		return dest.relativeFilePath(fi.filePath());		
	}
	
	if(!pixmap.save(fi.filePath())) {
//		emit finished(QString(tr("The resource file \"%1\" could NOT be saved.")).arg(fi.filePath()), QU::warning);
		return QString();
	}
	
//	emit finished(QString(tr("The resource file \"%1\" was extracted successfully.")).arg(fi.filePath()), QU::information);
	
	dest.cdUp();
	return dest.relativeFilePath(fi.filePath());
}
