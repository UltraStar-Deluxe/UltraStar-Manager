#include "QUAmazonRequestUrl.h"

#include <QString>
#include <QChar>
#include <QPair>
#include <QList>
#include <QVariant>
#include <QMessageBox>

QUAmazonRequestUrl::QUAmazonRequestUrl(const QString &endpoint, const QString &artistProperty, const QString &titleProperty, QUSongFile *song): QUrl() {
	setUrl(endpoint);

	if(song)
		this->init(song, artistProperty, titleProperty);
}

QString QUAmazonRequestUrl::request() const {
	return QString("%1?%2").arg(path()).arg(QString(encodedQuery()));
}

void QUAmazonRequestUrl::init(QUSongFile *song, const QString &artistProperty, const QString &titleProperty) {
	setQueryDelimiters('=', '&');

	QList<QPair<QString, QString> > query;

	query << QPair<QString, QString>("Service",        "AWSECommerceService");
	query << QPair<QString, QString>("Version",        "2008-04-07");
	query << QPair<QString, QString>("Operation",      "ItemSearch");
	query << QPair<QString, QString>("AWSAccessKeyId", "0B4QDARQ6V77NXZAZD02");
	query << QPair<QString, QString>("ResponseGroup",  "Images");
	query << QPair<QString, QString>("SearchIndex",    "Music");

	if(QString::compare(artistProperty, NONE, Qt::CaseInsensitive) == 0)
		query << QPair<QString, QString>("Artist", "");
	else if(QUSongFile::customTags().contains(artistProperty, Qt::CaseInsensitive))
		query << QPair<QString, QString>("Artist", toPercentEncoding(QU::withoutAnyUmlaut(song->customTag(artistProperty))));
	else
		query << QPair<QString, QString>("Artist", toPercentEncoding(QU::withoutAnyUmlaut(song->property(artistProperty.toLower().toLocal8Bit().data()).toString())));

	if(QString::compare(titleProperty, NONE, Qt::CaseInsensitive) == 0)
		query << QPair<QString, QString>("Title", "");
	else if(QUSongFile::customTags().contains(titleProperty, Qt::CaseInsensitive))
		query << QPair<QString, QString>("Title", toPercentEncoding(QU::withoutAnyUmlaut(song->customTag(titleProperty))));
	else
		query << QPair<QString, QString>("Title", toPercentEncoding(QU::withoutAnyUmlaut(song->property(titleProperty.toLower().toLocal8Bit().data()).toString())));

	setQueryItems(query);
}
