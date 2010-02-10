#ifndef QUALBUMARTEXREQUESTURL_H
#define QUALBUMARTEXREQUESTURL_H

#include "QURequestUrl.h"

class QUAlbumArtExRequestUrl: public QURequestUrl {
public:
	enum SortBy {
		Artist = 1,
		Composer = 2,
		Title = 3,
		Date = 4,
		Poster = 5,
		MostViewed = 6,
		Rating = 7
	};

	enum Filter {
		AllImages = 1,
		ReadyToGo = 2,
		NeedsWork = 3
	};

	QUAlbumArtExRequestUrl(const QString &host, const QStringList &properties, QUSongInterface *song);
	virtual QString request() const;

private:
	void initQuery();
};

#endif // QUALBUMARTEXREQUESTURL_H
