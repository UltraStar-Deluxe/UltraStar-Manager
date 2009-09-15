#ifndef QUAMAZONIMAGESOURCE_H
#define QUAMAZONIMAGESOURCE_H

#include "QUMultilingualImageSource.h"

class QUAmazonImageSource: public QUMultilingualImageSource {
	Q_OBJECT

public:
	QUAmazonImageSource(QObject *parent = 0);

	virtual ImageType type() const { return CoverImage; }

	virtual QString name() const { return "Amazon Product Advertising API"; }
	virtual QString version() const { return "1.0.0"; }
	virtual QString author() const { return "Marcel Taeumel"; }
	virtual QString description() const { return tr("Get song covers from Amazon."); }

	virtual QStringList songDataFields() const { return QStringList() << "Artist" << "Title"; }
	virtual QStringList customDataFields() const { return QStringList() << "Access Key ID" << "Secret Access Key"; }
	virtual QString help(const QString &field) const;
	virtual QStringList hosts() const;

protected:
	virtual QString registryKey() const { return "amazon"; }
	virtual QString defaultValue(const QString &key) const;
	virtual QMap<QString, QString> translationLocations() const;
	virtual QURemoteImageCollector* imageCollector(QUSongInterface*);
};

#endif // QUAMAZONIMAGESOURCE_H
