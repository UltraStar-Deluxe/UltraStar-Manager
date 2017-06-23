#ifndef QUWEBINFOTREE_H_
#define QUWEBINFOTREE_H_

#include "QU.h"

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QFileInfo>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrlQuery>
#include <QNetworkReply>

class QUWebInfoTree: public QTreeWidget {
	Q_OBJECT

public:
	QUWebInfoTree(QWidget *parent = 0);

public slots:
	void showInformation(const QString &artist, const QString &title, const QString &genre, const QString &year);

private slots:
	void processNetworkReply(QNetworkReply* reply);

private:
	QNetworkAccessManager *_manager;
	QTreeWidgetItem *_swisscharts;
	QTreeWidgetItem *_allmusic;
	QTreeWidgetItem *_discogs;
	QString _artist;
	QString _title;
	QString _genre;
	QString _year;
	QString _swisscharts_artist;
	QString _swisscharts_title;
	QString _swisscharts_year;
	QString _allmusic_url;
	QString _allmusic_title;
	QString _allmusic_artist;
	QString _allmusic_genre;
	QString _discogs_url;
	QString _discogs_artist;
	QString _discogs_title;
	QString _discogs_genre;
	QString _discogs_style;
	QString _discogs_year;

	QTreeWidgetItem* createInfoItem(const QIcon &icon, const QString &tag, const QString &value, const QIcon &status);
	void getSwisschartsInformation();
	void getAllmusicInformation();
	void getDiscogsInformation();
	void processSwisschartsReply(QNetworkReply* reply);
	void processAllmusicReply(QNetworkReply* reply);
	void processAllmusicSearchReply(QNetworkReply* reply);
	void processAllmusicSongReply(QNetworkReply* reply);
	void processDiscogsReply(QNetworkReply* reply);
	void processDiscogsSearchReply(QNetworkReply* reply);
	void processDiscogsSongReply(QNetworkReply* reply);
};

#endif /* QUWEBINFOTREE_H_ */
