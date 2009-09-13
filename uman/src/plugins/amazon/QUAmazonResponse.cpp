#include "QUAmazonResponse.h"

#include <QVariant>

#include <QDomElement>
#include <QDomAttr>
#include <QDomText>

#include <QMessageBox>

QUAmazonResponse::QUAmazonResponse(const QDomDocument &response, QObject *parent):
	QObject(parent),
	_isValid(false),
	_count(0)
{
	QDomElement itemSearchResponse = response.firstChildElement("ItemSearchResponse");

	if(!itemSearchResponse.isNull())
		processResponse(itemSearchResponse);
}

void QUAmazonResponse::processResponse(const QDomElement &itemSearchResponse) {
	QDomElement items = itemSearchResponse.firstChildElement("Items");

	// check, whether the request was successful
	QDomElement request = items.firstChildElement("Request");
	QDomElement isValid = request.firstChildElement("IsValid");
	_isValid = QVariant(isValid.firstChild().nodeValue()).toBool();
	if(!_isValid) return;

	// check the total number of results
	QDomElement totalResults = items.firstChildElement("TotalResults");
	_count = QVariant(totalResults.firstChild().nodeValue()).toInt();
	if(_count == 0)	return;

	// get found image links
	QDomElement item = items.firstChildElement("Item");
	while(!item.isNull()) {
		QString mediumImgUrl = item.firstChildElement("MediumImage").firstChildElement("URL").firstChild().nodeValue();
		if(mediumImgUrl.isEmpty())
			mediumImgUrl = item
				.firstChildElement("ImageSets").firstChild()
				.firstChildElement("MediumImage")
				.firstChildElement("URL").firstChild().nodeValue();

		QString largeImgUrl = item.firstChildElement("LargeImage").firstChildElement("URL").firstChild().nodeValue();
		if(largeImgUrl.isEmpty())
			largeImgUrl = item
				.firstChildElement("ImageSets").firstChild()
				.firstChildElement("LargeImage")
				.firstChildElement("URL").firstChild().nodeValue();

//		qDebug(mediumImgUrl.toLocal8Bit().data());
//		qDebug(largeImgUrl.toLocal8Bit().data());

		if(!largeImgUrl.isEmpty())
			_results << QUrl(largeImgUrl);

		item = item.nextSiblingElement("Item");
	}
}

QUrl QUAmazonResponse::url(int index, QU::ImageSizes size) const {
	if(index < 0 or index >= _results.size())
		return QUrl();

	return _results.at(index);
}
