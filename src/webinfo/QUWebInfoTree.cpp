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
#include "QUStringSupport.h"
#include "QU.h"

QUWebInfoTree::QUWebInfoTree(QWidget *parent): QTreeWidget(parent) {
	this->setColumnCount(3);

	//this->header()->hide();
	this->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
	this->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
	this->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
	//this->header()->setSectionResizeMode(1, QHeaderView::Stretch);

	this->setRootIsDecorated(true);
	this->setIndentation(10);
	connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(applyWebInformationToSong(QTreeWidgetItem*, int)));

	// set up "swisscharts" toplevel item
	_swisscharts = new QTreeWidgetItem();

	this->addTopLevelItem(_swisscharts);

	_swisscharts->setIcon(0, QIcon(":/faviconSwissCharts.ico"));
	_swisscharts->setText(0, tr("swisscharts.com"));
	_swisscharts->setFlags(Qt::ItemIsEnabled);
	_swisscharts->setForeground(0, Qt::darkGray);
	_swisscharts->setFirstColumnSpanned(true);

	_swisscharts->setExpanded(true);
	_swisscharts->setHidden(true);

	// set up "discogs" toplevel item
	_discogs = new QTreeWidgetItem();
	this->addTopLevelItem(_discogs);

	_discogs->setIcon(0, QIcon(":/faviconDiscogs.ico"));
	_discogs->setText(0, tr("discogs.com"));
	_discogs->setFlags(Qt::ItemIsEnabled);
	_discogs->setForeground(0, Qt::darkGray);
	_discogs->setFirstColumnSpanned(true);

	_discogs->setExpanded(true);
	_discogs->setHidden(true);

	// set up "allmusic" toplevel item
	_allmusic = new QTreeWidgetItem();
	this->addTopLevelItem(_allmusic);

	_allmusic->setIcon(0, QIcon(":/faviconAllMusic.ico"));
	_allmusic->setText(0, tr("allmusic.com"));
	_allmusic->setFlags(Qt::ItemIsEnabled);
	_allmusic->setForeground(0, Qt::darkGray);
	_allmusic->setFirstColumnSpanned(true);

	_allmusic->setExpanded(true);
	_allmusic->setHidden(true);

	_manager = new QNetworkAccessManager(this);
	connect(_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(processNetworkReply(QNetworkReply*)));
}

QTreeWidgetItem* QUWebInfoTree::createInfoItem(const QIcon &icon, const QString &tag, const QString &value, const QIcon &status, const QString &toolTip) {
	QTreeWidgetItem *infoItem = new QTreeWidgetItem();

	infoItem->setIcon(0, icon);
	infoItem->setText(0, tag);
	infoItem->setText(1, value);
	infoItem->setIcon(2, status);
	infoItem->setToolTip(0, toolTip);
	infoItem->setToolTip(1, toolTip);
	infoItem->setToolTip(2, toolTip);

	QFont f(infoItem->font(0));
	f.setBold(true);
	infoItem->setFont(0, f);

	infoItem->setFlags(Qt::ItemIsEnabled);

	return infoItem;
}

QTreeWidgetItem* QUWebInfoTree::createInfoItem(const QIcon &icon, const QString &tag, const QString &value, const QU::SpellState &spellState) {
	QTreeWidgetItem *infoItem = new QTreeWidgetItem();

	infoItem->setIcon(0, icon);
	infoItem->setText(0, tag);
	infoItem->setText(1, value);

	QIcon status;
	QString toolTip;
	if(spellState == QU::spellingOk) {
		status = QIcon(":/marks/spell_ok.png");
		toolTip = "";
	} else if(spellState == QU::spellingWarning) {
		status =  QIcon(":/marks/spell_warn.png");
		toolTip = QString("Double-click to change %1 to '%2'.").arg(tag.toLower()).arg(value);
	} else {
		status = QIcon(":/marks/spell_error.png");
		toolTip = QString("Double-click to change %1 to '%2'.").arg(tag.toLower()).arg(value);
	}
	infoItem->setIcon(2, status);
	infoItem->setToolTip(0, toolTip);
	infoItem->setToolTip(1, toolTip);
	infoItem->setToolTip(2, toolTip);
	infoItem->setData(2, Qt::UserRole, QVariant(spellState));

	QFont f(infoItem->font(0));
	f.setBold(true);
	infoItem->setFont(0, f);

	infoItem->setFlags(Qt::ItemIsEnabled);

	return infoItem;
}

/*!
 * Retrieve additional information about the song from music sites such as swisscharts, e.g. correct spelling of artist/title and genre/release year
 */
