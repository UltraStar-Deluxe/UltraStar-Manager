#ifndef QUMONTY_H_
#define QUMONTY_H_

#define monty (QUMonty::instance())

#include <QPixmap>
#include <QString>
#include <QLabel>

class QUMonty {
public:
	enum Status {
		normal = 0,
		seated = 1,
		back = 2,
		happy = 3
	};
	
	static QUMonty* instance();
		
	QPixmap pic(QUMonty::Status status = QUMonty::normal);
	QString welcomeMsg(int songCount = -1);
	void talk(QLabel *montyLbl, QLabel *msgLbl);
	
	void setSongCount(int c) { songCount = c; }
	
protected:
	QUMonty();
	
private:
	static QUMonty* _instance;

	int songCount;
	QStringList messages;
};

#endif /*QUMONTY_H_*/
