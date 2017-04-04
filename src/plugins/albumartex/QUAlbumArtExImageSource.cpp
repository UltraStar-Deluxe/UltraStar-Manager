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

QString QUAlbumArtExImageSource::help(const QString &field) const {
	if(QString::compare(field, customDataFields().first(), Qt::CaseInsensitive) == 0)
			return tr("Let's you specify the search domain.<br><br>ALL ... all<br>ARTIST ... artist<br>COMPOSER ... composer<br>TITLE ... title<br>UPLOADER ... by uploader<br>ARTISTTITLE ... artist & title");
	else if(QString::compare(field, customDataFields().at(1), Qt::CaseInsensitive) == 0)
		return tr("Let's you specify the result order. This helps you to better handle the download limit.<br><br>ARTIST ... by artist<br>COMPOSER ... by composer<br>TITLE ... title<br>DATE ... by date<br>POSTER ... by uploader<br>VIEWS ... by  most viewed<br>RATING ... by rating");
	else if(QString::compare(field, customDataFields().at(2), Qt::CaseInsensitive) == 0)
		return tr("Show only special kinds of images:<br><br>(empty) ... all images<br>RDY ... ready-to-go images<br>WRK ... \"needs work\" images");
	else if(QString::compare(field, customDataFields().at(3), Qt::CaseInsensitive) == 0)
		return tr("Show only images in size range:<br><br>any ... any size<br>1-600 ... 1x1 px to 600x600 px<br>601-800 ... 601x601 px to 800x800 px<br>801-1000 ... 801x801 px to 1000x1000 px<br>1001+ ... 1001x1001 px or larger");
	else
		return "";
}

QString QUAlbumArtExImageSource::defaultValue(const QString &key) const {
	if(QString::compare(key, songDataFields().first(), Qt::CaseInsensitive) == 0)
		return "ARTIST";
	else if(QString::compare(key, songDataFields().at(1), Qt::CaseInsensitive) == 0)
		return "TITLE";
	else if(QString::compare(key, customDataFields().first(), Qt::CaseInsensitive) == 0)
		return "ARTISTTITLE";
	else if(QString::compare(key, customDataFields().at(1), Qt::CaseInsensitive) == 0)
		return "DATE";
	else if(QString::compare(key, customDataFields().at(2), Qt::CaseInsensitive) == 0)
		return "RDY";
	else if(QString::compare(key, customDataFields().at(3), Qt::CaseInsensitive) == 0)
		return "1-600";
	else
		return QUMultilingualImageSource::defaultValue(key);
}

QMap<QString, QString> QUAlbumArtExImageSource::translationLocations() const {
	QDir dir = QCoreApplication::applicationDirPath();
	QMap<QString, QString> locations;

	if(dir.cd("plugins") && dir.cd("languages")) {
		locations.insert(QLocale(QLocale::German, QLocale::Germany).name(), dir.filePath("albumartex.de.qm"));
		locations.insert(QLocale(QLocale::Polish, QLocale::Poland).name(), dir.filePath("albumartex.pl.qm"));
		locations.insert(QLocale(QLocale::French, QLocale::France).name(), dir.filePath("albumartex.fr.qm"));
		locations.insert(QLocale(QLocale::Spanish, QLocale::Spain).name(), dir.filePath("albumartex.es.qm"));
		locations.insert(QLocale(QLocale::Portuguese, QLocale::Portugal).name(), dir.filePath("albumartex.pt.qm"));
	}

	return locations;
}
