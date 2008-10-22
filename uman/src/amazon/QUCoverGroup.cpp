#include "QUCoverGroup.h"
#include "QUAmazonRequestUrl.h"
#include "QULogService.h"

#include <QCoreApplication>
#include <QDesktopServices>
#include <QSettings>

#include <QVariant>
#include <QHttpRequestHeader>

#include <QMessageBox>
#include <QDomDocument>

QUCoverGroup::QUCoverGroup(QUSongItem *songItem, QWidget *parent):
	QWidget(parent),
	_item(songItem),
	_response(0),
	_waitForResult(false)
{
	setupUi(this);

	list->setIconSize(QSize(COVER_ICON_WIDTH, COVER_ICON_HEIGHT));

	showStatus("");

	_http = new QHttp(this);
	connect(_http, SIGNAL(stateChanged(int)), this, SLOT(showStateChange(int)));
	connect(_http, SIGNAL(done(bool)), this, SLOT(finishRequest(bool)));

	_buffer = new QBuffer(this);

//	list->setMinimumHeight(1);

	connect(list, SIGNAL(coverActivated (const QString&)), this, SLOT(previewActivePicture(const QString&)));

	connect(buyBtn, SIGNAL(clicked()), this, SLOT(openAmazonSearchUrl()));
	buyBtn->setEnabled(false);
}

void QUCoverGroup::getCovers(const QString &endpoint, const QString &artistProperty, const QString &titleProperty) {
	if(!_item) {
		logSrv->add(tr("Could not get covers. No song was set."), QU::warning);
		return;
	}

	if(_http->hasPendingRequests() or _waitForResult) {
		logSrv->add(QString(tr("Could not get covers for \"%1 - %2\". Http connection is busy.")).arg(_item->song()->artist()).arg(_item->song()->title()), QU::warning);
		return;
	}

	_endpoint = endpoint; // cache the current endpoint, see showStatus();
	buyBtn->setEnabled(true);

	QUAmazonRequestUrl url(endpoint, artistProperty, titleProperty, _item->song());

	_waitForResult = true;
	_http->setHost(url.host());
	_http->get(url.request(), _buffer);
}

void QUCoverGroup::previewActivePicture(const QString &filePath) {
	QUPictureDialog *dlg = new QUPictureDialog(filePath, this);
	dlg->exec();
	delete dlg;
}

void QUCoverGroup::showStateChange(int state) {
//	     if(state == QHttp::Unconnected) showStatus(tr(" (Not connected.)"));
//	else if(state == QHttp::HostLookup)  showStatus(tr(" (Lookup host...)"));
//	else if(state == QHttp::Connecting)  showStatus(tr(" (Connecting...)"));
	     if(state == QHttp::Sending)     showStatus(tr(" (Sending...)"));
	else if(state == QHttp::Reading)     showStatus(tr(" (Reading...)"));
//	else if(state == QHttp::Connected)   showStatus(tr(" (Connected.)"));
//	else if(state == QHttp::Closing)     showStatus(tr(" (Closing...)"));
}

void QUCoverGroup::finishRequest(bool error) {
	// close all open files
	foreach(QFile* download, _downloads) { download->close(); }
	qDeleteAll(_downloads);
	_downloads.clear();

	if(_waitForResult) {
		_waitForResult = false;
		QDomDocument doc; doc.setContent(_buffer->data());

		if(_response)
			delete _response;

		_response = new QUAmazonResponse(doc);
		delete _buffer;	_buffer = new QBuffer(this);

		if(!_response->isValid()) {
			showStatus(tr(" (Invalid request.)"));
			return;
		}

		// TODO: Exception handling - disk full and so on..
		QDir outDir(QCoreApplication::applicationDirPath());
		outDir.mkdir("covers"); outDir.cd("covers");

		outDir.mkdir(customDir()); outDir.cd(customDir());

		// remove old downloads, if set
		QSettings settings;
		if(!settings.value("keepDownloads", false).toBool()) {
			QFileInfoList picFiList = outDir.entryInfoList(QU::allowedPictureFiles(), QDir::Files, QDir::Name);
			foreach(QFileInfo fi, picFiList) {
				QFile::remove(fi.filePath());
			}
		}

		if(_response->count() == 0) {
			showStatus(tr(" (No results.)"));
			showCovers();
		}

		// start new downloads
		int amazonLimit = settings.value("amazonLimit", "5").toInt();
		for(int i = 0; i < _response->count() and i < amazonLimit; i++) {
			QFile *file;
			// medium size
//			file = new QFile(QFileInfo(outDir, QFileInfo(_response->url(i, QU::mediumImage).path()).fileName()).filePath(), this);
//			if(file->open(QIODevice::WriteOnly | QIODevice::Truncate)) {
//				_http->setHost(_response->url(i, QU::mediumImage).host());
//				_http->get(_response->url(i, QU::mediumImage).path(), file);
//
//				_downloads.append(file);
//			} else
//				delete file;

			// large size
			file = new QFile(QFileInfo(outDir, QFileInfo(_response->url(i, QU::largeImage).path()).fileName()).filePath(), this);
			if(file->open(QIODevice::WriteOnly | QIODevice::Truncate)) {
				_http->setHost(_response->url(i, QU::largeImage).host());
				_http->get(_response->url(i, QU::largeImage).path(), file);

				_downloads.append(file);
			} else
				delete file;
		}

	} else { // cover download finished
		showStatus(QString(tr(" (%1 results)")).arg(_response->count()));
		showCovers();
	}
}

