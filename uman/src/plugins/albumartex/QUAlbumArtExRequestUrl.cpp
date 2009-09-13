#include "QUAlbumArtExRequestUrl.h"

#include "QU.h"
#include "QUStringSupport.h"
#include "QUSongSupport.h"
#include "QUSongInterface.h"

#include <QVariant>

QUAlbumArtExRequestUrl::QUAlbumArtExRequestUrl(const QString &host, const QStringList &properties, QUSongInterface *song): QURequestUrl(host, properties, song) {
	initQuery();
}

QString QUAlbumArtExRequestUrl::request() const {
	QString result = QString("http://%1/covers.php?grid=4x5&%2")
			.arg(host())
			.arg(QString(fixedPercentageEncoding()));

	return result;
}

void QUAlbumArtExRequestUrl::initQuery() {
	QList<QPair<QString, QString> > query;

	QStringList data;
	foreach(QString property, properties()) {
		if(QUSongSupport::availableCustomTags().contains(property, Qt::CaseInsensitive))
			data <<	(QUStringSupport::withoutAnyUmlaut(song()->customTag(property)));
		else
			data << (QUStringSupport::withoutAnyUmlaut(song()->property(property.toLower().toLocal8Bit().data()).toString()));
	}

	query << QPair<QString, QString>("q", data.join(" ").trimmed());
	setQueryItems(query);
}
