#include "QUWebInfoTree.h"
#include "QUSongFile.h"

#include <QStringList>
#include <QHeaderView>
#include <QFont>
#include <QMessageBox>
#include <QImage>
#include <QTextStream>
#include <QBuffer>
#include <QtNetwork>
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
	_swisscharts = new QTreeWidgetItem();
	this->addTopLevelItem(_swisscharts);

	_swisscharts->setIcon(0, QIcon(":/faviconSwissCharts.ico"));
	_swisscharts->setText(0, tr("swisscharts.com"));
	_swisscharts->setFlags(Qt::ItemIsEnabled);
	_swisscharts->setTextColor(0, Qt::darkGray);
	_swisscharts->setFirstColumnSpanned(true);

	_swisscharts->setExpanded(true);
	_swisscharts->setHidden(true);

	// set up "discogs" toplevel item
	_discogs = new QTreeWidgetItem();
	this->addTopLevelItem(_discogs);

	_discogs->setIcon(0, QIcon(":/faviconDiscogs.ico"));
	_discogs->setText(0, tr("discogs.com"));
	_discogs->setFlags(Qt::ItemIsEnabled);
	_discogs->setTextColor(0, Qt::darkGray);
	_discogs->setFirstColumnSpanned(true);

	_discogs->setExpanded(true);
	_discogs->setHidden(true);

	// set up "allmusic" toplevel item
	_allmusic = new QTreeWidgetItem();
	this->addTopLevelItem(_allmusic);

	_allmusic->setIcon(0, QIcon(":/faviconAllMusic.ico"));
	_allmusic->setText(0, tr("allmusic.com"));
	_allmusic->setFlags(Qt::ItemIsEnabled);
	_allmusic->setTextColor(0, Qt::darkGray);
	_allmusic->setFirstColumnSpanned(true);

	_allmusic->setExpanded(true);
	_allmusic->setHidden(true);

	_manager = new QNetworkAccessManager(this);
	connect(_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(processNetworkReply(QNetworkReply*)));
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

	qDeleteAll(_swisscharts->takeChildren());
	qDeleteAll(_discogs->takeChildren());
	qDeleteAll(_allmusic->takeChildren());

	_swisscharts->setHidden(true);
	_discogs->setHidden(true);
	_allmusic->setHidden(true);

	QSettings settings;
	if(settings.value("swisschartsWebInfo", QVariant(false)).toBool())
		getSwisschartsInformation();
	if(settings.value("discogsWebInfo", QVariant(false)).toBool())
		getDiscogsInformation();
	if(settings.value("allmusicWebInfo", QVariant(false)).toBool())
		getAllmusicInformation();
}

void QUWebInfoTree::getSwisschartsInformation() {
	QUrl url("http://swisscharts.com/search.asp");
	QUrlQuery urlQuery;
	urlQuery.addQueryItem("cat", "s");

	QString simplifiedArtist = _artist;
	simplifiedArtist = simplifiedArtist.remove(QRegExp("\\(.*\\)")).remove(QRegExp(" feat\\..*")).remove(QRegExp(" ft\\..*")).remove(" &").remove(" +").simplified();
	QString simplifiedTitle = _title;
	simplifiedTitle = simplifiedTitle.remove(QRegExp("\\(.*\\)")).simplified();
	QString queryString = simplifiedArtist + " " + simplifiedTitle;
	qDebug() << "simplified query: " << queryString;
	// fixme: possibly remove "(.*)" and "feat. .*"/"vs."/" & " from queryString?
	while(queryString.length() > 50) { // swisscharts.com only allows search queries up to 50 characters
		queryString = queryString.left(queryString.lastIndexOf(' '));
	}
	urlQuery.addQueryItem("search", queryString.toLatin1().toPercentEncoding());
	url.setQuery(urlQuery);
	_manager->get(QNetworkRequest(url));
	//qDebug() << "URL: " + url.toString();
}

