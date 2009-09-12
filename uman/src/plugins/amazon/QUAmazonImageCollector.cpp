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

QUAmazonImageCollector::QUAmazonImageCollector(QUSongInterface *song,QUAmazonImageSource *source): QObject(source) {
	_song = song;
	_source = source;
	initNetwork();
	setState(Idle);
}

void QUAmazonImageCollector::clearBuffer() {
	if(_buffer)
		delete _buffer;
	_buffer = new QBuffer(this);
}

void QUAmazonImageCollector::initNetwork() {
	_http = new QHttp(this);
	connect(_http, SIGNAL(stateChanged(int)), this, SLOT(processNetworkStateChange(int)));
	connect(_http, SIGNAL(done(bool)), this, SLOT(processNetworkOperationDone(bool)));

	_buffer = 0;
}

void QUAmazonImageCollector::collect() {
	if(http()->hasPendingRequests() or state() != Idle) {
		song()->log(tr("Could not get covers for \"%1 - %2\". Http connection is busy.").arg(song()->artist()).arg(song()->title()), QU::Warning);
		return;
	}

	QUAmazonRequestUrl url(
			source()->host(),
			source()->songDataField("artist"),
			source()->songDataField("title"),
			song());

	setState(SearchRequest);

	clearBuffer();
	http()->setHost(url.host());
	http()->get(url.request(), buffer());
}

QFileInfoList QUAmazonImageCollector::results() const {
	return source()->imageFolder(song()).entryInfoList(QUSongSupport::allowedPictureFiles(), QDir::Files, QDir::Name);
}

void QUAmazonImageCollector::processNetworkStateChange(int state) {
	if(state == QHttp::Sending)
		communicator()->send(tr("Sending..."));
	else if(state == QHttp::Reading)
		communicator()->send(tr("Reading..."));
}

void QUAmazonImageCollector::processNetworkOperationDone(bool error) {
	int count = localFiles().size();
	closeLocalFiles();

	if(error) {
		setState(Idle);
		communicator()->send(http()->errorString());
		communicator()->send(QUCommunicatorInterface::Failed);
		return;
	}

	if(state() == SearchRequest)
		processSearchResults();
	else if(state() == ImageRequest)
		processImageResults(count);
}

QFile* QUAmazonImageCollector::openLocalFile(const QString &filePath) {
	QFile *file = new QFile(filePath, this);

	if(!file->open(QIODevice::WriteOnly | QIODevice::Truncate)) {
		communicator()->send(tr("Could not open local file: \"%1\"").arg(filePath));
		return 0;
	}

	_localFiles.append(file);
	return file;
}

void QUAmazonImageCollector::closeLocalFiles() {
	foreach(QFile* file, localFiles())
		file->close();

	qDeleteAll(_localFiles);
	_localFiles.clear();
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

	// remove old downloads, if necessary
	if(!source()->keepDownloads()) {
		QFileInfoList fiList = source()->imageFolder(song()).entryInfoList(QUSongSupport::allowedPictureFiles(), QDir::Files, QDir::Name);
		foreach(QFileInfo fi, fiList)
			QFile::remove(fi.filePath());
	}

	if(response.count() == 0) {
		setState(Idle);
		communicator()->send(tr("No results."));
		communicator()->send(QUCommunicatorInterface::Done);
		return;
	}

	setState(ImageRequest);

	for(int i = 0; i < response.count() and i < source()->limit(); i++) {
		QFile *file = openLocalFile(source()->imageFolder(song()).filePath(QFileInfo(response.url(i, QU::largeImage).path()).fileName()));

		if(file) {
			http()->setHost(response.url(i, QU::largeImage).host());
			http()->get(response.url(i, QU::largeImage).path(), file);
		}
	}
}

void QUAmazonImageCollector::processImageResults(int count) {
	setState(Idle);
	communicator()->send(tr("%1 results").arg(count));
	communicator()->send(QUCommunicatorInterface::Done);
}
