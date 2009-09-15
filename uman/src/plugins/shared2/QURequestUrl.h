#ifndef QUREQUESTURL_H
#define QUREQUESTURL_H

#include <QCoreApplication>
#include <QUrl>
#include <QByteArray>
#include <QStringList>

class QUSongInterface;

class QURequestUrl: public QUrl {
	Q_DECLARE_TR_FUNCTIONS(QURequestUrl)

public:
	QURequestUrl(const QString &host, const QStringList &properties, QUSongInterface *song);
	virtual ~QURequestUrl() {}

	virtual QString request() const = 0;

protected:
	QUSongInterface* song() const { return _song; }
	QStringList properties() const { return _properties; }

	QByteArray fixedPercentageEncoding() const;
	QByteArray fixedPercentageEncoding(QByteArray source) const;

private:
	QUSongInterface *_song;
	QStringList      _properties;
};

#endif // QUREQUESTURL_H
