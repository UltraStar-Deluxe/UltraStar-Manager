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
#include <QHttp>
#include <QVariant>

QUAlbumArtExCollector::QUAlbumArtExCollector(QUSongInterface *song, QUAlbumArtExImageSource *source): QUHttpCollector(song, source) {}

QURequestUrl* QUAlbumArtExCollector::url() const {
	return new QUAlbumArtExRequestUrl(
			source()->host(),
			QStringList() << source()->songDataField("first") << source()->songDataField("second") << source()->songDataField("third"),
			song());
}

void QUAlbumArtExCollector::processSearchResults() {
	QRegExp rx = QRegExp(";src=(.*)\" width=.*(\\d+)&times;(\\d+)\"");

	rx.setMinimal(true);
	rx.setCaseSensitivity(Qt::CaseInsensitive);

	QString text(buffer()->data());
	QStringList allUrls;
	QList<QPair<int, int> > resolutions;
	int pos = 0;

	while ((pos = rx.indexIn(text, pos)) != -1) {
		allUrls << rx.cap(1).trimmed().replace("%2F", "/");
		resolutions << QPair<int, int>(QVariant(rx.cap(2)).toInt(), QVariant(rx.cap(3)).toInt());
		pos += rx.matchedLength();
	}

	rx.setPattern("Images (\\d+)\\-(\\d+) of (\\d+)\\.");
	rx.indexIn(text);
	int from = QVariant(rx.cap(1)).toInt();
	int to = QVariant(rx.cap(2)).toInt();
	int last = QVariant(rx.cap(3)).toInt();

	handleOldDownloads();

	QStringList urls;
	QPair<int, int> maxResolution(
			QVariant(source()->customDataField(source()->customDataFields().at(0))).toInt(),
			QVariant(source()->customDataField(source()->customDataFields().at(1))).toInt());
	for(int i = 0; i < allUrls.size(); i++)
		if(resolutions.at(i).first <= maxResolution.first and
		   resolutions.at(i).second <= maxResolution.second)
			urls << allUrls.at(i);

	ignoredUrls = allUrls.size() - qMin(urls.size(), source()->limit()) + last - to;

	if(urls.isEmpty()) {
		setState(Idle);
		if(ignoredUrls > 0)
			communicator()->send(tr("No results, %1 ignored.").arg(ignoredUrls));
		else
			communicator()->send(tr("No results."));
		communicator()->send(QUCommunicatorInterface::Done);
		return;
	}

	setState(ImageRequest);

	for(int i = 0; i < urls.size() and i < source()->limit(); i++) {
		QFile *file = openLocalFile(source()->imageFolder(song()).filePath(QFileInfo(urls.at(i)).fileName()));

//		song()->log(tr("[albumartex - result] ") + "http://" + source()->host() + urls.at(i), QU::Help);

		if(file) {
			http()->setHost(source()->host());
			http()->get("http://" + source()->host() + urls.at(i), file);
		}
	}
}
