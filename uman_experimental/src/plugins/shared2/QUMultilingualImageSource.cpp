#include "QUMultilingualImageSource.h"
#include "QUStringSupport.h"

#include <QCoreApplication>
#include <QSettings>
#include <QString>
#include <QVariant>
#include <QLocale>
#include <QTranslator>

QUMultilingualImageSource::QUMultilingualImageSource(QObject *parent): QObject(parent) {}

QUMultilingualImageSource::~QUMultilingualImageSource() {
	qDeleteAll(_translations);
	_translations.clear();
}

QString QUMultilingualImageSource::songDataField(const QString &field) const {
	return QSettings().value(registryKey() + "/" + field.toLower(), defaultValue(field)).toString();
}

void QUMultilingualImageSource::setSongDataField(const QString &field, const QString &songProperty) {
	QSettings().setValue(registryKey() + "/" + field.toLower(), songProperty);
}

QString QUMultilingualImageSource::customDataField(const QString &field) const {
	return QSettings().value(registryKey() + "/" + field.toLower(), defaultValue(field)).toString();
}

void QUMultilingualImageSource::setCustomDataField(const QString &field, const QString &value) {
	QSettings().setValue(registryKey() + "/" + field.toLower(), value);
}

QString QUMultilingualImageSource::host() const {
	return QSettings().value(registryKey() + "/host", hosts().first()).toString();
}

void QUMultilingualImageSource::setHost(const QString &host) {
	QSettings().setValue(registryKey() + "/host", host);
}

int QUMultilingualImageSource::limit() const {
	return QSettings().value(registryKey() + "/limit", 10).toInt();
}

void QUMultilingualImageSource::setLimit(int limit) {
	QSettings().setValue(registryKey() + "/limit", limit);
}

bool QUMultilingualImageSource::keepDownloads() const {
	return QSettings().value(registryKey() + "/keepDownloads", false).toBool();
}

void QUMultilingualImageSource::setKeepDownloads(bool keep) {
	QSettings().setValue(registryKey() + "/keepDownloads", keep);
}

QList<QURemoteImageCollector*> QUMultilingualImageSource::imageCollectors(QList<QUSongInterface*> songs) {
	if(!_collectors.isEmpty()) {
		qDeleteAll(_collectors);
		_collectors.clear();
	}

	foreach(QUSongInterface *song, songs)
		_collectors << imageCollector(song);
	return _collectors;
}

QDir QUMultilingualImageSource::imageFolder(QUSongInterface *song) const {
	QString folderName = QUStringSupport::withoutUnsupportedCharacters(
			QString("%1 - %2").arg(song->artist()).arg(song->title())).trimmed();

	if(folderName.isEmpty())
		folderName = "_unknown";

	QDir result(QCoreApplication::applicationDirPath());
	result.mkdir("covers");
	if(result.cd("covers")) {
		result.mkdir(registryKey());
		if(result.cd(registryKey())) {
			result.mkdir(folderName);
			result.cd(folderName);
		}
	}

	return result;
}

QMap<QString, QTranslator*> QUMultilingualImageSource::translations() const {
	if(_translations.isEmpty()) {
		QMapIterator<QString, QString> i(translationLocations());
		while(i.hasNext()) {
			i.next();
			QTranslator *translator = new QTranslator();
			if(translator->load(i.value()))
				_translations.insert(i.key(), translator);
		}
	}

	return _translations;
}

QString QUMultilingualImageSource::defaultValue(const QString &key) const {
	return NONE;
}
