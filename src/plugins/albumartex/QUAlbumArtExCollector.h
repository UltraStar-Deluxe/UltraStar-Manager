#ifndef QUALBUMARTEXCOLLECTOR_H
#define QUALBUMARTEXCOLLECTOR_H

#include "QUHttpCollector.h"

class QUAlbumArtExImageSource;

class QUAlbumArtExCollector: public QUHttpCollector {
	Q_OBJECT

public:
	QUAlbumArtExCollector(QUSongInterface *song, QUAlbumArtExImageSource *source = 0);

protected:
	virtual QURequestUrl* url() const;
	virtual void processSearchResults();
	virtual void processImageResults(QNetworkReply* reply);
};

#endif // QUALBUMARTEXCOLLECTOR_H
