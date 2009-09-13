#include "QUAmazonImageSource.h"
#include "QUAmazonImageCollector.h"

#include <QCoreApplication>
#include <QString>
#include <QLocale>

QUAmazonImageSource::QUAmazonImageSource(QObject *parent): QUMultilingualImageSource(parent) {}

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

QURemoteImageCollector* QUAmazonImageSource::imageCollector(QUSongInterface *song) {
	return new QUAmazonImageCollector(song, this);
}

QString QUAmazonImageSource::defaultValue(const QString &key) const {
	if(QString::compare(key, songDataFields().first(), Qt::CaseInsensitive) == 0)
		return "ARTIST";
	else if(QString::compare(key, songDataFields().at(1), Qt::CaseInsensitive) == 0)
		return "TITLE";
	else
		return QUMultilingualImageSource::defaultValue(key);
}

QMap<QString, QString> QUAmazonImageSource::translationLocations() const {
	QDir dir = QCoreApplication::applicationDirPath();
	QMap<QString, QString> locations;

	if(dir.cd("plugins") && dir.cd("languages")) {
		locations.insert(QLocale(QLocale::German, QLocale::Germany).name(), dir.filePath("amazon.de.qm"));
		locations.insert(QLocale(QLocale::Polish, QLocale::Poland).name(), dir.filePath("amazon.pl.qm"));
	}

	return locations;
}

Q_EXPORT_PLUGIN2(quamazonimagesource, QUAmazonImageSource);
