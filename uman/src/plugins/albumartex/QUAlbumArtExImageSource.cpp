#include "QUAlbumArtExImageSource.h"
#include "QUAlbumArtExCollector.h"

#include <QCoreApplication>
#include <QString>
#include <QLocale>

QUAlbumArtExImageSource::QUAlbumArtExImageSource(QObject *parent): QUMultilingualImageSource(parent) {}

QStringList QUAlbumArtExImageSource::hosts() const {
	return QStringList() << "www.albumartexchange.com";
}

QURemoteImageCollector* QUAlbumArtExImageSource::imageCollector(QUSongInterface *song) {
	return new QUAlbumArtExCollector(song, this);
}

QString QUAlbumArtExImageSource::defaultValue(const QString &key) const {
	if(QString::compare(key, songDataFields().first(), Qt::CaseInsensitive) == 0)
		return "ARTIST";
	else if(QString::compare(key, songDataFields().at(1), Qt::CaseInsensitive) == 0)
		return "TITLE";
	else if(QString::compare(key, customDataFields().at(0), Qt::CaseInsensitive) == 0)
		return "600";
	else if(QString::compare(key, customDataFields().at(1), Qt::CaseInsensitive) == 0)
		return "600";
	else
		return QUMultilingualImageSource::defaultValue(key);
}

QMap<QString, QString> QUAlbumArtExImageSource::translationLocations() const {
	QDir dir = QCoreApplication::applicationDirPath();
	QMap<QString, QString> locations;

	if(dir.cd("plugins") && dir.cd("languages")) {
		locations.insert(QLocale(QLocale::German, QLocale::Germany).name(), dir.filePath("albumartex.de.qm"));
		locations.insert(QLocale(QLocale::Polish, QLocale::Poland).name(), dir.filePath("albumartex.pl.qm"));
	}

	return locations;
}

Q_EXPORT_PLUGIN2(qualbumarteximagesource, QUAlbumArtExImageSource);