QString QUCoverGroup::customDir() const {
	 QString result = QU::withoutUnsupportedCharacters(
			 QString("%1 - %2")
				 .arg(_item->song()->artist())
				 .arg(_item->song()->title())).trimmed();

	 if(result.isEmpty())
		 result = "_unknown";

	 return result;
}

void QUCoverGroup::showCovers() {
	list->model()->clear();
	list->hide();

	QDir covers(QCoreApplication::applicationDirPath());

	if(!covers.cd("covers"))
		return;

	if(!covers.cd(customDir()))
		return;

	QFileInfoList picFiList = covers.entryInfoList(QU::allowedPictureFiles(), QDir::Files, QDir::Name);

	foreach(QFileInfo pic, picFiList)
		list->model()->addCover(pic.filePath());

	if(list->model()->rowCount() > 0) {
		// TODO: fix this mess! - I just want to see all content all the time ... T_T
		list->show();

		int itemCount = list->model()->rowCount();

		int maxItemHeight = 1;
		for(int i = 0; i < itemCount; i++)
			maxItemHeight = qMax(maxItemHeight, list->sizeHintForRow(i));

		int rowCount = qRound((double)itemCount / 5.0);

		if(rowCount * 5 < itemCount)
			rowCount++;

		list->setMinimumHeight(maxItemHeight * qMax(1, rowCount));
	}
}

/*!
 * \returns file path of the current/selected image
 */
QString QUCoverGroup::currentFilePath() const {
	return list->currentFilePath();
}

/*!
 * Deletes the current cover.
 */
void QUCoverGroup::deleteCurrentCover() {
	if(!_item)
		return;

	if(!QFile::remove(_item->song()->coverFileInfo().filePath()))
		logSrv->add(QString(tr("Could not delete current cover: \"%1\"")).arg(_item->song()->coverFileInfo().filePath()), QU::warning);
	else
		logSrv->add(QString(tr("Current cover was deleted successfully: \"%1\"")).arg(_item->song()->coverFileInfo().filePath()), QU::information);
}

/*!
 * Uses the selected cover for the song.
 */
void QUCoverGroup::copyCoverToSongPath() {
	if(!_item)
		return;

	if(currentFilePath().isEmpty()) {
		logSrv->add(QString(tr("No cover selected for: \"%1 - %2\"")).arg(_item->song()->artist()).arg(_item->song()->title()), QU::warning);
		return;
	}

	QFileInfo target(_item->song()->songFileInfo().dir(), "cover_" + QFileInfo(currentFilePath()).fileName());

	if(!QFile::copy(currentFilePath(), target.filePath())) {
		logSrv->add(QString(tr("Could not copy the new cover \"%1\" to \"%2\".")).arg(currentFilePath()).arg(target.filePath()), QU::warning);
		return;
	}

	// copy operation well done - now set the new cover
	_item->song()->autoSetFile(target, true);

	_item->song()->save();
	_item->update();
}

void QUCoverGroup::showStatus(const QString &status) {
	if(!_item)
		return;

	group->setTitle(QString("%1 - %2%3").arg(_item->song()->artist()).arg(_item->song()->title()).arg(status));
}

void QUCoverGroup::openAmazonSearchUrl() {
	QString tmp = _endpoint;
	QUrl url(tmp.replace("ecs", "www").remove("aws"));

	url.setPath(QString("s/url=search-alias%3Daps&field-keywords=%1 %2").arg(_item->song()->artist()).arg(_item->song()->title()));
	QDesktopServices::openUrl(url);

	url.setPath(QString("s/url=search-alias%3Daps&field-keywords=%1").arg(_item->song()->artist()));
	QDesktopServices::openUrl(url);
}
