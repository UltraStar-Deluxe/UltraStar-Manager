#include "QUAmazonImageSource.h"
#include "QUAmazonImageCollector.h"
#include "QUStringSupport.h"

#include <QCoreApplication>
#include <QSettings>
#include <QString>
#include <QVariant>

QUAmazonImageSource::QUAmazonImageSource(QObject *parent): QObject(parent) {}

QString QUAmazonImageSource::songDataField(const QString &field) const {
	return QSettings().value(registryKey() + "/" + field.toLower(), defaultValue(field)).toString();
}

void QUAmazonImageSource::setSongDataField(const QString &field, const QString &songProperty) {
	QSettings().setValue(registryKey() + "/" + field.toLower(), songProperty);
}

QString QUAmazonImageSource::customDataField(const QString &field) const {
	return QSettings().value(registryKey() + "/" + field.toLower(), NONE).toString();
}

void QUAmazonImageSource::setCustomDataField(const QString &field, const QString &value) {
	QSettings().setValue(registryKey() + "/" + field.toLower(), value);
}

QString QUAmazonImageSource::help(const QString &field) const {
	if(QString::compare(field, customDataFields().first(), Qt::CaseInsensitive) == 0)
		return tr("<b>What is an 'Access Key ID'?</b><br><br>"
				"This public key identifies you as the party responsible for the request. It belongs to an Amazon Web Services Account. All free services require you to send this key along with the request.<br><br>"
				"You get your own key if you <a href=\"https://aws-portal.amazon.com/gp/aws/developer/registration/index.html\">create an AWS Account</a>.");
	else if(QString::compare(field, customDataFields().at(1), Qt::CaseInsensitive) == 0)
		return tr("<b>What is a 'Secret Access Key'?</b><br><br>"
				"This secret key is used to calculate a signature to include in requests to the Amazon Product Advertising API which requires this kind of authentication. It should be known only by you and AWS. That's why each user of a desktop application like this one needs his own key.<br><br>"
				"You get your own key if you <a href=\"https://aws-portal.amazon.com/gp/aws/developer/registration/index.html\">create an AWS Account</a>.");
	else
		return tr("There is no help for this field. Try to figure it out by yourself.");
}

QStringList QUAmazonImageSource::hosts() const {
	return QStringList()
			<< "ecs.amazonaws.de"
			<< "ecs.amazonaws.co.uk"
			<< "ecs.amazonaws.com"
			<< "ecs.amazonaws.jp"
			<< "ecs.amazonaws.fr"
			<< "ecs.amazonaws.ca";
}

QString QUAmazonImageSource::host() const {
	return QSettings().value(registryKey() + "/host", hosts().first()).toString();
}

void QUAmazonImageSource::setHost(const QString &host) {
	QSettings().setValue(registryKey() + "/host", host);
}

int QUAmazonImageSource::limit() const {
	return QSettings().value(registryKey() + "/limit", 10).toInt();
}

void QUAmazonImageSource::setLimit(int limit) {
	QSettings().setValue(registryKey() + "/limit", limit);
}

bool QUAmazonImageSource::keepDownloads() const {
	return QSettings().value(registryKey() + "/keepDownloads", false).toBool();
}

void QUAmazonImageSource::setKeepDownloads(bool keep) {
	QSettings().setValue(registryKey() + "/keepDownloads", keep);
}

QList<QURemoteImageCollector*> QUAmazonImageSource::imageCollectors(QList<QUSongInterface*> songs) {
	if(!_collectors.isEmpty()) {
		qDeleteAll(_collectors);
		_collectors.clear();
	}

	foreach(QUSongInterface *song, songs)
		_collectors << new QUAmazonImageCollector(song, this);
	return _collectors;
}

QDir QUAmazonImageSource::imageFolder(QUSongInterface *song) const {
	QString folderName = QUStringSupport::withoutUnsupportedCharacters(
			QString("%1 - %2").arg(song->artist()).arg(song->title())).trimmed();

	if(folderName.isEmpty())
		folderName = "_unknown";

	QDir result(QCoreApplication::applicationDirPath());
	result.mkdir("covers");
	if(result.cd("covers")) {
		result.mkdir(folderName);
		result.cd(folderName);
	}

	return result;
}

QString QUAmazonImageSource::defaultValue(const QString &key) const {
	if(QString::compare(key, songDataFields().first(), Qt::CaseInsensitive) == 0)
		return "artist";
	else if(QString::compare(key, songDataFields().at(1), Qt::CaseInsensitive) == 0)
		return "title";
	else
		return NONE;
}

Q_EXPORT_PLUGIN2(quamazonimagesource, QUAmazonImageSource);
