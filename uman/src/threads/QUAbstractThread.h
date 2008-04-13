#ifndef QUABSTRACTTHREAD_H_
#define QUABSTRACTTHREAD_H_

#include <QThread>
#include <QString>

class QUAbstractThread: public QThread {
	Q_OBJECT
	
public:
	QUAbstractThread(QObject *parent = 0);
	
	virtual int count() const = 0; //!< \returns the number of possible "continued" signal emitations
	
signals:
	void continued(const QString &text);
};

#endif /*QUABSTRACTTHREAD_H_*/
