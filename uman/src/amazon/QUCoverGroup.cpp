#include "QUCoverGroup.h"
#include "QUAmazonRequestUrl.h"

#include <QCoreApplication>

#include <QVariant>
#include <QHttpRequestHeader>

#include <QMessageBox>
#include <QDomDocument>

QUCoverGroup::QUCoverGroup(QUSongItem *songItem, QWidget *parent):
	QWidget(parent),
	_response(0),
	_waitForResult(false)
{
	setupUi(this);

	list->setIconSize(QSize(COVER_ICON_WIDTH, COVER_ICON_HEIGHT));

	if(songItem)
		group->setTitle(QString("%1 - %2").arg(songItem->song()->artist()).arg(songItem->song()->title()));

	_item = songItem;

	_http = new QHttp(this);
	connect(_http, SIGNAL(stateChanged(int)), this, SLOT(showStateChange(int)));
	connect(_http, SIGNAL(done(bool)), this, SLOT(showDone(bool)));
	connect(_http, SIGNAL(done(bool)), this, SLOT(finishRequest(bool)));

	_buffer = new QBuffer(this);

//	list->setMinimumHeight(1);

	connect(list, SIGNAL(itemActivated (QListWidgetItem*)), this, SLOT(previewActivePicture(QListWidgetItem*)));
}

void QUCoverGroup::getCovers(const QString &endpoint, const QString &artistProperty, const QString &titleProperty) {
	if(!_item) {
		emit finished(tr("Could not get covers. No song was set."), QU::warning);
		return;
	}

	if(_http->hasPendingRequests() or _waitForResult) {
		emit finished(QString(tr("Could not get covers for \"%1 - %2\". Http connection is busy.")).arg(_item->song()->artist()).arg(_item->song()->title()), QU::warning);
		return;
	}

	QUAmazonRequestUrl url(endpoint, artistProperty, titleProperty, _item->song());

	_waitForResult = true;
	_http->setHost(url.host());          // id == 1
	_http->get(url.request(), _buffer);  // id == 2

//	QHttpRequestHeader header("GET", url.request());
//	header.setValue("Host", url.host());
//	header.setValue("accept-enconding", "utf-8");
//	_http->setHost(url.host());
//	_http->request(header, 0, _buffer);
//	emit finished(QString(tr("Request: %1")).arg(url.request()), QU::information);
//	emit finished(QString(tr("Artist: %1 Title: %2")).arg(artistProperty).arg(titleProperty), QU::information);
}

void QUCoverGroup::previewActivePicture(QListWidgetItem *item) {
	QUPictureDialog *dlg = new QUPictureDialog(item->data(Qt::UserRole).toString(), this);
	dlg->exec();
	delete dlg;
}

void QUCoverGroup::showStateChange(int state) {
	if(!_item)
		return;

	QString groupTitle = QString("%1 - %2 (%3)").arg(_item->song()->artist()).arg(_item->song()->title());

	     if(state == QHttp::Unconnected) groupTitle = groupTitle.arg(tr("Not connected."));
	else if(state == QHttp::HostLookup)  groupTitle = groupTitle.arg(tr("Lookup host..."));
	else if(state == QHttp::Connecting)  groupTitle = groupTitle.arg(tr("Connecting..."));
	else if(state == QHttp::Sending)     groupTitle = groupTitle.arg(tr("Sending..."));
	else if(state == QHttp::Reading)     groupTitle = groupTitle.arg(tr("Reading..."));
	else if(state == QHttp::Connected)   groupTitle = groupTitle.arg(tr("Connected."));
	else if(state == QHttp::Closing)     groupTitle = groupTitle.arg(tr("Closing..."));
	else                                 groupTitle = groupTitle.arg(tr("???"));

	group->setTitle(groupTitle);
}

void QUCoverGroup::showDone(bool error) {
	if(!_item)
		return;

	QString groupTitle = QString("%1 - %2 %3").arg(_item->song()->artist()).arg(_item->song()->title());

	groupTitle = groupTitle.arg(error ? "*ERROR*" : "");

	group->setTitle(groupTitle);
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

		if(!_response->isValid())
			return;

		// TODO: Exception handling - disk full and so on..
		QDir outDir(QCoreApplication::applicationDirPath());
		outDir.mkdir("covers"); outDir.cd("covers");

		outDir.mkdir(customDir()); outDir.cd(customDir());

		// remove old downloads
		QFileInfoList picFiList = outDir.entryInfoList(QU::allowedPictureFiles(), QDir::Files, QDir::Name);
		foreach(QFileInfo fi, picFiList) {
			QFile::remove(fi.filePath());
		}

		// start new downloads
		for(int i = 0; i < _response->count() and i < 5; i++) {
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
		this->showCovers();
	}
}

QString QUCoverGroup::customDir() const {
	 return QString("%1 - %2")
		.arg(QU::withoutUnsupportedCharacters(_item->song()->artist()))
		.arg(QU::withoutUnsupportedCharacters(_item->song()->title()));
}

void QUCoverGroup::showCovers() {
	list->clear();

	QDir covers(QCoreApplication::applicationDirPath());

	if(!covers.cd("covers"))
		return;

	if(!covers.cd(customDir()))
		return;

	QFileInfoList picFiList = covers.entryInfoList(QU::allowedPictureFiles(), QDir::Files, QDir::Name);

	foreach(QFileInfo pic, picFiList) {
		QPixmap pixmap(pic.filePath());
		QListWidgetItem *newItem = new QListWidgetItem(QIcon(pixmap.scaledToWidth(COVER_ICON_WIDTH, Qt::SmoothTransformation)), QString("%1 x %2").arg(pixmap.width()).arg(pixmap.height()));

		newItem->setData(Qt::UserRole, pic.filePath());

		list->addItem(newItem);
	}
}

/*!
 * \returns file path of the current/selected image
 */
QString QUCoverGroup::currentFilePath() const {
	QListWidgetItem *currentItem = list->currentItem();

	if(!currentItem)
		return QString();

	return currentItem->data(Qt::UserRole).toString();
}

/*!
 * Uses the selected cover for the song.
 */
void QUCoverGroup::copyCoverToSongPath() {
	if(!_item)
		return;

	if(currentFilePath().isEmpty()) {
		emit finished(QString(tr("No cover selected for: \"%1 - %2\"")).arg(_item->song()->artist()).arg(_item->song()->title()), QU::warning);
		return;
	}

	QFileInfo target(_item->song()->songFileInfo().dir(), "cover_" + QFileInfo(currentFilePath()).fileName());

	if(!QFile::copy(currentFilePath(), target.filePath())) {
		emit finished(QString(tr("Could not copy the new cover \"%1\" to \"%2\".")).arg(currentFilePath()).arg(target.filePath()), QU::warning);
		return;
	}

	// copy operation well done - now set the new cover
	_item->song()->autoSetFile(target, true);

	_item->song()->save();
	_item->update();
}
