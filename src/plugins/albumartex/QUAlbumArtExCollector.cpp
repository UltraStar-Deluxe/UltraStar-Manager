#include "QUAlbumArtExCollector.h"
#include "QUAlbumArtExImageSource.h"
#include "QUAlbumArtExRequestUrl.h"

#include "QUCommunicatorInterface.h"
#include "QUSongInterface.h"
#include "QUSongSupport.h"

#include <QFile>
#include <QBuffer>
#include <QRegExp>
#include <QTextStream>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QVariant>

QUAlbumArtExCollector::QUAlbumArtExCollector(QUSongInterface *song, QUAlbumArtExImageSource *source): QUHttpCollector(song, source) {}

QURequestUrl* QUAlbumArtExCollector::url() const {
	return new QUAlbumArtExRequestUrl(
			source()->host(),
			QStringList() << source()->songDataField("first") << source()->songDataField("second") << source()->songDataField("third"),
			song());
}

void QUAlbumArtExCollector::processSearchResults() {
	QRegExp rx = QRegExp("<div class=\"img-box\" style=\"background-image: url\\(/coverart/_tn/(.*)\\);\">(.*)<p class=\"image-info\"><span class=\"dimensions\">(\\d+)&times;(\\d+)</span>");

	rx.setMinimal(true);
	rx.setCaseSensitivity(Qt::CaseInsensitive);

	QString text(buffer()->data());
//	song()->log(tr("[albumartex - result] ") + text, QU::Help);
	QStringList urls;
	QList<QPair<int, int> > resolutions;
	int pos = 0;

	while ((pos = rx.indexIn(text, pos)) != -1) {
		urls << "/coverart/gallery/" + rx.cap(1).trimmed();
		resolutions << QPair<int, int>(QVariant(rx.cap(3)).toInt(), QVariant(rx.cap(4)).toInt());
		pos += rx.matchedLength();
	}

	/*
	rx.setPattern("Images (\\d+)\\-(\\d+) of (\\d+)\\.");
	rx.indexIn(text);
	int from = QVariant(rx.cap(1)).toInt();
	int to = QVariant(rx.cap(2)).toInt();
	int last = QVariant(rx.cap(3)).toInt();
	*/

	handleOldDownloads();

	if(urls.isEmpty()) {
		setState(Idle);
		communicator()->send(tr("No results."));
		communicator()->send(QUCommunicatorInterface::Done);
		return;
	}

	setState(ImageRequest);

	for(int i = 0; i < urls.size() and i < source()->limit(); i++) {
		QFile *file = openLocalFile(source()->imageFolder(song()).filePath(QFileInfo(urls.at(i)).fileName()));

		song()->log(tr("[albumartex - result] ") + "http://" + source()->host() + urls.at(i), QU::Help);

		if(file) {
			manager()->get(QNetworkRequest(QUrl("http://" + source()->host() + urls.at(i))));
			// FIX ME: this cannot be done here, because the request is not done yet!
			file->write(buffer()->data());
		}
	}
}
