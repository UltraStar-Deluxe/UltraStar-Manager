#include "QUMonty.h"

#include <QFile>

QUMonty::QUMonty() {
	initMessages();
	initGenres();
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
