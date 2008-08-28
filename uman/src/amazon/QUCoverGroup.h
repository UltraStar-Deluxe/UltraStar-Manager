#ifndef QUCOVERGROUP_H_
#define QUCOVERGROUP_H_

#define COVER_ICON_WIDTH  64
#define COVER_ICON_HEIGHT 64

#include "QU.h"
#include "QUSongFile.h"
#include "QUPictureDialog.h"

#include <QWidget>
#include <QSize>
#include <QListWidgetItem>

#include "ui_QUCoverGroup.h"

class QUCoverGroup: public QWidget, public Ui::QUCoverGroup {
	Q_OBJECT

public:
	QUCoverGroup(const QString &name, QWidget *parent = 0);

private slots:
	void previewActivePicture(QListWidgetItem *item);
};

#endif /* QUCOVERGROUP_H_ */