void QUWebInfoTree::getDiscogsInformation() {
	_discogs->addChild(this->createInfoItem(QIcon(":/marks/cross_error.png"), "Error:", "not supported yet.", QIcon()));
	_discogs->addChild(this->createInfoItem(QIcon(), "", "", QIcon()));
	_discogs->setHidden(false);
	return;

	// Retrieve information from discogs
	QUrl url("https://www.discogs.com/search/");
	QUrlQuery urlQuery;
	urlQuery.addQueryItem("type", "release");
	//urlQuery.addQueryItem("title", QUrl::toPercentEncoding(_title.split(' ').join('+')));
	//urlQuery.addQueryItem("artist", QUrl::toPercentEncoding(_artist.split(' ').join('+')));
	urlQuery.addQueryItem("q", QString(_artist + " " + _title).split(' ').join('+'));
	url.setQuery(urlQuery);
	QNetworkRequest req(url);
	req.setRawHeader("User-Agent", "UltraStar-Manager/1.8.4 +https://github.com/UltraStar-Deluxe/UltraStar-Manager");
	_manager->get(req);
	qDebug() << "URL: " + url.toString();
	//QList<QByteArray> headerList = req.rawHeaderList();
	//foreach(QByteArray head, headerList) {
	//	qDebug() << head << ":" << req.rawHeader(head);
	//}
}

void QUWebInfoTree::getAllmusicInformation() {
	_allmusic->addChild(this->createInfoItem(QIcon(":/marks/cross_error.png"), "Error:", "not supported yet.", QIcon()));
	_allmusic->setHidden(false);
	return;

	QUrl url("http://www.allmusic.com/search/songs/" + _artist.split(QRegExp("\\s+"), QString::SkipEmptyParts).join('+') + "+" + _title.split(QRegExp("\\s+"), QString::SkipEmptyParts).join('+'));
	_manager->get(QNetworkRequest(url));
}

void QUWebInfoTree::processNetworkReply(QNetworkReply* reply) {
	if(reply->url().toString().contains("swisscharts")) {
		processSwisschartsReply(reply);
	} else
	if(reply->url().toString().contains("allmusic.com")) {
		processAllmusicReply(reply);
	} else
	if(reply->url().toString().contains("discogs.com")) {
		processDiscogsReply(reply);
	}
}

