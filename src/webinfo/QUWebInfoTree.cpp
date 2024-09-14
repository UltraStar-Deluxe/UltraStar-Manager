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
#include <QRegularExpression>
#include <QDebug>

#include "QUSongSupport.h"
#include "QUStringSupport.h"
#include "QU.h"

#include "compact_lang_det.h"

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

	// set up language toplevel item
	_cld2 = new QTreeWidgetItem();
	
	this->addTopLevelItem(_cld2);
	
	_cld2->setIcon(0, QIcon(":/types/language.png"));
	_cld2->setText(0, tr("Compact Language Detection"));
	_cld2->setFlags(Qt::ItemIsEnabled);
	_cld2->setForeground(0, Qt::darkGray);
	_cld2->setFirstColumnSpanned(true);
	
	_cld2->setExpanded(true);
	_cld2->setHidden(true);
	
	// set up "hitparade.ch" toplevel item
	_hitparade = new QTreeWidgetItem();

	this->addTopLevelItem(_hitparade);

	_hitparade->setIcon(0, QIcon(":/faviconHitparade.png"));
	_hitparade->setText(0, tr("hitparade.ch"));
	_hitparade->setFlags(Qt::ItemIsEnabled);
	_hitparade->setForeground(0, Qt::darkGray);
	_hitparade->setFirstColumnSpanned(true);

	_hitparade->setExpanded(true);
	_hitparade->setHidden(true);

	// set up "discogs" toplevel item
	_discogs = new QTreeWidgetItem();
	this->addTopLevelItem(_discogs);

	_discogs->setIcon(0, QIcon(":/faviconDiscogs.png"));
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

