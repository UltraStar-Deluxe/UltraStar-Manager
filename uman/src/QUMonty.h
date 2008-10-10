#ifndef QUMONTY_H_
#define QUMONTY_H_

#define monty (QUMonty::instance())

#include <QPixmap>
#include <QString>
#include <QLabel>
#include <QDir>

#include <QFileSystemWatcher>

class QUMonty {
public:
	~QUMonty();

	enum Status {
		normal = 0,
		seated = 1,
		back = 2,
		happy = 3
	};

	static QUMonty* instance();
	void initMessages(const QString &source);

	QPixmap pic(QUMonty::Status status = QUMonty::normal);
	QString welcomeMsg(int songCount = -1);

	void talk(QLabel *montyLbl, QLabel *msgLbl);
	void answer(QLabel *montyLbl, QLabel *msgLbl, const QString &question = QString(), bool prev = false);

	const QStringList& genres() const { return _genres; }
	const QStringList& languages() const { return _languages; }

	void setSongCount(int c) { songCount = c; }

	bool autoSaveEnabled() const;

	QString useImageFromResource(const QString &item, QDir dest);

	QFileSystemWatcher* watcher() const { return _watcher; }

protected:
	QUMonty();

private:
	static QUMonty* _instance;

	int songCount;
	QStringList messages;
	QStringList _genres;
	QStringList _languages;

	QFileSystemWatcher *_watcher;

	void initGenres();
	void initLanguages();
};

#endif /*QUMONTY_H_*/