void QUWebInfoTree::processSwisschartsReply(QNetworkReply* reply) {
	if(reply->error() != QNetworkReply::NoError) {
		qDebug() << reply->errorString();
		_swisscharts->addChild(this->createInfoItem(QIcon(":/marks/cross_error.png"), "Error:", reply->errorString(), QIcon()));
		_swisscharts->setHidden(false);
		return;
	}

	QByteArray newData = reply->readAll();
	QString swisscharts_reply = QString::fromLatin1(newData);

	QRegExp rx1 = QRegExp("<a name=\"itemsmdb\">(.*)</table>");
	rx1.setMinimal(true);
	rx1.setCaseSensitivity(Qt::CaseInsensitive);

	if(rx1.indexIn(swisscharts_reply) != -1) {
		QString swisscharts_results = QVariant(rx1.cap(0)).toString().remove("\r\n");

		rx1.setPattern("<tr><td class=\"text\"><a href=\".*\">.*</a></td><td class=\"text\">(?:<a href=.*</a>\\s*)+</td><td class=\"text\">(?:\\d{4}|&nbsp;)</td>.*</tr>");
		rx1.setMinimal(true);

		int pos = 0;
		QStringList entries;
		while ((pos = rx1.indexIn(swisscharts_results, pos)) != -1) {
			entries << rx1.cap(0);
			pos += rx1.matchedLength();
		}
		if(entries.isEmpty()) {
			_swisscharts->addChild(this->createInfoItem(QIcon(":/marks/cross_error.png"), "Error:", "Song not found.", QIcon(":/marks/spell_error.png")));
			_swisscharts->setHidden(false);
			return;
		}

		rx1.setPattern("<tr><td class=\"text\"><a href=\"(.*)\">(.*)</a></td><td class=\"text\"><a href=\".*\">(.*)</a>(?:<a href=.*</a>)*</td><td class=\"text\">(?:(\\d{4})|&nbsp;)</td>");
		rx1.setMinimal(true);

		QStringList::iterator it = entries.begin();
		while(it != entries.end()) {
			if(rx1.indexIn(*it) != -1) {
				QIcon icon;
				_swisscharts_url = QVariant(rx1.cap(1)).toString();
				_swisscharts_artist = QVariant(rx1.cap(2)).toString();
				_swisscharts_title = QVariant(rx1.cap(3)).toString();
				// fixme: ugly hack, should be fixed via a correct QRegExp pattern/greediness
				if(_swisscharts_title.contains("</a>")) {
					_swisscharts_title = _swisscharts_title.left(_swisscharts_title.indexOf("</a"));
				}
				// end fixme
				_swisscharts_year = QVariant(rx1.cap(rx1.captureCount())).toString();

				if(QString::compare(_artist, _swisscharts_artist, Qt::CaseSensitive) == 0)
					icon = QIcon(":/marks/spell_ok.png");
				else if(QString::compare(_artist, _swisscharts_artist, Qt::CaseInsensitive) == 0)
					icon =  QIcon(":/marks/spell_warn.png");
				else
					icon = QIcon(":/marks/spell_error.png");
				_swisscharts->addChild(this->createInfoItem(QIcon(":/types/user.png"), tr("Artist"), _swisscharts_artist, icon));

				if(QString::compare(_title, _swisscharts_title, Qt::CaseSensitive) == 0)
					icon = QIcon(":/marks/spell_ok.png");
				else if(QString::compare(_title, _swisscharts_title, Qt::CaseInsensitive) == 0)
					icon = QIcon(":/marks/spell_warn.png");
				else
					icon = QIcon(":/marks/spell_error.png");
				_swisscharts->addChild(this->createInfoItem(QIcon(":/types/font.png"), tr("Title"), _swisscharts_title, icon));

				if(QString::compare(_year, _swisscharts_year, Qt::CaseSensitive) == 0)
					icon = QIcon(":/marks/spell_ok.png");
				else
					icon = QIcon(":/marks/spell_error.png");
				_swisscharts->addChild(this->createInfoItem(QIcon(":/types/date.png"), tr("Year"), _swisscharts_year, icon));

				_swisscharts->addChild(this->createInfoItem(QIcon(), "", "", QIcon()));

				++it;
			}
		}
	} else {
		_swisscharts->addChild(this->createInfoItem(QIcon(":/marks/cross_error.png"), "Error:", "Song not found.", QIcon(":/marks/spell_error.png")));
	}

	_swisscharts->setHidden(false);
}

void QUWebInfoTree::processDiscogsReply(QNetworkReply* reply) {
	if(reply->error() != QNetworkReply::NoError) {
		QList<QByteArray> headerList = reply->rawHeaderList();
		foreach(QByteArray head, headerList) {
			qDebug() << head << ":" << reply->rawHeader(head);
		}
		_discogs->addChild(this->createInfoItem(QIcon(":/marks/cross_error.png"), "Error:", reply->errorString(), QIcon()));
		_discogs->setHidden(false);
		return;
	}

	if((reply->url().toString().contains("discogs.com")) && (reply->url().toString().contains("search"))) {
		processDiscogsSearchReply(reply);
	} else if((reply->url().toString().contains("discogs.com")) && (reply->url().toString().contains("master"))) {
		processDiscogsSongReply(reply);
	}
}