QTreeWidgetItem* QUWebInfoTree::createInfoItem(const QIcon &icon, const QString &tag, const QString &value, const QIcon &status, const QString &score, const QString &toolTip) {
	QTreeWidgetItem *infoItem = new QTreeWidgetItem();

	infoItem->setIcon(0, icon);
	infoItem->setText(0, tag);
	infoItem->setText(1, value);
	infoItem->setIcon(2, status);
	infoItem->setText(3, score);
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
 * Retrieve additional information about the song from music sites such as hitparade.ch, e.g. correct spelling of artist/title and genre/release year
 */
void QUWebInfoTree::showInformation(QUSongFile *song) {
	if(song->artist().isEmpty() || song->title().isEmpty())
		return;

	_song = song;
	_artist = song->artist();
	_title = song->title();
	_language = song->language();
	_genre = song->genre();
	_year = song->year();

	qDeleteAll(_cld2->takeChildren());
	qDeleteAll(_hitparade->takeChildren());
	qDeleteAll(_discogs->takeChildren());
	qDeleteAll(_allmusic->takeChildren());

	_cld2->setHidden(true);
	_hitparade->setHidden(true);
	_discogs->setHidden(true);
	_allmusic->setHidden(true);

	QSettings settings;
	if(settings.value("cld2Info", QVariant(true)).toBool())
		getCld2Information();
	if(settings.value("hitparadeWebInfo", QVariant(false)).toBool())
		getHitparadeInformation();
	if(settings.value("discogsWebInfo", QVariant(false)).toBool())
		getDiscogsInformation();
	if(settings.value("allmusicWebInfo", QVariant(false)).toBool())
		getAllmusicInformation();
}

void QUWebInfoTree::getCld2Information() {
	int threshold = 10; // 10 % of song lyrics need to identified as a certain language to count
	bool isReliable;
	QU::SpellState spellState;
	QString cld2_language;
	QStringList cld2_languages;
	QStringList cld2_percentages;
	//qDebug() << _song->lyrics().join("").remove(QChar('~'));
	//CLD2::Language cld2_lang = CLD2::DetectLanguage(_song->lyrics().join("").remove(QChar('~'), Qt::CaseInsensitive).toStdString().c_str(), _song->lyrics().join("").toStdString().length(), false, &isReliable);
	
	CLD2::Language language3[3];
	int percent3[3];
	int text_bytes;
	std::string songText = _song->lyrics().join("").remove(QChar('~'), Qt::CaseInsensitive).toStdString();

	CLD2::Language cld2_lang = CLD2::DetectLanguageSummary(songText.c_str(), songText.length(), false, language3, percent3, &text_bytes, &isReliable);
	
	if(isReliable) {
		for(int i = 0; i < 3; ++i) {
			if(language3[i] != CLD2::UNKNOWN_LANGUAGE && percent3[i] > threshold) {
				cld2_language = QString::fromUtf8(CLD2::LanguageDeclaredName(language3[i])).toLower();
				cld2_language[0] = cld2_language[0].toUpper();
				cld2_languages.append(cld2_language);
				cld2_percentages.append(QString("(%1 %)").arg(percent3[i]));
			}
		}
		
		if(cld2_languages.length() > 1) {
			cld2_language = cld2_languages.join(", ");
		}
		
		if(QString::compare(_language, cld2_language, Qt::CaseSensitive) == 0) {
			spellState = QU::spellingOk;
		} else if(QString::compare(_language, cld2_language, Qt::CaseInsensitive) == 0) {
			spellState = QU::spellingWarning;
		} else {
			spellState = QU::spellingError;
		}
		
		_cld2->addChild(this->createInfoItem(QIcon(":/types/language.png"), tr("Language"), cld2_language, spellState));
		_cld2->child(0)->setToolTip(2, _cld2->child(0)->toolTip(2) + cld2_percentages.join(", "));
		_cld2->setHidden(false);
		if(spellState != QU::spellingOk) {
			_cld2->child(0)->setExpanded(true);
		}
		
		// if more than one language is identified, add children for each language
		if(cld2_languages.length() > 1) {
			for(int i = 0; i < cld2_languages.length(); ++i) {
				if(QString::compare(_language, cld2_languages.at(i), Qt::CaseSensitive) == 0) {
					spellState = QU::spellingOk;
				} else if(QString::compare(_language, cld2_languages.at(i), Qt::CaseInsensitive) == 0) {
					spellState = QU::spellingWarning;
				} else {
					spellState = QU::spellingError;
				}
				
				_cld2->child(0)->addChild(this->createInfoItem(QIcon(":/types/language.png"), tr("Language"), cld2_languages[i], spellState));
				_cld2->child(0)->child(i)->setToolTip(2, _cld2->child(0)->child(i)->toolTip(2) + cld2_percentages.at(i));
				_cld2->child(0)->setText(3, QString::number(spellState));
			}
			//_cld2->child(0)->setExpanded(true);
		}
	} else {
		_cld2->addChild(this->createInfoItem(QIcon(":/types/language.png"), tr("Language"), tr("unreliable"), QU::spellingError));
		_cld2->setHidden(false);
	}
}

void QUWebInfoTree::getHitparadeInformation() {
	QUrl url("http://hitparade.ch/search.asp");
	QUrlQuery urlQuery;
	urlQuery.addQueryItem("cat", "s");

	QString queryString = QUStringSupport::simplifiedQueryString(_artist) + " " + QUStringSupport::simplifiedQueryString(_title);
	//qDebug() << "simplified query: " << queryString;

	while(queryString.length() > 50) { // hitparade.ch only allows search queries up to 50 characters
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

	QUrl url("http://www.allmusic.com/search/songs/" + _artist.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts).join('+') + "+" + _title.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts).join('+'));
	_manager->get(QNetworkRequest(url));
}

void QUWebInfoTree::processNetworkReply(QNetworkReply* reply) {
	if(reply->url().toString().contains("hitparade")) {
		processHitparadeReply(reply);
	} else
	if(reply->url().toString().contains("allmusic.com")) {
		processAllmusicReply(reply);
	} else
	if(reply->url().toString().contains("discogs.com")) {
		processDiscogsReply(reply);
	}
}

