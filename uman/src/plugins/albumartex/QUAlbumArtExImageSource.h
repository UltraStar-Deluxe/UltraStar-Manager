#ifndef QUALBUMARTEXIMAGESOURCE_H
#define QUALBUMARTEXIMAGESOURCE_H

#include "QUMultilingualImageSource.h"

class QUAlbumArtExImageSource: public QUMultilingualImageSource {
	Q_OBJECT

public:
	QUAlbumArtExImageSource(QObject *parent = 0);

	virtual ImageType type() const { return CoverImage; }

	virtual QString name() const { return "Album Art Exchange"; }
	virtual QString version() const { return "1.0.0"; }
	virtual QString author() const { return "Marcel Taeumel"; }
	virtual QString description() const { return tr("Get splendid covers with high resolution for your songs."); }

	virtual QStringList songDataFields() const { return QStringList() << "First" << "Second" << "Third"; }
	virtual QStringList customDataFields() const { return QStringList() << "Max. width" << "Max. height" << "Sort by" << "Filter"; }
	virtual QString help(const QString &field) const;
	virtual QStringList hosts() const;

protected:
	virtual QString registryKey() const { return "albumartex"; }
	virtual QString defaultValue(const QString &key) const;
	virtual QMap<QString, QString> translationLocations() const;
	virtual QURemoteImageCollector* imageCollector(QUSongInterface*);
};

#endif // QUALBUMARTEXIMAGESOURCE_H
