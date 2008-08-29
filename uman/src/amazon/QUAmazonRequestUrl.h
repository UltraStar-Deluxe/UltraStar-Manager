#ifndef QUAMAZONREQUESTURL_H_
#define QUAMAZONREQUESTURL_H_

#include "QU.h"
#include "QUSongFile.h"

#include <QUrl>

class QUAmazonRequestUrl: public QUrl {
public:
	QUAmazonRequestUrl(const QString &endpoint, const QString &artistProperty, const QString &titleProperty, QUSongFile *song);

	QString request() const;
private:
	void init(QUSongFile *song, const QString &artistProperty, const QString &titleProperty);
};

#endif /* QUAMAZONREQUESTURL_H_ */
