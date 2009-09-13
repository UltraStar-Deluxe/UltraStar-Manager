#include "QUHttpCollector.h"
#include "QUMultilingualImageSource.h"
#include "QURequestUrl.h"

#include "QUCommunicatorInterface.h"
#include "QUSongInterface.h"

#include "QUSongSupport.h"

#include <QHttp>
#include <QBuffer>
#include <QFile>
#include <QDomDocument>

QUHttpCollector::QUHttpCollector(QUSongInterface *song, QUMultilingualImageSource *source): QObject(source) {
	_song = song;
	_source = source;
	_communicator = 0;
	ignoredUrls = 0;
	initNetwork();
	setState(Idle);
}

void QUHttpCollector::clearBuffer() {
	if(_buffer)
		delete _buffer;
	_buffer = new QBuffer(this);
}

void QUHttpCollector::initNetwork() {
	_http = new QHttp(this);
	connect(_http, SIGNAL(stateChanged(int)), this, SLOT(processNetworkStateChange(int)));
	connect(_http, SIGNAL(done(bool)), this, SLOT(processNetworkOperationDone(bool)));

	_buffer = 0;
}

void QUHttpCollector::collect() {
	if(http()->hasPendingRequests() or state() != Idle) {
		song()->log(tr("Could not get covers for \"%1 - %2\". HTTP connection is busy.").arg(song()->artist()).arg(song()->title()), QU::Warning);
		return;
	}

	QURequestUrl *url = this->url();

	setState(SearchRequest);

	clearBuffer();
	http()->setHost(url->host());
	http()->get(url->request(), buffer());

	delete url;
}

QFileInfoList QUHttpCollector::results() const {
	return source()->imageFolder(song()).entryInfoList(QUSongSupport::allowedPictureFiles(), QDir::Files, QDir::Name);
}

void QUHttpCollector::processNetworkStateChange(int state) {
	if(state == QHttp::Sending)
		communicator()->send(tr("Sending..."));
	else if(state == QHttp::Reading)
		communicator()->send(tr("Reading..."));
}

void QUHttpCollector::processNetworkOperationDone(bool error) {
	int count = localFiles().size();
	closeLocalFiles();

	if(error) {
		setState(Idle);
		communicator()->send(http()->errorString());
		communicator()->send(QUCommunicatorInterface::Failed);
		return;
	}

	if(state() == SearchRequest) {
		ignoredUrls = 0;
		processSearchResults();
	} else if(state() == ImageRequest)
		processImageResults(count);
}

QFile* QUHttpCollector::openLocalFile(const QString &filePath) {
	QFile *file = new QFile(filePath, this);

	if(!file->open(QIODevice::WriteOnly | QIODevice::Truncate)) {
		communicator()->send(tr("Could not open local file: \"%1\"").arg(filePath));
		return 0;
	}

	_localFiles.append(file);
	return file;
}

void QUHttpCollector::closeLocalFiles() {
	foreach(QFile* file, localFiles())
		file->close();

	qDeleteAll(_localFiles);
	_localFiles.clear();
}

void QUHttpCollector::handleOldDownloads() {
	// remove old downloads, if necessary
	if(!source()->keepDownloads()) {
		QFileInfoList fiList = source()->imageFolder(song()).entryInfoList(QUSongSupport::allowedPictureFiles(), QDir::Files, QDir::Name);
		foreach(QFileInfo fi, fiList)
			QFile::remove(fi.filePath());
	}
}

void QUHttpCollector::processImageResults(int count) {
	setState(Idle);
	if(ignoredUrls > 0)
		communicator()->send(tr("%1 results, %2 ignored").arg(count).arg(ignoredUrls));
	else
		communicator()->send(tr("%1 results").arg(count));
	communicator()->send(QUCommunicatorInterface::Done);
}
