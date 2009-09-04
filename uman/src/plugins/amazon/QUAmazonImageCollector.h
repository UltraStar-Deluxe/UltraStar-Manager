#ifndef QUAMAZONIMAGECOLLECTOR_H
#define QUAMAZONIMAGECOLLECTOR_H

#include <QObject>

#include "QURemoteImageSourcePlugin.h"

class QUAmazonImageCollector: public QObject, public QURemoteImageCollector {
	Q_OBJECT
	Q_INTERFACES(QURemoteImageCollector)

public:
	QUAmazonImageCollector(QObject *parent = 0);

	virtual void setCommunicator(QUCommunicatorInterface*) {}
	virtual void collect() {}
	virtual QFileInfoList results() const { return QFileInfoList(); }
};

#endif // QUAMAZONIMAGECOLLECTOR_H
