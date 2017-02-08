#ifndef QUAMAZONREQUESTURL_H_
#define QUAMAZONREQUESTURL_H_

#include "QURequestUrl.h"

class QUAmazonRequestUrl: public QURequestUrl {
public:
	QUAmazonRequestUrl(const QString &host, const QStringList &properties, QUSongInterface *song);

	virtual QString request() const;

private:
	void initQuery();
	QString stringToSign() const;
	QByteArray signWithSHA256(const QString &text) const;
};

#endif /* QUAMAZONREQUESTURL_H_ */