void QUWebInfoTree::processHitparadeReply(QNetworkReply* reply) {
	qDeleteAll(_hitparade->takeChildren());

	if(reply->error() != QNetworkReply::NoError) {
		qDebug() << reply->errorString();
		_hitparade->addChild(this->createInfoItem(QIcon(":/marks/cross_error.png"), "Error:", reply->errorString(), QIcon()));
		_hitparade->setHidden(false);
		return;
	}
	
	QByteArray newData = reply->readAll();
	QString searchresult = QString::fromLatin1(newData).remove("\r\n");
	
	QRegularExpression re = QRegularExpression("<tr><td.*?><a href=\"/song/.*?\">(.*?)</a></td.*?>.*?<a href=\"/song/.*?\">(.*?)</a>.*?<td.*?>(\\d{4}|&nbsp;|\\s*)</td>", QRegularExpression::DotMatchesEverythingOption);
	QRegularExpressionMatchIterator mi = re.globalMatch(searchresult);
	
	if (mi.hasNext()) {
		int i = 0;
		while (mi.hasNext()) {
			QRegularExpressionMatch match = mi.next();
		
			QString hitparade_artist = match.captured(1);
			QString hitparade_title = match.captured(2);
			QString hitparade_year = match.captured(3).toInt() != 0 ? match.captured(3) : "";
			
			_hitparade->addChild(this->createInfoItem(QIcon(":/types/music.png"), hitparade_artist, hitparade_title, QIcon(), QString()));
	
			QU::SpellState spellState;
			if(QString::compare(_artist.replace("’", "'"), hitparade_artist, Qt::CaseSensitive) == 0) {
				spellState = QU::spellingOk;
			} else if(QString::compare(_artist.replace("’", "'"), hitparade_artist, Qt::CaseInsensitive) == 0) {
				spellState = QU::spellingWarning;
			} else {
				spellState = QU::spellingError;
			}
			_hitparade->child(i)->addChild(this->createInfoItem(QIcon(":/types/user.png"), tr("Artist"), hitparade_artist, spellState));
			_hitparade->child(i)->setText(3, _hitparade->child(i)->text(3) + QString::number(spellState));
	
			if(QString::compare(_title.replace("’", "'"), hitparade_title, Qt::CaseSensitive) == 0) {
				spellState = QU::spellingOk;
			} else if(QString::compare(_title.replace("’", "'"), hitparade_title, Qt::CaseInsensitive) == 0) {
				spellState = QU::spellingWarning;
			} else {
				spellState = QU::spellingError;
			}
			_hitparade->child(i)->addChild(this->createInfoItem(QIcon(":/types/font.png"), tr("Title"), hitparade_title, spellState));
			_hitparade->child(i)->setText(3, _hitparade->child(i)->text(3) + QString::number(spellState));
	
			if(QString::compare(_year, hitparade_year, Qt::CaseSensitive) == 0) {
				spellState = QU::spellingOk;
			} else {
				spellState = QU::spellingError;
			}
			_hitparade->child(i)->addChild(this->createInfoItem(QIcon(":/types/date.png"), tr("Year"), hitparade_year, spellState));
			_hitparade->child(i)->setText(3, _hitparade->child(i)->text(3) + QString::number(spellState));
	
			// column 3 data contains score string "xyz" where x = artist spellState, y = title spellState, z = year spellState (perfect match = "000")
			if(_hitparade->child(i)->text(3).contains('2')) {
				_hitparade->child(i)->setIcon(2, QIcon(":/marks/spell_error.png"));
			} else if(_hitparade->child(i)->text(3).contains('1')) {
				_hitparade->child(i)->setIcon(2, QIcon(":/marks/spell_warn.png"));
			} else {
				_hitparade->child(i)->setIcon(2, QIcon(":/marks/spell_ok.png"));
			}
	
			if(_hitparade->child(i)->text(3) == "222") {
				_hitparade->child(i)->setExpanded(false);
			} else {
				_hitparade->child(i)->setExpanded(true);
			}
			_hitparade->child(i)->addChild(this->createInfoItem(QIcon(), "", "", QIcon()));
			++i;
		}
	} else {
		_hitparade->addChild(this->createInfoItem(QIcon(":/marks/cross_error.png"), "Error:", "Song not found.", QIcon(":/marks/spell_error.png")));
		_hitparade->setHidden(false);
		return;
	}
	
	_hitparade->sortChildren(3, Qt::AscendingOrder);
	_hitparade->setHidden(false);
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

	QRegularExpression rx1 = QRegularExpression("<h4><a href=\".*\" class=\"search_result_title\".*>.*</a></h4>\\s*<h5>\\s*<span itemprop=\"name\" title=.*>\\s*<a href=\".*\">.*</a></span></h5>", QRegularExpression::InvertedGreedinessOption | QRegularExpression::CaseInsensitiveOption);
	QRegularExpressionMatchIterator i = rx1.globalMatch(discogs_reply);
	
	QStringList entries;
	while (i.hasNext()) {
		QRegularExpressionMatch match = i.next();
		entries << match.captured(0);
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
		QRegularExpressionMatch match = rx1.match(*it);
		if(match.hasMatch()) {
			discogs_url = "https://www.discogs.com" + QVariant(match.captured(1)).toString();
			discogs_title = QVariant(match.captured(2)).toString();
			discogs_artist = QVariant(match.captured(3)).toString();
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

	QRegularExpression rx2 = QRegularExpression("<span itemprop=\"name\" title=\".*\" >\\s*<a href=\".*\">(.+)</a></span>\\s*</span>.*<span itemprop=\"name\">\\s*(.+)\\s*</span>\\s*</h1>\\s*<div class=\"head\">.*:</div>\\s*<div class=\"content\" itemprop=\"genre\">\\s*<a href=\".*\">(.+)</a>\\s*</div>\\s*<div class=\"head\">.*:</div>\\s*<div class=\"content\">\\s*<a href=\".*\">(.+)</a>", QRegularExpression::InvertedGreedinessOption | QRegularExpression::CaseInsensitiveOption);
	QRegularExpressionMatch match = rx2.match(discogs_reply);
	
	QString discogs_url;
	QString discogs_artist;
	QString discogs_title;
	QString discogs_genre;
	QString discogs_style;
	QString discogs_year;
	if(match.hasMatch()) {
		discogs_artist = QVariant(match.captured(1)).toString();
		discogs_title = QVariant(match.captured(2)).toString().trimmed();
		discogs_genre = QVariant(match.captured(3)).toString();
		discogs_style = QVariant(match.captured(4)).toString();
		//discogs_year = QVariant(match.captured(5)).toString();

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

	QRegularExpression rx1 = QRegularExpression("<ul class=\"search-results\">.*</ul>", QRegularExpression::InvertedGreedinessOption | QRegularExpression::CaseInsensitiveOption);
	QRegularExpressionMatch match = rx1.match(allmusic_reply);
	
	if(match.hasMatch()) {
		QString allmusic_results = QVariant(match.captured(0)).toString();

		rx1.setPattern("<li class=\"song\">\\s*<h4>Song</h4>\\s*<div class=\"title\">\\s*<a href=\".*\">&quot;.*&quot;</a>\\s*</div>\\s*<div class=\"performers\">\\s*by <a href=\".*\">.*</a>\\s*</div>.*</li>");
		QRegularExpressionMatchIterator mi = rx1.globalMatch(allmusic_reply);
		
		QStringList entries;
		while (mi.hasNext()) {
			QRegularExpressionMatch match = mi.next();
			entries << match.captured(0);
		}
		if(entries.isEmpty()) {
			_allmusic->addChild(this->createInfoItem(QIcon(":/marks/cross_error.png"), "Error:", "Song not found.", QIcon(":/marks/spell_error.png")));
			_allmusic->setHidden(false);
			return;
		}

		QString allmusic_url;
		QString allmusic_title;
		QString allmusic_artist;
		QRegularExpression rx1 = QRegularExpression("<li class=\"song\">\\s*<h4>Song</h4>\\s*<div class=\"title\">\\s*<a href=\"(.*)\">&quot;(.*)&quot;</a>\\s*</div>\\s*<div class=\"performers\">\\s*by <a href=\".*\">(.*)</a>\\s*</div>.*</li>");
		QStringList::iterator it = entries.begin();
		while(it != entries.end()) {
			QRegularExpressionMatch match = rx1.match(*it);
			if(match.hasMatch()) {
				allmusic_url = QVariant(match.captured(1)).toString();
				allmusic_title = QVariant(match.captured(2)).toString();
				allmusic_artist = QVariant(match.captured(3)).toString();

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

	QRegularExpression rx2 = QRegularExpression("<hgroup>\\s*<h2 class=\"song-artist\".*<a href=\".*\">(.*)</a>\\s*</span>\\s*</h2>\\s*<h1 class=\"song-title\".*>\\s*(.*)\\s*</h1>", QRegularExpression::InvertedGreedinessOption | QRegularExpression::CaseInsensitiveOption);
	QRegularExpressionMatch match = rx2.match(allmusic_reply);
	
	QString allmusic_title;
	QString allmusic_artist;
	QString allmusic_genre;

	if(match.hasMatch()) {
		allmusic_artist = QVariant(match.captured(1)).toString().trimmed();
		allmusic_title = QVariant(match.captured(2)).toString().trimmed();
	}

	rx2.setPattern("<div class=\"genre\">\\s*<h4>Genre</h4>\\s*<div>\\s*<a href=.*>(.*)</a>");
	match = rx2.match(allmusic_reply);

	if(match.hasMatch()) {
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

		allmusic_genre = QVariant(match.captured(1)).toString();
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
	} else if(item->text(0) == "Language") {
		_song->setInfo(LANGUAGE_TAG, item->text(1));
	} else if(item->text(0) == "Genre") {
		_song->setInfo(GENRE_TAG, item->text(1));
	} else if(item->text(0) == "Year") {
		_song->setInfo(YEAR_TAG, item->text(1));
	}
	
	_song->save();
}
