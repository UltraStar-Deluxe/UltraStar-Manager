#ifndef QUFREECOVERSSOURCE_H
#define QUFREECOVERSSOURCE_H

#include "QUMultilingualImageSource.h"

class QUFreeCoversSource: public QUMultilingualImageSource {
	Q_OBJECT

public:
	QUFreeCoversSource(QObject *parent = 0);

	virtual ImageType type() const { return CoverImage; }

	virtual QString name() const { return "FreeCovers"; }
	virtual QString version() const { return "1.0.0"; }
	virtual QString author() const { return "Marcel Taeumel"; }
	virtual QString description() const { return tr("The cd covers archive. No uncompressed covers."); }

	virtual QStringList songDataFields() const { return QStringList() << "First" << "Second" << "Third"; }
	virtual QStringList customDataFields() const { return QStringList(); }
	virtual QString help(const QString &field) const { return QString(); }
	virtual QStringList hosts() const;

protected:
	virtual QString registryKey() const { return "freecovers"; }
	virtual QString defaultValue(const QString &key) const;
	virtual QMap<QString, QString> translationLocations() const;
	virtual QURemoteImageCollector* imageCollector(QUSongInterface*);
};

#endif // QUFREECOVERSSOURCE_H
