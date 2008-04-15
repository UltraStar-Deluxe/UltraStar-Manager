#ifndef QUABSTRACTJOB_H_
#define QUABSTRACTJOB_H_

#include <QObject>
#include <QString>

class QUAbstractJob: public QObject {
	Q_OBJECT
	
public:
	QUAbstractJob(QObject *parent = 0);
	
	virtual int count() const = 0; //!< \returns the number of possible "continued" signal emitations
	
signals:
	void continued(const QString &text);
};

#endif /*QUABSTRACTJOB_H_*/
