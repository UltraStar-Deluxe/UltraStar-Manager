#include "QUCoverGroup.h"
#include "QULogService.h"

#include <QCoreApplication>
#include <QDesktopServices>
#include <QSettings>
#include <QUrl>
#include <QVariant>
#include <QMessageBox>

#include "QUSongSupport.h"
#include "QUStringSupport.h"

QUCoverGroup::QUCoverGroup(QUSongItem *songItem, QWidget *parent):
	QWidget(parent),
	_item(songItem)
{
	setupUi(this);

	list->setIconSize(QSize(COVER_ICON_WIDTH, COVER_ICON_HEIGHT));

	showStatus("");

	connect(buyBtn, SIGNAL(clicked()), this, SLOT(openAmazonSearchUrl()));
	buyBtn->setEnabled(false);
}

void QUCoverGroup::getCovers(const QString &endpoint, const QString &artistProperty, const QString &titleProperty) {
	if(!_item) {
		logSrv->add(tr("Could not get covers. No song was set."), QU::Warning);
		return;
	}

	_endpoint = endpoint; // cache the current endpoint, see showStatus();
	buyBtn->setEnabled(true);
}

QString QUCoverGroup::customDir() const {
	 QString result = QUStringSupport::withoutUnsupportedCharacters(
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

	QFileInfoList picFiList = covers.entryInfoList(QUSongSupport::allowedPictureFiles(), QDir::Files, QDir::Name);

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
 * Deletes the current cover from this song.
 */
void QUCoverGroup::deleteCurrentCover() {
	if(!_item)
		return;

	if(!QFile::remove(_item->song()->coverFileInfo().filePath()))
		logSrv->add(QString(tr("Could not delete current cover: \"%1\"")).arg(_item->song()->coverFileInfo().filePath()), QU::Warning);
	else
		logSrv->add(QString(tr("Current cover was deleted successfully: \"%1\"")).arg(_item->song()->coverFileInfo().filePath()), QU::Information);
}

/*!
 * Uses the selected cover for the song.
 */
void QUCoverGroup::copyCoverToSongPath() {
	if(!_item)
		return;

	if(currentFilePath().isEmpty()) {
		logSrv->add(QString(tr("No cover selected for: \"%1 - %2\"")).arg(_item->song()->artist()).arg(_item->song()->title()), QU::Warning);
		return;
	}

	QFileInfo target(_item->song()->songFileInfo().dir(), "cover_" + QFileInfo(currentFilePath()).fileName());

	if(!QFile::copy(currentFilePath(), target.filePath())) {
		logSrv->add(QString(tr("Could not copy the new cover \"%1\" to \"%2\".")).arg(currentFilePath()).arg(target.filePath()), QU::Warning);
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

	url.setPath(QString("s/url=search-alias=mp3-downloads&field-keywords=%1+%2").arg(_item->song()->artist()).arg(_item->song()->title()));
	QDesktopServices::openUrl(url);

	url.setPath(QString("s/url=search-alias=aps&field-keywords=%1").arg(_item->song()->artist()));
	QDesktopServices::openUrl(url);
}