void QUWebInfoTree::processDiscogsSearchReply(QNetworkReply* reply) {
	QByteArray newData = reply->readAll();
	QString discogs_reply = QString(newData.data());
	qDebug() << "Discogs reply received!";
	qDebug() << "Reply: " << discogs_reply;

	QRegExp rx1 = QRegExp("<h4><a href=\".*\" class=\"search_result_title\".*>.*</a></h4>\\s*<h5>\\s*<span itemprop=\"name\" title=.*>\\s*<a href=\".*\">.*</a></span></h5>");
	rx1.setMinimal(true);
	rx1.setCaseSensitivity(Qt::CaseInsensitive);

	int pos = 0;
	QStringList entries;
	while ((pos = rx1.indexIn(discogs_reply, pos)) != -1) {
		entries << rx1.cap(0);
		pos += rx1.matchedLength();
	}
	if(entries.isEmpty()) {
		_discogs->addChild(this->createInfoItem(QIcon(":/marks/cross_error.png"), "Error:", "Song not found.", QIcon(":/marks/spell_error.png")));
		_discogs->setHidden(false);
		return;
	}

	rx1.setPattern("<h4><a href=\"(.*)\" class=\"search_result_title\".*>(.*)</a></h4>\\s*<h5>\\s*<span itemprop=\"name\" title=.*>\\s*<a href=\".*\">(.*)</a></span></h5>");
	QStringList::iterator it = entries.begin();
	while(it != entries.end()) {
		//qDebug() << "*it: " << *it;
		if(rx1.indexIn(*it) != -1) {
			_discogs_url = "https://www.discogs.com" + QVariant(rx1.cap(1)).toString();
			_discogs_title = QVariant(rx1.cap(2)).toString();
			_discogs_artist = QVariant(rx1.cap(3)).toString();
			//qDebug() << "Discogs URL: " << _discogs_url;
			//qDebug() << "Discogs title: " << _discogs_title;
			//qDebug() << "Discogs artist: " << _discogs_artist;

			if((_artist.contains(_discogs_artist, Qt::CaseInsensitive) || _discogs_artist.contains(_artist, Qt::CaseInsensitive)) && (_title.contains(_discogs_title, Qt::CaseInsensitive) || _discogs_title.contains(_title, Qt::CaseInsensitive))) {
				_manager->get(QNetworkRequest(_discogs_url));
				qDebug() << "Discogs URL: " << _discogs_url;
				qDebug() << "Discogs title: " << _discogs_title;
				qDebug() << "Discogs artist: " << _discogs_artist;
			}

			++it;
		}
	}
}

void QUWebInfoTree::processDiscogsSongReply(QNetworkReply* reply) {
	QByteArray newData = reply->readAll();

	QString discogs_reply = QString::fromUtf8(newData);
	QIcon icon;

	QRegExp rx2 = QRegExp("<span itemprop=\"name\" title=\".*\" >\\s*<a href=\".*\">(.+)</a></span>\\s*</span>.*<span itemprop=\"name\">\\s*(.+)\\s*</span>\\s*</h1>\\s*<div class=\"head\">.*:</div>\\s*<div class=\"content\" itemprop=\"genre\">\\s*<a href=\".*\">(.+)</a>\\s*</div>\\s*<div class=\"head\">.*:</div>\\s*<div class=\"content\">\\s*<a href=\".*\">(.+)</a>"); //\\s*</div>\\s*<div class=\"head\">.*:</div>\\s*<div class=\"content\">\\*s<a href=\".*\">(.+)</a>");
	rx2.setMinimal(true);
	rx2.setCaseSensitivity(Qt::CaseInsensitive);

	if(rx2.indexIn(discogs_reply) != -1) {
		_discogs_artist = QVariant(rx2.cap(1)).toString();
		_discogs_title = QVariant(rx2.cap(2)).toString().trimmed();
		_discogs_genre = QVariant(rx2.cap(3)).toString();
		_discogs_style = QVariant(rx2.cap(4)).toString();
		//_discogs_year = QVariant(rx2.cap(5)).toString();

		if(QString::compare(_artist, _discogs_artist, Qt::CaseSensitive) == 0)
			icon = QIcon(":/marks/spell_ok.png");
		else if(QString::compare(_artist, _discogs_artist, Qt::CaseInsensitive) == 0)
			icon =  QIcon(":/marks/spell_warn.png");
		else
			icon = QIcon(":/marks/spell_error.png");
		_discogs->addChild(this->createInfoItem(QIcon(":/types/user.png"), tr("Artist"), _discogs_artist, icon));

		if(QString::compare(_title, _discogs_title, Qt::CaseSensitive) == 0)
			icon = QIcon(":/marks/spell_ok.png");
		else if(QString::compare(_title, _discogs_title, Qt::CaseInsensitive) == 0)
			icon = QIcon(":/marks/spell_warn.png");
		else
			icon = QIcon(":/marks/spell_error.png");
		_discogs->addChild(this->createInfoItem(QIcon(":/types/font.png"), tr("Title"), _discogs_title, icon));

		if(QString::compare(_genre, _discogs_genre, Qt::CaseSensitive) == 0)
			icon = QIcon(":/marks/spell_ok.png");
		else if(QString::compare(_artist, _discogs_genre, Qt::CaseInsensitive) == 0)
			icon =  QIcon(":/marks/spell_warn.png");
		else
			icon = QIcon(":/marks/spell_error.png");
		_discogs->addChild(this->createInfoItem(QIcon(":/types/genre.png"), tr("Genre"), _discogs_genre, icon));
		_discogs->addChild(this->createInfoItem(QIcon(":/types/genre.png"), tr("Style"), _discogs_style, QIcon()));

		/*
		if(QString::compare(_year, _discogs_year, Qt::CaseSensitive) == 0)
			icon = QIcon(":/marks/spell_ok.png");
		else
			icon = QIcon(":/marks/spell_error.png");
		_discogs->addChild(this->createInfoItem(QIcon(":/types/date.png"), tr("Year"), _discogs_year, icon));
		*/

		_discogs->addChild(this->createInfoItem(QIcon(), "", "", QIcon()));
		_discogs->setHidden(false);
	} else {
		qDebug() << "Information NOT found.";
		_discogs->addChild(this->createInfoItem(QIcon(":/marks/cross_error.png"), "Error", "Song not found.", QIcon(":/marks/spell_error.png")));
		_discogs->setHidden(false);
	}
}

