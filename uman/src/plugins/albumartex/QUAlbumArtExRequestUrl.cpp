#include "QUAlbumArtExRequestUrl.h"

QUAlbumArtExRequestUrl::QUAlbumArtExRequestUrl(const QString &host, const QStringList &properties, QUSongInterface *song): QURequestUrl(host, properties, song) {
}

QString QUAlbumArtExRequestUrl::request() const {
//	QString result = QString("http://%1/onca/xml?%2&Signature=")
//			.arg(host())
//			.arg(QString(fixedPercentageEncoding())) + QString(fixedPercentageEncoding(signWithSHA256(stringToSign()).toBase64().toPercentEncoding()));

	return "";
}
