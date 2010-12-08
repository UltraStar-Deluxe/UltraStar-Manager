#include "QUCoverGroup.h"
#include "QULogService.h"
#include "QUSimpleCommunicator.h"

#include <QCoreApplication>
#include <QDesktopServices>
#include <QSettings>
#include <QUrl>
#include <QVariant>
#include <QMessageBox>

#include "QUSongSupport.h"
#include "QUStringSupport.h"

QUCoverGroup::QUCoverGroup(QUSongItem *item, QURemoteImageCollector *collector, QWidget *parent):
	QWidget(parent),
	_item(item)
{
	setupUi(this);
	list->setIconSize(QSize(COVER_ICON_WIDTH, COVER_ICON_HEIGHT));
	showStatus("");
	setCollector(collector);
}

void QUCoverGroup::setCollector(QURemoteImageCollector *collector) {
	_collector = collector;

	if(!collector->communicator()) {
		QUSimpleCommunicator *c = new QUSimpleCommunicator(this);
		connect(c, SIGNAL(messageSent(QString)), this, SLOT(showStatus(QString)));
		connect(c, SIGNAL(done()), this, SLOT(showCovers()));
		connect(c, SIGNAL(failed()), this, SLOT(showFailure()));
		this->collector()->setCommunicator(c);
	}
}

void QUCoverGroup::getCovers() {
	collector()->collect();
}

void QUCoverGroup::showCovers() {
	list->model()->clear();
	list->hide();

	foreach(QFileInfo fi, collector()->results())
		list->model()->addCover(fi.filePath());

	list->selectionModel()->setCurrentIndex(list->model()->index(0), QItemSelectionModel::Select);

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

void QUCoverGroup::showFailure() {
	logSrv->add(tr("Cover download failed for \"%1 - %2\"").arg(song()->artist()).arg(song()->title()), QU::Error);
}

/*!
 * \returns file path of the current/selected image
 */
QString QUCoverGroup::currentFilePath() const {
	return list->currentFilePath();
}

/*!
 * Uses the selected cover for the song.
 */
void QUCoverGroup::copyCoverToSongPath(bool deleteCurrentCover) {
	if(currentFilePath().isEmpty()) {
		logSrv->add(QString(tr("Could not copy cover to song path. No cover selected for: \"%1 - %2\"")).arg(song()->artist()).arg(song()->title()), QU::Warning);
		return;
	}

	QFileInfo target(song()->songFileInfo().dir(), "cover_" + QFileInfo(currentFilePath()).fileName());

	if(!QFile::copy(currentFilePath(), target.filePath())) {
		logSrv->add(QString(tr("Could not copy the new cover \"%1\" to \"%2\".")).arg(currentFilePath()).arg(target.filePath()), QU::Warning);
		return;
	}

	QString oldName = song()->coverFileInfo().filePath();
	if(deleteCurrentCover) {
		if(!QFile::remove(oldName)) {
			logSrv->add(QString(tr("Could not delete current cover: \"%1\"")).arg(song()->coverFileInfo().filePath()), QU::Warning);
			song()->autoSetFile(target, true);
		} else {
			logSrv->add(QString(tr("Current cover was replaced successfully for: \"%1 - %2\"")).arg(song()->artist()).arg(song()->title()), QU::Information);
			QFile::rename(target.filePath(), oldName);
		}
	} else {
		// copy operation well done - now set the new cover
		song()->autoSetFile(target, true);
	}

	song()->save();
	_item->update();
}

void QUCoverGroup::showStatus(const QString &status) {
	if(!status.isEmpty())
		group->setTitle(QString("%1 - %2 (%3)").arg(song()->artist()).arg(song()->title()).arg(status));
	else
		group->setTitle(QString("%1 - %2").arg(song()->artist()).arg(song()->title()));
}

void QUCoverGroup::openAmazonSearchUrl() {
//	QString tmp = _endpoint;
//	QUrl url(tmp.replace("ecs", "www").remove("aws"));
//
//	url.setPath(QString("s/url=search-alias=mp3-downloads&field-keywords=%1+%2").arg(_item->song()->artist()).arg(_item->song()->title()));
//	QDesktopServices::openUrl(url);
//
//	url.setPath(QString("s/url=search-alias=aps&field-keywords=%1").arg(_item->song()->artist()));
//	QDesktopServices::openUrl(url);
}
