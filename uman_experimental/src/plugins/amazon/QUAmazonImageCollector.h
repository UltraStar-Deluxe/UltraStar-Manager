#ifndef QUAMAZONIMAGECOLLECTOR_H
#define QUAMAZONIMAGECOLLECTOR_H

#include "QUHttpCollector.h"

class QUAmazonImageSource;

class QUAmazonImageCollector: public QUHttpCollector {
	Q_OBJECT

public:
	QUAmazonImageCollector(QUSongInterface *song, QUAmazonImageSource *source = 0);

protected:
	virtual QURequestUrl* url() const;
	virtual void processSearchResults();
};

#endif // QUAMAZONIMAGECOLLECTOR_H
