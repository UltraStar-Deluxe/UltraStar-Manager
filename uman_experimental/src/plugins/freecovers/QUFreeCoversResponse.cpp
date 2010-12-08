#include "QUFreeCoversResponse.h"

#include <QVariant>

#include <QDomElement>
#include <QDomAttr>
#include <QDomText>

#include <QMessageBox>

QUFreeCoversResponse::QUFreeCoversResponse(const QDomDocument &response, QObject *parent): QObject(parent) {
	QDomElement rsp = response.firstChildElement("rsp");

	if(!rsp.isNull())
		processResponse(rsp);
}

void QUFreeCoversResponse::processResponse(const QDomElement &rsp) {
	QDomElement title = rsp.firstChildElement("title");
	while(!title.isNull()) {
		QDomElement covers = title.firstChildElement("covers");
		if(!covers.isNull()) {
			QDomElement cover = covers.firstChildElement("cover");
			while(!cover.isNull()) {
				QString type = cover.firstChildElement("type").firstChild().nodeValue();
				if(QString::compare(type, "front", Qt::CaseInsensitive) == 0) { // only front covers - no back, cd or inlay
					QString previewUrl = cover.firstChildElement("preview").firstChild().nodeValue();
					if(!previewUrl.isEmpty())
						_results << previewUrl;
				}
				cover = cover.nextSiblingElement("cover");
			}
		}
		title = title.nextSiblingElement("title");
	}
}
