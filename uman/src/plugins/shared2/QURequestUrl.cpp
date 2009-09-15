#include "QURequestUrl.h"

QURequestUrl::QURequestUrl(const QString &host, const QStringList &properties, QUSongInterface *song): QUrl() {	
	_song = song;
	_properties = properties;

	// setup this URL instance
	setScheme("http");
	setHost(host);
	setQueryDelimiters('=', '&');
}

QByteArray QURequestUrl::fixedPercentageEncoding() const {
	return fixedPercentageEncoding(encodedQuery());
}

QByteArray QURequestUrl::fixedPercentageEncoding(QByteArray source) const {
	return source
		.replace(":", "%3A")
		.replace("/", "%2F")
		.replace("?", "%3F")
		.replace("#", "%23")
		.replace("[", "%5B")
		.replace("]", "%5D")
		.replace("@", "%40")
		.replace("+", "%20")
		.replace("!", "")
		.replace("'", "")
		.replace("(", "")
		.replace(")", "");
}
