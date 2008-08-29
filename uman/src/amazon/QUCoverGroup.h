#ifndef QUCOVERGROUP_H_
#define QUCOVERGROUP_H_

#define COVER_ICON_WIDTH  64
#define COVER_ICON_HEIGHT 64

#include "QU.h"
#include "QUSongItem.h"
#include "QUPictureDialog.h"
#include "QUAmazonResponse.h"

#include <QWidget>
#include <QSize>
#include <QListWidgetItem>
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

private slots:
	void previewActivePicture(QListWidgetItem *item);

	void showStateChange(int state);
	void showDone(bool error);

	void finishRequest(bool error);

signals:
	void finished(const QString &message, QU::EventMessageTypes type);

private:
	QHttp            *_http;
	QUSongItem       *_item;
	QBuffer          *_buffer;
	QUAmazonResponse *_response;
	bool              _waitForResult;
	QList<QFile*>     _downloads;

	QString customDir() const;
};

#endif /* QUCOVERGROUP_H_ */
