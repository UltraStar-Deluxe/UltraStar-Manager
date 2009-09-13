#ifndef QUHTTPCOLLECTOR_H
#define QUHTTPCOLLECTOR_H

#include <QObject>

#include "QURemoteImageSourcePlugin.h"

class QHttp;
class QBuffer;
class QFile;
class QUMultilingualImageSource;
class QURequestUrl;

class QUHttpCollector: public QObject, public QURemoteImageCollector {
	Q_OBJECT
	Q_INTERFACES(QURemoteImageCollector)

public:
	enum State {
		Idle,
		SearchRequest, // current or next request will be handled as search information download
		ImageRequest // current or next request will be handled as picture file download
	};

	QUHttpCollector(QUSongInterface *song, QUMultilingualImageSource *source = 0);

	QUCommunicatorInterface* communicator() const { return _communicator; }
	virtual void setCommunicator(QUCommunicatorInterface *c) { _communicator = c; }

	virtual QFileInfoList results() const;
	virtual void collect();

protected slots:
	virtual void processNetworkStateChange(int state);
	virtual void processNetworkOperationDone(bool error);

protected:
	State state() const { return _state; }
	void setState(State s) { _state = s; }

	QUMultilingualImageSource* source() const { return _source; }
	QHttp* http() const { return _http; }
	QUSongInterface* song() const { return _song; }

	QBuffer* buffer() const { return _buffer; }
	void clearBuffer();

	void initNetwork();

	QFile* openLocalFile(const QString &filePath);
	void closeLocalFiles();
	QList<QFile*> localFiles() const { return _localFiles; }

	void handleOldDownloads();

	int ignoredUrls;

	virtual QURequestUrl* url() const = 0;

	virtual void processSearchResults() = 0;
	virtual void processImageResults(int count);

private:
	State                      _state;
	QUMultilingualImageSource *_source;
	QUCommunicatorInterface   *_communicator;
	QHttp                     *_http;
	QUSongInterface           *_song;
	QBuffer                   *_buffer;
	QList<QFile*>              _localFiles; // should all be open
};

#endif // QUHTTPCOLLECTOR_H
