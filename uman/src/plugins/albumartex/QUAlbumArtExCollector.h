#ifndef QUALBUMARTEXCOLLECTOR_H
#define QUALBUMARTEXCOLLECTOR_H

#include "QUHttpCollector.h"

class QUAlbumArtExImageSource;

class QUAlbumArtExCollector: public QUHttpCollector {
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "QUAlbumArtExCollector" FILE "albumartex.json")

public:
	QUAlbumArtExCollector(QUSongInterface *song, QUAlbumArtExImageSource *source = 0);

protected:
	virtual QURequestUrl* url() const;
	virtual void processSearchResults();
};

#endif // QUALBUMARTEXCOLLECTOR_H
