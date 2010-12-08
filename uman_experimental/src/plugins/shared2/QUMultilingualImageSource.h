#ifndef QUMULTILINGUALIMAGESOURCE_H
#define QUMULTILINGUALIMAGESOURCE_H

#include <QObject>

#include "QU.h"
#include "QURemoteImageSourcePlugin.h"
#include "QUSongInterface.h"

/*!
 * This class offers support for multiple languages for each RIS plugin.
 *
 * It also definies basic API versions.
 */
class QUMultilingualImageSource: public QObject, public QURemoteImageSource {
	Q_OBJECT
	Q_INTERFACES(QURemoteImageSource)

public:
	QUMultilingualImageSource(QObject *parent = 0);
	~QUMultilingualImageSource();

	virtual int pluginApiVersion() const { return REMOTE_IMAGE_SOURCE_PLUGIN_API_VERSION; }
	virtual int songApiVersion() const { return SONG_API_VERSION; }

	virtual QString name() const { return tr("Unnamed plugin"); }
	virtual QString version() const { return "1.0.0"; }
	virtual QString author() const { return tr("Unknown person"); }
	virtual QString description() const { return tr("No description given."); }
	virtual QIcon icon() const { return QIcon(":/control/images.png"); }

	virtual QMap<QString, QTranslator*> translations() const;

	virtual QString songDataField(const QString &field) const;
	virtual void setSongDataField(const QString &field, const QString &songProperty);

	virtual QString customDataField(const QString &field) const;
	virtual void setCustomDataField(const QString &field, const QString &value);

	virtual QString host() const;
	virtual void setHost(const QString &);

	virtual int limit() const;
	virtual void setLimit(int);

	virtual bool keepDownloads() const;
	virtual void setKeepDownloads(bool);

	virtual QList<QURemoteImageCollector*> imageCollectors(QList<QUSongInterface*>);
	virtual QDir imageFolder(QUSongInterface*) const;

protected:
	// list all "locale > location" associations for translation files
	virtual QMap<QString, QString> translationLocations() const = 0;

	// should be unique to all plugins
	virtual QString registryKey() const = 0;

	// used for registry access
	virtual QString defaultValue(const QString &key) const;

	// create a collector for a special song
	virtual QURemoteImageCollector* imageCollector(QUSongInterface*) = 0;

private:
	mutable QMap<QString, QTranslator*> _translations;
	QList<QURemoteImageCollector*>      _collectors;
};

#endif // QUMULTILINGUALIMAGESOURCE_H
