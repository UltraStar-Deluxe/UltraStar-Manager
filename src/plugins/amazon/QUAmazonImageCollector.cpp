#include "QUAmazonImageCollector.h"
#include "QUAmazonImageSource.h"

#include "QUCommunicatorInterface.h"
#include "QUSongInterface.h"

#include "QUSongSupport.h"

#include "QUAmazonRequestUrl.h"
#include "QUAmazonResponse.h"

#include <QHttp>
#include <QBuffer>
#include <QFile>
#include <QDomDocument>

QUAmazonImageCollector::QUAmazonImageCollector(QUSongInterface *song, QUAmazonImageSource *source): QUHttpCollector(song, source) {}

QURequestUrl* QUAmazonImageCollector::url() const {
	return new QUAmazonRequestUrl(
			source()->host(),
			QStringList() << source()->songDataField("artist") << source()->songDataField("title"),
			song());
}

void QUAmazonImageCollector::processSearchResults() {
	QDomDocument doc; doc.setContent(buffer()->data());
	QUAmazonResponse response(doc);

	if(!response.isValid()) {
		setState(Idle);
		communicator()->send(tr("Invalid request."));
		communicator()->send(QUCommunicatorInterface::Failed);
		return;
	}

	handleOldDownloads();

	ignoredUrls = qMax(0, response.count() - source()->limit());

	if(response.count() == 0) {
		setState(Idle);
		if(ignoredUrls > 0)
			communicator()->send(tr("No results, %1 ignored.").arg(ignoredUrls));
		else
			communicator()->send(tr("No results."));
		communicator()->send(QUCommunicatorInterface::Done);
		return;
	}

	setState(ImageRequest);

	for(int i = 0; i < response.count() and i < source()->limit(); i++) {
//		song()->log(tr("[amazon - result] ") + response.url(i, QU::largeImage).toString(), QU::Help);
		QFile *file = openLocalFile(source()->imageFolder(song()).filePath(QFileInfo(response.url(i, QU::largeImage).path()).fileName()));

		if(file) {
			http()->setHost(response.url(i, QU::largeImage).host());
			http()->get(response.url(i, QU::largeImage).toString(), file);
		}
	}
}
