#ifndef QUSIMPLECOMMUNICATOR_H
#define QUSIMPLECOMMUNICATOR_H

#include "QUCommunicatorInterface.h"

class QUSimpleCommunicator: public QUCommunicatorInterface {
	Q_OBJECT

public:
	QUSimpleCommunicator(QObject *parent = 0);
	~QUSimpleCommunicator() {}

	virtual void send(SimpleMessage msg);
	virtual void send(const QString &msg);

signals:
	void done();
	void failed();
	void messageSent(const QString&);
};

#endif // QUSIMPLECOMMUNICATOR_H
