#ifndef QUAMAZONIMAGESOURCE_H
#define QUAMAZONIMAGESOURCE_H

#include <QObject>

#include "QURemoteImageSourcePlugin.h"
#include "QUSongInterface.h"

class QUAmazonImageSource: public QObject, public QURemoteImageSource {
	Q_OBJECT
	Q_INTERFACES(QURemoteImageSource)

public:
	QUAmazonImageSource(QObject *parent = 0);
	~QUAmazonImageSource() {}

	virtual int pluginApiVersion() const { return REMOTE_IMAGE_SOURCE_PLUGIN_API_VERSION; }
	virtual QMap<QString, QTranslator*> translations() const { return QMap<QString, QTranslator*>(); }

	virtual QStringList songDataFields() const { return QStringList() << "Artist" << "Title"; }
	virtual void setSongDataField(const QString &field, const QString &songProperty);

	virtual QStringList customDataFields() const { return QStringList() << "Access Key ID" << "Secret Access Key"; }
	virtual void setCustomDataField(const QString &field, const QString &value);
	virtual void showHelpDialog(const QString &field, QWidget *parent) const;

	virtual QString name() const { return "Amazon Product Advertising API"; }

	virtual QStringList hosts() const;
	virtual void setHost(const QString &);

	virtual int limit() const;
	virtual void setLimit(int);

	virtual bool keepDownloads() const;
	virtual void setKeepDownloads(bool);

	virtual QList<QURemoteImageCollector*> imageCollectors(QList<QUSongInterface*>);
	virtual QDir imageFolder(QUSongInterface*) const;
};

#endif // QUAMAZONIMAGESOURCE_H
