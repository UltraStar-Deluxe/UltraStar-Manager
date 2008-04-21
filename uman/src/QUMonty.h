#ifndef QUMONTY_H_
#define QUMONTY_H_

#define monty (QUMonty::instance())

#include <QPixmap>
#include <QString>
#include <QLabel>
#include <QDir>

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
	const QStringList& genres() const { return _genres; }
	const QStringList& languages() const { return _languages; }
	
	void setSongCount(int c) { songCount = c; }
	
	bool autoSaveEnabled() const;
	
	QString useImageFromResource(const QString &item, QDir dest);
	
protected:
	QUMonty();
	
private:
	static QUMonty* _instance;

	int songCount;
	QStringList messages;
	QStringList _genres;
	QStringList _languages;
	
	void initMessages();
	void initGenres();
	void initLanguages();
};

#endif /*QUMONTY_H_*/
