#ifndef QUAMAZONRESPONSE_H_
#define QUAMAZONRESPONSE_H_

#include "QU.h"

#include <QObject>
#include <QList>
#include <QPair>
#include <QUrl>
#include <QDomDocument>

/*!
 * Parse a given XML response.
 */
class QUAmazonResponse: public QObject {
	Q_OBJECT

public:
	QUAmazonResponse(const QDomDocument &response, QObject *parent = 0);

	bool isValid() const { return _isValid; }
	int count() const { return _results.size(); }
	QUrl url(int index, QU::ImageSizes size) const;

private:
	bool _isValid;
	int  _count;
	QList<QUrl> _results; // store the urls to medium and large images

	void processResponse(const QDomElement &itemSearchResponse);
};

#endif /* QUAMAZONRESPONSE_H_ */