void QUWebInfoTree::showInformation(QUSongFile *song) {
	if(song->artist().isEmpty() || song->title().isEmpty())
		return;

	_song = song;
	_artist = song->artist();
	_title = song->title();
	_genre = song->genre();
	_year = song->year();

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

	QString queryString = QUStringSupport::simplifiedQueryString(_artist) + " " + QUStringSupport::simplifiedQueryString(_title);
	//qDebug() << "simplified query: " << queryString;

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
	qDeleteAll(_swisscharts->takeChildren());

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

		if(entries.length() == 1) {
			_swisscharts->setText(0, QString("swisscharts.com (%1 entry)").arg(entries.length()));
		} else {
			_swisscharts->setText(0, QString("swisscharts.com (%1 entries)").arg(entries.length()));
		}

		if(entries.isEmpty()) {
			_swisscharts->addChild(this->createInfoItem(QIcon(":/marks/cross_error.png"), "Error:", "Song not found.", QIcon(":/marks/spell_error.png")));
			_swisscharts->setHidden(false);
			return;
		}

		rx1.setPattern("<tr><td class=\"text\"><a href=\"(.*)\">(.*)</a></td><td class=\"text\"><a href=\".*\">(.*)</a>(?:<a href=.*</a>)*</td><td class=\"text\">(?:(\\d{4})|&nbsp;)</td>");
		rx1.setMinimal(true);

		QString swisscharts_url;
		QString swisscharts_artist;
		QString swisscharts_title;
		QString swisscharts_year;

		for(int i = 0; i < entries.length(); ++i) {
			if(rx1.indexIn(entries.at(i)) != -1) {
				QU::SpellState spellState;
				swisscharts_url = QVariant(rx1.cap(1)).toString();
				swisscharts_artist = QVariant(rx1.cap(2)).toString();
				swisscharts_title = QVariant(rx1.cap(3)).toString();
				// fixme: ugly hack, should be fixed via a correct QRegExp pattern/greediness
				if(swisscharts_title.contains("</a>")) {
					swisscharts_title = swisscharts_title.left(swisscharts_title.indexOf("</a"));
				}
				// end fixme
				swisscharts_year = QVariant(rx1.cap(rx1.captureCount())).toString();

				_swisscharts->addChild(this->createInfoItem(QIcon(":/types/music.png"), swisscharts_artist, swisscharts_title, QIcon(), QString()));

				if(QString::compare(_artist, swisscharts_artist, Qt::CaseSensitive) == 0) {
					spellState = QU::spellingOk;
				} else if(QString::compare(_artist, swisscharts_artist, Qt::CaseInsensitive) == 0) {
					spellState = QU::spellingWarning;
				} else {
					spellState = QU::spellingError;
				}
				_swisscharts->child(i)->addChild(this->createInfoItem(QIcon(":/types/user.png"), tr("Artist"), swisscharts_artist, spellState));
				_swisscharts->child(i)->setData(2, Qt::UserRole, QVariant(_swisscharts->child(i)->data(2, Qt::UserRole).toString() + QString::number(spellState)));

				if(QString::compare(_title, swisscharts_title, Qt::CaseSensitive) == 0) {
					spellState = QU::spellingOk;
				} else if(QString::compare(_title, swisscharts_title, Qt::CaseInsensitive) == 0) {
					spellState = QU::spellingWarning;
				} else {
					spellState = QU::spellingError;
				}
				_swisscharts->child(i)->addChild(this->createInfoItem(QIcon(":/types/font.png"), tr("Title"), swisscharts_title, spellState));
				_swisscharts->child(i)->setData(2, Qt::UserRole, QVariant(_swisscharts->child(i)->data(2, Qt::UserRole).toString() + QString::number(spellState)));

				if(QString::compare(_year, swisscharts_year, Qt::CaseSensitive) == 0) {
					spellState = QU::spellingOk;
				} else {
					spellState = QU::spellingError;
				}
				_swisscharts->child(i)->addChild(this->createInfoItem(QIcon(":/types/date.png"), tr("Year"), swisscharts_year, spellState));
				_swisscharts->child(i)->setData(2, Qt::UserRole, QVariant(_swisscharts->child(i)->data(2, Qt::UserRole).toString() + QString::number(spellState)));

				if(_swisscharts->child(i)->data(2, Qt::UserRole).toString().contains('2')) {
					_swisscharts->child(i)->setIcon(2, QIcon(":/marks/spell_error.png"));
				} else if(_swisscharts->child(i)->data(2, Qt::UserRole).toString().contains('1')) {
					_swisscharts->child(i)->setIcon(2, QIcon(":/marks/spell_warn.png"));
				} else {
					_swisscharts->child(i)->setIcon(2, QIcon(":/marks/spell_ok.png"));
				}

				_swisscharts->child(i)->setExpanded(true);
				_swisscharts->child(i)->addChild(this->createInfoItem(QIcon(), "", "", QIcon()));
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

	QString discogs_url;
	QString discogs_artist;
	QString discogs_title;
	rx1.setPattern("<h4><a href=\"(.*)\" class=\"search_result_title\".*>(.*)</a></h4>\\s*<h5>\\s*<span itemprop=\"name\" title=.*>\\s*<a href=\".*\">(.*)</a></span></h5>");
	QStringList::iterator it = entries.begin();
	while(it != entries.end()) {
		//qDebug() << "*it: " << *it;
		if(rx1.indexIn(*it) != -1) {
			discogs_url = "https://www.discogs.com" + QVariant(rx1.cap(1)).toString();
			discogs_title = QVariant(rx1.cap(2)).toString();
			discogs_artist = QVariant(rx1.cap(3)).toString();
			//qDebug() << "Discogs URL: " << discogs_url;
			//qDebug() << "Discogs title: " << discogs_title;
			//qDebug() << "Discogs artist: " << discogs_artist;

			if((_artist.contains(discogs_artist, Qt::CaseInsensitive) || discogs_artist.contains(_artist, Qt::CaseInsensitive)) && (_title.contains(discogs_title, Qt::CaseInsensitive) || discogs_title.contains(_title, Qt::CaseInsensitive))) {
				_manager->get(QNetworkRequest(discogs_url));
				qDebug() << "Discogs URL: " << discogs_url;
				qDebug() << "Discogs title: " << discogs_title;
				qDebug() << "Discogs artist: " << discogs_artist;
			}

			++it;
		}
	}
}

void QUWebInfoTree::processDiscogsSongReply(QNetworkReply* reply) {
	QByteArray newData = reply->readAll();

	QString discogs_reply = QString::fromUtf8(newData);
	QIcon icon;
	QString toolTip;

	QRegExp rx2 = QRegExp("<span itemprop=\"name\" title=\".*\" >\\s*<a href=\".*\">(.+)</a></span>\\s*</span>.*<span itemprop=\"name\">\\s*(.+)\\s*</span>\\s*</h1>\\s*<div class=\"head\">.*:</div>\\s*<div class=\"content\" itemprop=\"genre\">\\s*<a href=\".*\">(.+)</a>\\s*</div>\\s*<div class=\"head\">.*:</div>\\s*<div class=\"content\">\\s*<a href=\".*\">(.+)</a>"); //\\s*</div>\\s*<div class=\"head\">.*:</div>\\s*<div class=\"content\">\\*s<a href=\".*\">(.+)</a>");
	rx2.setMinimal(true);
	rx2.setCaseSensitivity(Qt::CaseInsensitive);

	QString discogs_url;
	QString discogs_artist;
	QString discogs_title;
	QString discogs_genre;
	QString discogs_style;
	QString discogs_year;
	if(rx2.indexIn(discogs_reply) != -1) {
		discogs_artist = QVariant(rx2.cap(1)).toString();
		discogs_title = QVariant(rx2.cap(2)).toString().trimmed();
		discogs_genre = QVariant(rx2.cap(3)).toString();
		discogs_style = QVariant(rx2.cap(4)).toString();
		//discogs_year = QVariant(rx2.cap(5)).toString();

		if(QString::compare(_artist, discogs_artist, Qt::CaseSensitive) == 0) {
			icon = QIcon(":/marks/spell_ok.png");
			toolTip = "";
		} else if(QString::compare(_artist, discogs_artist, Qt::CaseInsensitive) == 0) {
			icon =  QIcon(":/marks/spell_warn.png");
			toolTip = QString("Double-click to change artist from '%1' to '%2'.").arg(_artist).arg(discogs_artist);
		} else {
			icon = QIcon(":/marks/spell_error.png");
			toolTip = QString("Double-click to change artist from '%1' to '%2'.").arg(_artist).arg(discogs_artist);
		}
		_discogs->addChild(this->createInfoItem(QIcon(":/types/user.png"), tr("Artist"), discogs_artist, icon, toolTip));

		if(QString::compare(_title, discogs_title, Qt::CaseSensitive) == 0){
			icon = QIcon(":/marks/spell_ok.png");
			toolTip = "";
		} else if(QString::compare(_title, discogs_title, Qt::CaseInsensitive) == 0) {
			icon = QIcon(":/marks/spell_warn.png");
			toolTip = QString("Double-click to change title from '%1' to '%2'.").arg(_title).arg(discogs_title);
		} else {
			icon = QIcon(":/marks/spell_error.png");
			toolTip = QString("Double-click to change title from '%1' to '%2'.").arg(_title).arg(discogs_title);
		}
		_discogs->addChild(this->createInfoItem(QIcon(":/types/font.png"), tr("Title"), discogs_title, icon, toolTip));

		if(QString::compare(_genre, discogs_genre, Qt::CaseSensitive) == 0) {
			icon = QIcon(":/marks/spell_ok.png");
			toolTip = "";
		} else if(QString::compare(_genre, discogs_genre, Qt::CaseInsensitive) == 0) {
			icon =  QIcon(":/marks/spell_warn.png");
			toolTip = QString("Double-click to change genre from '%1' to '%2'.").arg(_genre).arg(discogs_genre);
		} else {
			icon = QIcon(":/marks/spell_error.png");
			toolTip = QString("Double-click to change genre from '%1' to '%2'.").arg(_genre).arg(discogs_genre);
		}
		_discogs->addChild(this->createInfoItem(QIcon(":/types/genre.png"), tr("Genre"), discogs_genre, icon, toolTip));
		_discogs->addChild(this->createInfoItem(QIcon(":/types/genre.png"), tr("Style"), discogs_style, QIcon()));

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

		QString allmusic_url;
		QString allmusic_title;
		QString allmusic_artist;
		QRegExp rx1 = QRegExp("<li class=\"song\">\\s*<h4>Song</h4>\\s*<div class=\"title\">\\s*<a href=\"(.*)\">&quot;(.*)&quot;</a>\\s*</div>\\s*<div class=\"performers\">\\s*by <a href=\".*\">(.*)</a>\\s*</div>.*</li>");
		QStringList::iterator it = entries.begin();
		while(it != entries.end()) {
			if(rx1.indexIn(*it) != -1) {
				allmusic_url = QVariant(rx1.cap(1)).toString();
				allmusic_title = QVariant(rx1.cap(2)).toString();
				allmusic_artist = QVariant(rx1.cap(3)).toString();

				if((_artist.contains(allmusic_artist, Qt::CaseInsensitive) || allmusic_artist.contains(_artist, Qt::CaseInsensitive)) && (_title.contains(allmusic_title, Qt::CaseInsensitive) || allmusic_title.contains(_title, Qt::CaseInsensitive))) {
					_manager->get(QNetworkRequest(allmusic_url));
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

	QString allmusic_title;
	QString allmusic_artist;
	QString allmusic_genre;

	if(rx2.indexIn(allmusic_reply) != -1) {
		allmusic_artist = QVariant(rx2.cap(1)).toString().trimmed();
		allmusic_title = QVariant(rx2.cap(2)).toString().trimmed();
	}

	rx2.setPattern("<div class=\"genre\">\\s*<h4>Genre</h4>\\s*<div>\\s*<a href=.*>(.*)</a>");

	if(rx2.indexIn(allmusic_reply) != -1) {
		if(QString::compare(_artist, allmusic_artist, Qt::CaseSensitive) == 0)
			icon = QIcon(":/marks/spell_ok.png");
		else if(QString::compare(_artist, allmusic_artist, Qt::CaseInsensitive) == 0)
			icon =  QIcon(":/marks/spell_warn.png");
		else
			icon = QIcon(":/marks/spell_error.png");
		_allmusic->addChild(this->createInfoItem(QIcon(":/types/user.png"), tr("Artist"), allmusic_artist, icon));

		if(QString::compare(_title, allmusic_title, Qt::CaseSensitive) == 0)
			icon = QIcon(":/marks/spell_ok.png");
		else if(QString::compare(_title, allmusic_title, Qt::CaseInsensitive) == 0)
			icon = QIcon(":/marks/spell_warn.png");
		else
			icon = QIcon(":/marks/spell_error.png");
		_allmusic->addChild(this->createInfoItem(QIcon(":/types/font.png"), tr("Title"), allmusic_title, icon));

		allmusic_genre = QVariant(rx2.cap(1)).toString();
		if(QString::compare(_genre, allmusic_genre, Qt::CaseSensitive) == 0)
			icon = QIcon(":/marks/spell_ok.png");
		else if(QString::compare(_artist, allmusic_genre, Qt::CaseInsensitive) == 0)
			icon =  QIcon(":/marks/spell_warn.png");
		else
			icon = QIcon(":/marks/spell_error.png");
		_allmusic->addChild(this->createInfoItem(QIcon(":/types/genre.png"), tr("Genre"), allmusic_genre, icon));

		_allmusic->addChild(this->createInfoItem(QIcon(), "", "", QIcon()));
		_allmusic->setHidden(false);
	}
}

void QUWebInfoTree::applyWebInformationToSong(QTreeWidgetItem *item, int column) {
	if(item->text(0) == "Artist") {
		_song->setInfo(ARTIST_TAG, item->text(1));
	} else if(item->text(0) == "Title") {
		_song->setInfo(TITLE_TAG, item->text(1));
	} else if(item->text(0) == "Genre") {
		_song->setInfo(GENRE_TAG, item->text(1));
	} else if(item->text(0) == "Year") {
		_song->setInfo(YEAR_TAG, item->text(1));
	}
	
	_song->save();
}
