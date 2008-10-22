#ifndef QUCOVERGROUP_H_
#define QUCOVERGROUP_H_

#include "QU.h"
#include "QUSongItem.h"
#include "QUPictureDialog.h"
#include "QUAmazonResponse.h"

#include <QWidget>
#include <QSize>
#include <QHttp>
#include <QBuffer>
#include <QFile>

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
	void previewActivePicture(const QString &filePath);

	void showStateChange(int state);
	void finishRequest(bool error);

	void openAmazonSearchUrl();

private:
	QHttp            *_http;
	QUSongItem       *_item;
	QBuffer          *_buffer;
	QUAmazonResponse *_response;
	bool              _waitForResult;
	QList<QFile*>     _downloads;
	QString           _endpoint;

	QString customDir() const;
	QString currentFilePath() const;

	void showStatus(const QString &status);
};

#endif /* QUCOVERGROUP_H_ */
