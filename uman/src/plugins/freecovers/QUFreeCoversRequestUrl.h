#ifndef QUFREECOVERSREQUESTURL_H
#define QUFREECOVERSREQUESTURL_H

#include "QURequestUrl.h"

class QUFreeCoversRequestUrl: public QURequestUrl {
public:
	QUFreeCoversRequestUrl(const QString &host, const QStringList &properties, QUSongInterface *song);
	virtual QString request() const;

private:
	void initPath();
};

#endif // QUFREECOVERSREQUESTURL_H
