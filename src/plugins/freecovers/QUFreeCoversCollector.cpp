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
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QVariant>

QUFreeCoversCollector::QUFreeCoversCollector(QUSongInterface *song, QUFreeCoversSource *source): QUHttpCollector(song, source) {}

QURequestUrl* QUFreeCoversCollector::url() const {
	return new QUFreeCoversRequestUrl(
			source()->host(),
			QStringList() << source()->songDataField("first") << source()->songDataField("second") << source()->songDataField("third"),
			song());
}

void QUFreeCoversCollector::processSearchResults() {
	song()->log(tr("[QUFreeCoversCollector] processSearchResults(), state() = ") + QString::number(state()), QU::Help);
	QDomDocument doc; doc.setContent(buffer()->data());
	QUFreeCoversResponse response(doc);
	song()->log(tr("[freecovers - result] ") + buffer()->data(), QU::Help);

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

	for(int i = 0; i < response.results().size() && i < source()->limit(); i++) {
		song()->log(tr("[albumartex - result] ") + "http://" + source()->host() + response.results().at(i).toString(), QU::Help);
		manager()->get(QNetworkRequest(QUrl(response.results().at(i).toString())));
	}
}

void QUFreeCoversCollector::processImageResults(QNetworkReply* reply) {
	song()->log(tr("[QUFreeCoversCollector] processImageResults(), state() = ") + QString::number(state()), QU::Help);
	QUrl url = reply->url();
	QFile *file = openLocalFile(source()->imageFolder(song()).filePath(QFileInfo(url.toString().remove("/").remove("preview0")).fileName()));

	if(file) {
		file->write(reply->readAll());
	}
}
