#include "QUAlbumArtExCollector.h"
#include "QUAlbumArtExImageSource.h"
#include "QUAlbumArtExRequestUrl.h"

#include "QUCommunicatorInterface.h"
#include "QUSongInterface.h"
#include "QUSongSupport.h"

QUAlbumArtExCollector::QUAlbumArtExCollector(QUSongInterface *song, QUAlbumArtExImageSource *source): QUHttpCollector(song, source) {}

QURequestUrl* QUAlbumArtExCollector::url() const {
	return new QUAlbumArtExRequestUrl(
			source()->host(),
			QStringList() << source()->songDataField("first") << source()->songDataField("second") << source()->songDataField("third"),
			song());
}

void QUAlbumArtExCollector::processSearchResults() {
	setState(Idle);
}
