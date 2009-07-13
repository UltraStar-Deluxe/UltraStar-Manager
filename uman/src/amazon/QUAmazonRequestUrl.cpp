#include "QUAmazonRequestUrl.h"
#include "QUSongSupport.h"
#include "QUStringSupport.h"
#include "QULogService.h"
#include "hmac_sha2.h"

#include <QString>
#include <QChar>
#include <QPair>
#include <QList>
#include <QVariant>
#include <QMessageBox>
#include <QDateTime>
#include <QSettings>

QUAmazonRequestUrl::QUAmazonRequestUrl(const QString &endpoint, const QString &artistProperty, const QString &titleProperty, QUSongFile *song): QUrl() {
	setUrl(endpoint);

	if(song)
		this->init(song, artistProperty, titleProperty);
}

QString QUAmazonRequestUrl::request() const {
	QString result = QString("http://%1/onca/xml?%2&Signature=")
			.arg(host())
			.arg(QString(fixedPercentageEncoding())) + QString(fixedPercentageEncoding(signWithSHA256(stringToSign()).toBase64().toPercentEncoding()));

	return result;
}

void QUAmazonRequestUrl::init(QUSongFile *song, const QString &artistProperty, const QString &titleProperty) {
	setQueryDelimiters('=', '&');

	QList<QPair<QString, QString> > query;

	QSettings settings;
	query << QPair<QString, QString>("AWSAccessKeyId", settings.value("apaapi/key").toString());

	if(QString::compare(artistProperty, NONE, Qt::CaseInsensitive) == 0)
		query << QPair<QString, QString>("Artist", "");
	else if(QUSongSupport::availableCustomTags().contains(artistProperty, Qt::CaseInsensitive))
		query << QPair<QString, QString>("Artist", (QUStringSupport::withoutAnyUmlaut(song->customTag(artistProperty))));
	else
		query << QPair<QString, QString>("Artist", (QUStringSupport::withoutAnyUmlaut(song->property(artistProperty.toLower().toLocal8Bit().data()).toString())));

	query << QPair<QString, QString>("Operation",      "ItemSearch");
	query << QPair<QString, QString>("ResponseGroup",  "Images");
	query << QPair<QString, QString>("SearchIndex",    "Music");
	query << QPair<QString, QString>("Service",        "AWSECommerceService");
	query << QPair<QString, QString>("Timestamp",      QDateTime::currentDateTime().toUTC().toString(Qt::ISODate) + ".000Z");

	if(QString::compare(titleProperty, NONE, Qt::CaseInsensitive) == 0)
		query << QPair<QString, QString>("Title", "");
	else if(QUSongSupport::availableCustomTags().contains(titleProperty, Qt::CaseInsensitive))
		query << QPair<QString, QString>("Title", (QUStringSupport::withoutAnyUmlaut(song->customTag(titleProperty))));
	else
		query << QPair<QString, QString>("Title", (QUStringSupport::withoutAnyUmlaut(song->property(titleProperty.toLower().toLocal8Bit().data()).toString())));

	query << QPair<QString, QString>("Version",        "2009-03-31");

	setQueryItems(query);
}

QByteArray QUAmazonRequestUrl::fixedPercentageEncoding() const {
	return fixedPercentageEncoding(encodedQuery());
}

QByteArray QUAmazonRequestUrl::fixedPercentageEncoding(QByteArray source) const {
	return source
		.replace(":", "%3A")
		.replace("/", "%2F")
		.replace("?", "%3F")
		.replace("#", "%23")
		.replace("[", "%5B")
		.replace("]", "%5D")
		.replace("@", "%40")
		.replace("+", "%20");
}

QString QUAmazonRequestUrl::stringToSign() const {
	QString result;
	result.append("GET\n");
	result.append(host() + "\n");
	result.append("/onca/xml\n");
	result.append(fixedPercentageEncoding());
	return result;
}

QByteArray QUAmazonRequestUrl::signWithSHA256(const QString &text) const {
	QSettings settings;
	QString secret = settings.value("apaapi/secret").toString();

	unsigned char buf[SHA256_BLOCK_SIZE];
	hmac_sha256(
		(unsigned char*) secret.toLatin1().data(), secret.length(),
		(unsigned char*) text.toLatin1().data(), text.length(),
		buf, SHA256_BLOCK_SIZE);
             
	return QByteArray((const char*)buf, 32);
}
