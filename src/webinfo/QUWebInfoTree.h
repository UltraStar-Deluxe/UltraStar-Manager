#ifndef QUWEBINFOTREE_H_
#define QUWEBINFOTREE_H_

#include "QU.h"
#include "QUSongFile.h"

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
	void showInformation(QUSongFile *song);
	void applyWebInformationToSong(QTreeWidgetItem *item, int column);

private slots:
	void processNetworkReply(QNetworkReply* reply);

private:
	QNetworkAccessManager *_manager;
	QTreeWidgetItem *_swisscharts;
	QTreeWidgetItem *_discogs;
	QTreeWidgetItem *_allmusic;
	QUSongFile *_song;
	QString _artist;
	QString _title;
	QString _genre;
	QString _year;

	QTreeWidgetItem* createInfoItem(const QIcon &icon, const QString &tag, const QString &value, const QIcon &status, const QString &score = "", const QString &tooltip = "");
	QTreeWidgetItem* createInfoItem(const QIcon &icon, const QString &tag, const QString &value, const QU::SpellState &spellState);
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
