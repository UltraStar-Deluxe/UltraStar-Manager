#include "QUFreeCoversRequestUrl.h"

#include "QU.h"
#include "QUStringSupport.h"
#include "QUSongSupport.h"
#include "QUSongInterface.h"

#include <QVariant>

QUFreeCoversRequestUrl::QUFreeCoversRequestUrl(const QString &host, const QStringList &properties, QUSongInterface *song): QURequestUrl(host, properties, song) {
	initPath();
}

QString QUFreeCoversRequestUrl::request() const {
	QString result = QString("http://%1/api/search/%2/Music+CD")
			.arg(host())
			.arg(QString(fixedPercentageEncoding(encodedPath())));

//	song()->log(tr("[freecovers - search] ") + result, QU::Help);

	return result;
}

void QUFreeCoversRequestUrl::initPath() {
	QStringList data;
	foreach(QString property, properties()) {
		if(QUSongSupport::availableCustomTags().contains(property, Qt::CaseInsensitive))
			data <<	(QUStringSupport::withoutAnyUmlaut(song()->customTag(property)));
		else
			data << (QUStringSupport::withoutAnyUmlaut(song()->property(property.toLower().toLocal8Bit().data()).toString()));
	}

	setPath(QString("%1").arg(data.join(" ").trimmed()));
}
