#include "QUAlbumArtExRequestUrl.h"

#include "QU.h"
#include "QUStringSupport.h"
#include "QUSongSupport.h"
#include "QUSongInterface.h"

#include <QVariant>
#include <QUrlQuery>
#include <QSettings>

QUAlbumArtExRequestUrl::QUAlbumArtExRequestUrl(const QString &host, const QStringList &properties, QUSongInterface *song): QURequestUrl(host, properties, song) {
	initQuery();
}

QString QUAlbumArtExRequestUrl::request() const {
	QString result = QString("http://%1/covers?%2")
			.arg(host())
			.arg(QString(fixedPercentageEncoding(this->query().toUtf8())));

	song()->log(tr("[albumartex - search] ") + result, QU::Help);

	return result;
}

void QUAlbumArtExRequestUrl::initQuery() {
	QUrlQuery query;

	QStringList data;
	foreach(QString property, properties()) {
		if(QUSongSupport::availableCustomTags().contains(property, Qt::CaseInsensitive))
			data <<	(QUStringSupport::withoutAnyUmlaut(song()->customTag(property)));
		else
			data << (QUStringSupport::withoutAnyUmlaut(song()->property(property.toLower().toLocal8Bit().data()).toString()));
	}

	QSettings settings;
	query.addQueryItem("q", data.join(" ").trimmed());
	query.addQueryItem("fltr", settings.value("albumartex/filter").toString());
	query.addQueryItem("sort", settings.value("albumartex/sort by").toString());
	query.addQueryItem("status", settings.value("albumartex/image status").toString());
	query.addQueryItem("size", settings.value("albumartex/image size").toString());
	this->setQuery(query);
}
