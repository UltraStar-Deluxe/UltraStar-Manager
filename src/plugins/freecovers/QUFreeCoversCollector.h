#ifndef QUFREECOVERSCOLLECTOR_H
#define QUFREECOVERSCOLLECTOR_H

#include "QUHttpCollector.h"

class QUFreeCoversSource;

class QUFreeCoversCollector: public QUHttpCollector {
	Q_OBJECT

public:
	QUFreeCoversCollector(QUSongInterface *song, QUFreeCoversSource *source = 0);

protected:
	virtual QURequestUrl* url() const;
	virtual void processSearchResults();
};

#endif // QUFREECOVERSCOLLECTOR_H
