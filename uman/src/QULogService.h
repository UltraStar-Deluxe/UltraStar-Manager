#ifndef QULOGSERVICE_H_
#define QULOGSERVICE_H_

#define logSrv (QULogService::instance())

#include "QU.h"
#include <QObject>

class QULogService: public QObject {
	Q_OBJECT

public:
	static QULogService* instance();

	void add(const QString &message, QU::EventMessageTypes type);

protected:
	QULogService(QObject *parent = 0);

signals:
	void messageAdded(const QString &message, QU::EventMessageTypes type);

private:
	static QULogService* _instance;
};

#endif /* QULOGSERVICE_H_ */
