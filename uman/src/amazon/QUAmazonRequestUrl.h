#ifndef QUAMAZONREQUESTURL_H_
#define QUAMAZONREQUESTURL_H_

#include "QU.h"
#include "QUSongFile.h"

#include <QUrl>
#include <QByteArray>

class QUAmazonRequestUrl: public QUrl {
public:
	QUAmazonRequestUrl(const QString &endpoint, const QString &artistProperty, const QString &titleProperty, QUSongFile *song);

	QString request() const;
private:
	void init(QUSongFile *song, const QString &artistProperty, const QString &titleProperty);

	QByteArray fixedPercentageEncoding() const;
	QByteArray fixedPercentageEncoding(QByteArray source) const;
	QString stringToSign() const;
	QByteArray signWithSHA256(const QString &text) const;
};

#endif /* QUAMAZONREQUESTURL_H_ */
