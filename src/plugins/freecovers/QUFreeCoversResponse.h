#ifndef QUFREECOVERSRESPONSE_H
#define QUFREECOVERSRESPONSE_H

#include "QU.h"

#include <QObject>
#include <QList>
#include <QPair>
#include <QUrl>
#include <QDomDocument>

class QUFreeCoversResponse: public QObject {
	Q_OBJECT

public:
	QUFreeCoversResponse(const QDomDocument &response, QObject *parent = 0);
	QList<QUrl> results() const { return _results; }

private:
	QList<QUrl> _results;

	void processResponse(const QDomElement &rsp);
};

#endif // QUFREECOVERSRESPONSE_H