void QUWebInfoTree::processAllmusicReply(QNetworkReply* reply) {
	if(reply->error() != QNetworkReply::NoError) {
		qDebug() << reply->errorString();
		_allmusic->addChild(this->createInfoItem(QIcon(":/marks/cross_error.png"), "Error", reply->errorString(), QIcon()));
		_allmusic->setHidden(false);
		return;
	}

	if(reply->url().toString().contains("allmusic.com/search")) {
		processAllmusicSearchReply(reply);
	} else if(reply->url().toString().contains("allmusic.com/song")) {
		processAllmusicSongReply(reply);
	}
}

void QUWebInfoTree::processAllmusicSearchReply(QNetworkReply* reply) {
	QByteArray newData = reply->readAll();
	QString allmusic_reply = QString(newData.data());

	QRegExp rx1 = QRegExp("<ul class=\"search-results\">.*</ul>");
	rx1.setMinimal(true);
	rx1.setCaseSensitivity(Qt::CaseInsensitive);

	if(rx1.indexIn(allmusic_reply) != -1) {
		QString allmusic_results = QVariant(rx1.cap(0)).toString();

		rx1.setPattern("<li class=\"song\">\\s*<h4>Song</h4>\\s*<div class=\"title\">\\s*<a href=\".*\">&quot;.*&quot;</a>\\s*</div>\\s*<div class=\"performers\">\\s*by <a href=\".*\">.*</a>\\s*</div>.*</li>");

		int pos = 0;
		QStringList entries;
		while ((pos = rx1.indexIn(allmusic_results, pos)) != -1) {
			entries << rx1.cap(0);
			pos += rx1.matchedLength();
		}
		if(entries.isEmpty()) {
			_allmusic->addChild(this->createInfoItem(QIcon(":/marks/cross_error.png"), "Error:", "Song not found.", QIcon(":/marks/spell_error.png")));
			_allmusic->setHidden(false);
			return;
		}

		QRegExp rx1 = QRegExp("<li class=\"song\">\\s*<h4>Song</h4>\\s*<div class=\"title\">\\s*<a href=\"(.*)\">&quot;(.*)&quot;</a>\\s*</div>\\s*<div class=\"performers\">\\s*by <a href=\".*\">(.*)</a>\\s*</div>.*</li>");
		QStringList::iterator it = entries.begin();
		while(it != entries.end()) {
			if(rx1.indexIn(*it) != -1) {
				_allmusic_title = QVariant(rx1.cap(2)).toString();
				_allmusic_artist = QVariant(rx1.cap(3)).toString();

				if((_artist.contains(_allmusic_artist, Qt::CaseInsensitive) || _allmusic_artist.contains(_artist, Qt::CaseInsensitive)) && (_title.contains(_allmusic_title, Qt::CaseInsensitive) || _allmusic_title.contains(_title, Qt::CaseInsensitive))) {
					_allmusic_url = QVariant(rx1.cap(1)).toString();
					_manager->get(QNetworkRequest(_allmusic_url));
				}

				++it;
			}
		}
	} else {
		qDebug() << "No search results!";
		_allmusic->addChild(this->createInfoItem(QIcon(":/marks/cross_error.png"), "Error:", "Song not found.", QIcon(":/marks/spell_error.png")));
		_allmusic->setHidden(false);
		return;
	}
}

