#include "QUFreeCoversSource.h"
#include "QUFreeCoversCollector.h"

#include <QCoreApplication>
#include <QString>
#include <QLocale>

QUFreeCoversSource::QUFreeCoversSource(QObject *parent): QUMultilingualImageSource(parent) {}

QStringList QUFreeCoversSource::hosts() const {
	return QStringList() << "www.freecovers.net";
}

QURemoteImageCollector* QUFreeCoversSource::imageCollector(QUSongInterface *song) {
	return new QUFreeCoversCollector(song, this);
}

QString QUFreeCoversSource::defaultValue(const QString &key) const {
	if(QString::compare(key, songDataFields().first(), Qt::CaseInsensitive) == 0)
		return "ARTIST";
	else if(QString::compare(key, songDataFields().at(1), Qt::CaseInsensitive) == 0)
		return "TITLE";
	else
		return QUMultilingualImageSource::defaultValue(key);
}

QMap<QString, QString> QUFreeCoversSource::translationLocations() const {
	QDir dir = QCoreApplication::applicationDirPath();
	QMap<QString, QString> locations;

	if(dir.cd("plugins") && dir.cd("languages")) {
		locations.insert(QLocale(QLocale::German, QLocale::Germany).name(), dir.filePath("freecovers.de.qm"));
		locations.insert(QLocale(QLocale::Polish, QLocale::Poland).name(), dir.filePath("freecovers.pl.qm"));
		locations.insert(QLocale(QLocale::French, QLocale::France).name(), dir.filePath("freecovers.fr.qm"));
		locations.insert(QLocale(QLocale::Spanish, QLocale::Spain).name(), dir.filePath("freecovers.es.qm"));
		locations.insert(QLocale(QLocale::Portuguese, QLocale::Portugal).name(), dir.filePath("freecovers.pt.qm"));
	}

	return locations;
}
