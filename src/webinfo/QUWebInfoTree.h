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
	QTreeWidgetItem *swisscharts;
	QTreeWidgetItem *allmusic;
	QString _artist;
	QString _title;
	QString _genre;
	QString _year;

	QTreeWidgetItem* createInfoItem(const QIcon &icon, const QString &tag, const QString &value, const QIcon &status);
	//QTreeWidgetItem* createInfoItem(const QIcon &icon, const QString &tag, const QString &value);
};

#endif /* QUWEBINFOTREE_H_ */
