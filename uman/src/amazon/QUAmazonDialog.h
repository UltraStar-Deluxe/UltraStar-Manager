#ifndef QUAMAZONDIALOG_H_
#define QUAMAZONDIALOG_H_

#include "QU.h"
#include "QUSongItem.h"
#include "QUCoverGroup.h"

#include <QDialog>
#include <QList>
#include <QString>
#include <QVBoxLayout>

#include "ui_QUAmazonArea.h"

class QUAmazonDialog: public QDialog, private Ui::QUAmazonArea {
	Q_OBJECT

public:
	QUAmazonDialog(const QList<QUSongItem*> &items, QWidget *parent = 0);

private slots:
	void getCovers();
	void accept();

	void checkAllGroups();
	void uncheckAllGroups();

	void setLimit(const QString &limit);
	void setKeepDownloads(int state);

	void setKey();
	void setSecret();

	void showKeyHelp();
	void showSecretHelp();

private:
	QVBoxLayout         *_contentLayout;
	QList<QUCoverGroup*> _groups;

	void createGroups(const QList<QUSongItem*> &items);
};

#endif /* QUAMAZONDIALOG_H_ */
