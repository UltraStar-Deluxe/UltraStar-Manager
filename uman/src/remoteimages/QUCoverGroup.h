#ifndef QUCOVERGROUP_H_
#define QUCOVERGROUP_H_

#include "QU.h"
#include "QUSongItem.h"
#include "QURemoteImageSourcePlugin.h"

#include <QWidget>
#include <QSize>

#include "ui_QUCoverGroup.h"

class QUCoverGroup: public QWidget, public Ui::QUCoverGroup {
	Q_OBJECT

public:
	QUCoverGroup(QUSongItem *item, QURemoteImageCollector *collector, QWidget *parent = 0);

	void getCovers();
	void copyCoverToSongPath(bool deleteCurrentCover = false);

	void setCollector(QURemoteImageCollector *collector);

	QUSongItem* songItem() const { return _item; }

public slots:
	void openAmazonSearchUrl();
	void showStatus(const QString &status);
	void showCovers();
	void showFailure();

protected:
	QUSongFile* song() const { return _item->song(); }
	QURemoteImageCollector* collector() const { return _collector; }

private:
	QUSongItem             *_item;
	QURemoteImageCollector *_collector;

	QString currentFilePath() const;
};

#endif /* QUCOVERGROUP_H_ */
