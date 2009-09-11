#ifndef QUCOVERGROUP_H_
#define QUCOVERGROUP_H_

#include "QU.h"
#include "QUSongItem.h"

#include <QWidget>
#include <QSize>

#include "ui_QUCoverGroup.h"

class QUCoverGroup: public QWidget, public Ui::QUCoverGroup {
	Q_OBJECT

public:
	QUCoverGroup(QUSongItem *songItem = 0, QWidget *parent = 0);

	void getCovers(const QString &endpoint, const QString &artistProperty, const QString &titleProperty);
	void showCovers();

	void deleteCurrentCover();
	void copyCoverToSongPath();

private slots:
	void openAmazonSearchUrl();

private:
	QUSongItem       *_item;
	QString           _endpoint;

	QString customDir() const;
	QString currentFilePath() const;

	void showStatus(const QString &status);
};

#endif /* QUCOVERGROUP_H_ */
