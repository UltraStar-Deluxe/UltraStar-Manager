#include "QUAmazonRequestUrl.h"

#include "QU.h"
#include "QUSongInterface.h"
#include "QUSongSupport.h"
#include "QUStringSupport.h"

#include "hmac_sha2.h"

#include <QString>
#include <QChar>
#include <QPair>
#include <QList>
#include <QVariant>
#include <QDateTime>
#include <QSettings>

QUAmazonRequestUrl::QUAmazonRequestUrl(const QString &host, const QStringList &properties, QUSongInterface *song): QURequestUrl(host, properties, song) {
	initQuery();
}

QString QUAmazonRequestUrl::request() const {
	QString result = QString("http://%1/onca/xml?%2&Signature=")
			.arg(host())
			.arg(QString(fixedPercentageEncoding())) + QString(fixedPercentageEncoding(signWithSHA256(stringToSign()).toBase64().toPercentEncoding()));

//	song()->log(tr("[amazon - search] ") + result, QU::Help);

	return result;
}

void QUAmazonRequestUrl::initQuery() {
	if(properties().size() < 2)
		return;

	QList<QPair<QString, QString> > query;

	QSettings settings;
	query << QPair<QString, QString>("AWSAccessKeyId", settings.value("amazon/access key id").toString());

	if(QString::compare(properties().at(0), NONE, Qt::CaseInsensitive) == 0)
		query << QPair<QString, QString>("Artist", "");
	else if(QUSongSupport::availableCustomTags().contains(properties().at(0), Qt::CaseInsensitive))
		query << QPair<QString, QString>("Artist",
			(QUStringSupport::withoutAnyUmlaut(song()->customTag(properties().at(0)))));
	else
		query << QPair<QString, QString>("Artist",
			(QUStringSupport::withoutAnyUmlaut(song()->property(properties().at(0).toLower().toLocal8Bit().data()).toString())));

	query << QPair<QString, QString>("Operation",      "ItemSearch");
	query << QPair<QString, QString>("ResponseGroup",  "Images");
	query << QPair<QString, QString>("SearchIndex",    "Music");
	query << QPair<QString, QString>("Service",        "AWSECommerceService");
	query << QPair<QString, QString>("Timestamp",      QDateTime::currentDateTime().toUTC().toString(Qt::ISODate) + ".000Z");

	if(QString::compare(properties().at(1), NONE, Qt::CaseInsensitive) == 0)
		query << QPair<QString, QString>("Title", "");
	else if(QUSongSupport::availableCustomTags().contains(properties().at(1), Qt::CaseInsensitive))
		query << QPair<QString, QString>("Title",
			(QUStringSupport::withoutAnyUmlaut(song()->customTag(properties().at(1)))));
	else
		query << QPair<QString, QString>("Title",
			(QUStringSupport::withoutAnyUmlaut(song()->property(properties().at(1).toLower().toLocal8Bit().data()).toString())));

	query << QPair<QString, QString>("Version",        "2009-03-31");

	setQueryItems(query);
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
	QString secret = settings.value("amazon/secret access key").toString();

	unsigned char buf[SHA256_BLOCK_SIZE];
	hmac_sha256(
		(unsigned char*) secret.toLatin1().data(), secret.length(),
		(unsigned char*) text.toLatin1().data(), text.length(),
		buf, SHA256_BLOCK_SIZE);
             
	return QByteArray((const char*)buf, 32);
}
