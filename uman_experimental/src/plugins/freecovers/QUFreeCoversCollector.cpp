#include "QUFreeCoversCollector.h"
#include "QUFreeCoversSource.h"
#include "QUFreeCoversRequestUrl.h"
#include "QUFreeCoversResponse.h"

#include "QUCommunicatorInterface.h"
#include "QUSongInterface.h"
#include "QUSongSupport.h"

#include <QFile>
#include <QBuffer>
#include <QRegExp>
#include <QTextStream>
#include <QHttp>
#include <QVariant>

QUFreeCoversCollector::QUFreeCoversCollector(QUSongInterface *song, QUFreeCoversSource *source): QUHttpCollector(song, source) {}

QURequestUrl* QUFreeCoversCollector::url() const {
	return new QUFreeCoversRequestUrl(
			source()->host(),
			QStringList() << source()->songDataField("first") << source()->songDataField("second") << source()->songDataField("third"),
			song());
}

void QUFreeCoversCollector::processSearchResults() {
	QDomDocument doc; doc.setContent(buffer()->data());
	QUFreeCoversResponse response(doc);

	handleOldDownloads();

	ignoredUrls = qMax(0, response.results().size() - source()->limit());

	if(response.results().isEmpty()) {
		setState(Idle);
		if(ignoredUrls > 0)
			communicator()->send(tr("No results, %1 ignored.").arg(ignoredUrls));
		else
			communicator()->send(tr("No results."));
		communicator()->send(QUCommunicatorInterface::Done);
		return;
	}

	setState(ImageRequest);

	for(int i = 0; i < response.results().size() and i < source()->limit(); i++) {
		QString fileName = response.results().at(i).path().remove("/").remove("preview0");
		QFile *file = openLocalFile(source()->imageFolder(song()).filePath(fileName));

//		song()->log(tr("[freecovers - result] ") + response.results().at(i).toString(), QU::Help);

		if(file) {
			http()->setHost(response.results().at(i).host());
			http()->get(response.results().at(i).toString(), file);
		}
	}
}
