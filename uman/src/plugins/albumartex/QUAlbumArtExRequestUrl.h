#ifndef QUALBUMARTEXREQUESTURL_H
#define QUALBUMARTEXREQUESTURL_H

#include "QURequestUrl.h"

class QUAlbumArtExRequestUrl: public QURequestUrl {
public:
	QUAlbumArtExRequestUrl(const QString &host, const QStringList &properties, QUSongInterface *song);
	virtual QString request() const;
};

#endif // QUALBUMARTEXREQUESTURL_H