void QUWebInfoTree::processAllmusicSongReply(QNetworkReply* reply) {
	QByteArray newData = reply->readAll();
	QString allmusic_reply = QString::fromUtf8(newData);
	QIcon icon;

	QRegExp rx2 = QRegExp("<hgroup>\\s*<h2 class=\"song-artist\".*<a href=\".*\">(.*)</a>\\s*</span>\\s*</h2>\\s*<h1 class=\"song-title\".*>\\s*(.*)\\s*</h1>");
	rx2.setMinimal(true);
	rx2.setCaseSensitivity(Qt::CaseInsensitive);

	if(rx2.indexIn(allmusic_reply) != -1) {
		_allmusic_artist = QVariant(rx2.cap(1)).toString().trimmed();
		_allmusic_title = QVariant(rx2.cap(2)).toString().trimmed();
	}

	rx2.setPattern("<div class=\"genre\">\\s*<h4>Genre</h4>\\s*<div>\\s*<a href=.*>(.*)</a>");

	if(rx2.indexIn(allmusic_reply) != -1) {
		if(QString::compare(_artist, _allmusic_artist, Qt::CaseSensitive) == 0)
			icon = QIcon(":/marks/spell_ok.png");
		else if(QString::compare(_artist, _allmusic_artist, Qt::CaseInsensitive) == 0)
			icon =  QIcon(":/marks/spell_warn.png");
		else
			icon = QIcon(":/marks/spell_error.png");
		_allmusic->addChild(this->createInfoItem(QIcon(":/types/user.png"), tr("Artist"), _allmusic_artist, icon));

		if(QString::compare(_title, _allmusic_title, Qt::CaseSensitive) == 0)
			icon = QIcon(":/marks/spell_ok.png");
		else if(QString::compare(_title, _allmusic_title, Qt::CaseInsensitive) == 0)
			icon = QIcon(":/marks/spell_warn.png");
		else
			icon = QIcon(":/marks/spell_error.png");
		_allmusic->addChild(this->createInfoItem(QIcon(":/types/font.png"), tr("Title"), _allmusic_title, icon));

		_allmusic_genre = QVariant(rx2.cap(1)).toString();
		if(QString::compare(_genre, _allmusic_genre, Qt::CaseSensitive) == 0)
			icon = QIcon(":/marks/spell_ok.png");
		else if(QString::compare(_artist, _allmusic_genre, Qt::CaseInsensitive) == 0)
			icon =  QIcon(":/marks/spell_warn.png");
		else
			icon = QIcon(":/marks/spell_error.png");
		_allmusic->addChild(this->createInfoItem(QIcon(":/types/genre.png"), tr("Genre"), _allmusic_genre, icon));

		_allmusic->addChild(this->createInfoItem(QIcon(), "", "", QIcon()));
		_allmusic->setHidden(false);
	}
}
