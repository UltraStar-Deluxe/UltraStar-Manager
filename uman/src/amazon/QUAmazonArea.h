#ifndef QUAMAZONAREA_H_
#define QUAMAZONAREA_H_

#include "QU.h"
#include "QUSongFile.h"
#include "QUCoverGroup.h"

#include <QWidget>
#include <QList>
#include <QString>

#include "ui_QUAmazonArea.h"

class QUAmazonArea: public QWidget, private Ui::QUAmazonArea {
	Q_OBJECT

public:
	QUAmazonArea(QWidget *parent = 0);

public slots:
	void blubb();
	void foobar();

signals:
	void finished(const QString &message, QU::EventMessageTypes type);

private:
	QUCoverGroup *group1;
};

#endif /* QUAMAZONAREA_H_ */
