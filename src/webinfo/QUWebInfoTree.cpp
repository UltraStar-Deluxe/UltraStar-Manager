#include "QUWebInfoTree.h"
#include "QUSongFile.h"

#include <QStringList>
#include <QHeaderView>
#include <QFont>
#include <QMessageBox>
#include <QImage>
#include <QTextStream>
#include <QBuffer>
#include <QDebug>

#include "QUSongSupport.h"

QUWebInfoTree::QUWebInfoTree(QWidget *parent): QTreeWidget(parent) {
	this->setColumnCount(3);

	//this->header()->hide();
	this->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
	this->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
	this->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
	//this->header()->setSectionResizeMode(1, QHeaderView::Stretch);

	this->setRootIsDecorated(false);
	this->setIndentation(10);

	// set up "swisscharts" toplevel item
	swisscharts = new QTreeWidgetItem();
	this->addTopLevelItem(swisscharts);

	swisscharts->setIcon(0, QIcon(":/faviconSwissCharts.ico"));
	swisscharts->setText(0, tr("swisscharts.com"));
	swisscharts->setFlags(Qt::ItemIsEnabled);
	swisscharts->setTextColor(0, Qt::darkGray);
	swisscharts->setFirstColumnSpanned(true);

	swisscharts->setExpanded(true);
	swisscharts->setHidden(true);

	// set up "allmusic" toplevel item
	allmusic = new QTreeWidgetItem();
	this->addTopLevelItem(allmusic);

	allmusic->setText(0, tr("allmusic.com"));
	allmusic->setFlags(Qt::ItemIsEnabled);
	allmusic->setTextColor(0, Qt::darkGray);
	allmusic->setFirstColumnSpanned(true);

	allmusic->setExpanded(true);
	allmusic->setHidden(true);
}

QTreeWidgetItem* QUWebInfoTree::createInfoItem(const QIcon &icon, const QString &tag, const QString &value, const QIcon &status) {
	QTreeWidgetItem *infoItem = new QTreeWidgetItem();

	infoItem->setIcon(0, icon);
	infoItem->setText(0, tag);
	infoItem->setText(1, value);
	infoItem->setIcon(2, status);
	if(value != N_A) infoItem->setToolTip(1, value);

	QFont f(infoItem->font(0));
	f.setBold(true);
	infoItem->setFont(0, f);

	infoItem->setFlags(Qt::ItemIsEnabled);

	return infoItem;
}

/*!
 * Retrieve additional information about the song from music sites such as swisscharts, e.g correct spelling of artist/title and release year
 */
void QUWebInfoTree::showInformation(const QString &artist, const QString &title, const QString &genre, const QString &year) {
	if(artist.isEmpty() || title.isEmpty())
		return;

	_artist = artist;
	_title = title;
	_genre = genre;
	_year = year;
	qDeleteAll(swisscharts->takeChildren());
	qDeleteAll(allmusic->takeChildren());
	swisscharts->setHidden(true);
	allmusic->setHidden(true);

	// Retrieve information from swisscharts
	QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(processNetworkReply(QNetworkReply*)));
	QUrl url("http://swisscharts.com/search.asp");
	QUrlQuery urlQuery;
	urlQuery.addQueryItem("cat", "s");
	QString queryString = _artist + " " + _title;
	QStringList queryStrings = queryString.split(QRegExp("(\\s+)"));
	QByteArray encodedQuery;
	foreach(QString queryString, queryStrings) {
		encodedQuery += queryString.toLatin1().toPercentEncoding() + QString("+").toLatin1();
	}
	urlQuery.addQueryItem("search", encodedQuery);
	url.setQuery(urlQuery);
	manager->get(QNetworkRequest(url));
}

void QUWebInfoTree::processNetworkReply(QNetworkReply* reply) {
	if(reply->error() != QNetworkReply::NoError) {
		return;
	}

	qDeleteAll(swisscharts->takeChildren());

	QByteArray newData = reply->readAll();
	QString swisscharts_text = QString::fromUtf8(newData);

	QRegExp rx = QRegExp("<a name=\"itemsmdb\">(.*)</table>");

	rx.setMinimal(true);
	rx.setCaseSensitivity(Qt::CaseInsensitive);

	rx.indexIn(swisscharts_text);
	QString swisscharts_table = QVariant(rx.cap(0)).toString().remove("\r\n");

	rx.setPattern("<tr><td class=\"text\"><a href=.*>.*</a></td><td class=\"text\"><a href=.*>.*</a><a href=.*</td><td class=\"text\">\\s*\\d+</td>.*</tr>");

	int pos = 0;
	QStringList entries;
	while ((pos = rx.indexIn(swisscharts_table, pos)) != -1) {
		entries << rx.cap(0);
		pos += rx.matchedLength();
	}
	if(entries.isEmpty()) {
		swisscharts->addChild(this->createInfoItem(QIcon(":/marks/cross_error.png"), "Error:", "Song not found.", QIcon(":/marks/spell_error.png")));
		swisscharts->setHidden(false);
		return;
	}

	rx.setPattern("<tr><td class=\"text\"><a href=\"(.*)\">(.*)</a></td><td class=\"text\"><a href=.*>(.*)</a><a href=.*</td><td class=\"text\">\\s*(\\d+)</td>.*</tr>");
	QStringList::iterator it = entries.begin();
	while(it != entries.end()) {
		rx.indexIn(*it);

		QIcon icon;

		QString swisscharts_artist = QVariant(rx.cap(2)).toString();
		if(QString::compare(_artist, swisscharts_artist, Qt::CaseSensitive) == 0)
			icon = QIcon(":/marks/spell_ok.png");
		else if(QString::compare(_artist, swisscharts_artist, Qt::CaseInsensitive) == 0)
			icon =  QIcon(":/marks/spell_warn.png");
		else
			icon = QIcon(":/marks/spell_error.png");
		swisscharts->addChild(this->createInfoItem(QIcon(":/types/user.png"), tr("Artist"), swisscharts_artist, icon));

		QString swisscharts_title = QVariant(rx.cap(3)).toString();
		if(QString::compare(_title, swisscharts_title, Qt::CaseSensitive) == 0)
			icon = QIcon(":/marks/spell_ok.png");
		else if(QString::compare(_title, swisscharts_title, Qt::CaseInsensitive) == 0)
			icon = QIcon(":/marks/spell_warn.png");
		else
			icon = QIcon(":/marks/spell_error.png");
		swisscharts->addChild(this->createInfoItem(QIcon(":/types/font.png"), tr("Title"), swisscharts_title, icon));

		QString swisscharts_year = QVariant(rx.cap(4)).toString();
		if(QString::compare(_year, swisscharts_year, Qt::CaseSensitive) == 0)
			icon = QIcon(":/marks/spell_ok.png");
		else
			icon = QIcon(":/marks/spell_error.png");
		swisscharts->addChild(this->createInfoItem(QIcon(":/types/date.png"), tr("Year"), swisscharts_year, icon));

		//QString swisscharts_link = "http://swisscharts.com" + QVariant(rx.cap(1)).toString();
		//swisscharts->addChild(this->createInfoItem(QIcon(":/types/world.png"), tr("Link"), swisscharts_link, QIcon()));

		swisscharts->addChild(this->createInfoItem(QIcon(), "", "", QIcon()));

		++it;
	}

	//swisscharts->addChild(this->createInfoItem(tr("Table"), swisscharts_table));
	swisscharts->setHidden(false);
	//allmusic->setHidden(false);
}
