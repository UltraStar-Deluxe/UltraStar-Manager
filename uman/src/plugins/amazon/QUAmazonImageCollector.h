#ifndef QUAMAZONIMAGECOLLECTOR_H
#define QUAMAZONIMAGECOLLECTOR_H

#include <QObject>

#include "QURemoteImageSourcePlugin.h"

class QHttp;
class QBuffer;
class QFile;
class QUAmazonImageSource;

class QUAmazonImageCollector: public QObject, public QURemoteImageCollector {
	Q_OBJECT
	Q_INTERFACES(QURemoteImageCollector)

public:
	enum State {
		Idle,
		SearchRequest, // current or next request will be handled as search information download
		ImageRequest // current or next request will be handled as picture file download
	};

	QUAmazonImageCollector(QUSongInterface *song, QUAmazonImageSource *source = 0);

	virtual void setCommunicator(QUCommunicatorInterface *c) { _communicator = c; }
	virtual QFileInfoList results() const;
	virtual void collect();

protected slots:
	void processNetworkStateChange(int state);
	void processNetworkOperationDone(bool error);

protected:
	State state() const { return _state; }
	void setState(State s) { _state = s; }

	QUAmazonImageSource* source() const { return _source; }
	QUCommunicatorInterface* communicator() const { return _communicator; }
	QHttp* http() const { return _http; }
	QUSongInterface* song() const { return _song; }

	QBuffer* buffer() const { return _buffer; }
	void clearBuffer();

	void initNetwork();

	QFile* openLocalFile(const QString &filePath);
	void closeLocalFiles();
	QList<QFile*> localFiles() const { return _localFiles; }

private:
	State                    _state;
	QUAmazonImageSource     *_source;
	QUCommunicatorInterface *_communicator;
	QHttp                   *_http;
	QUSongInterface         *_song;
	QBuffer                 *_buffer;
	QList<QFile*>            _localFiles; // should all be open

	void processSearchResults();
	void processImageResults(int count);
};

#endif // QUAMAZONIMAGECOLLECTOR_H
