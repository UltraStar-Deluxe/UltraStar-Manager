#ifndef QUMULTILINGUALTASKFACTORY_H
#define QUMULTILINGUALTASKFACTORY_H

#include <QObject>

#include "QUTaskPlugin.h"
#include "QUSongInterface.h"

/*!
 * This class offers support for multiple languages for each plugins.
 *
 * It also definies basic API versions.
 */
class QUMultilingualTaskFactory: public QObject, public QUTaskFactory {
	Q_OBJECT
	Q_INTERFACES(QUTaskFactory)

public:
	QUMultilingualTaskFactory(QObject *parent = 0);
	~QUMultilingualTaskFactory();

	virtual int pluginApiVersion() const { return PLUGIN_API_VERSION; }
	virtual int songApiVersion() const { return SONG_API_VERSION; }

	virtual QString name() const { return tr("Unnamed plugin"); }
	virtual QString version() const { return "1.0.0"; }
	virtual QString author() const { return tr("Unknown person"); }
	virtual QString description() const { return tr("No description given."); }
	virtual QIcon icon() const { return QIcon(":/control/tasks.png"); }

	virtual QMap<QString, QTranslator*> translations() const;

protected:
	virtual QMap<QString, QString> translationLocations() const = 0;

private:
	mutable QMap<QString, QTranslator*> _translations;
};

#endif // QUMULTILINGUALTASKFACTORY_H
