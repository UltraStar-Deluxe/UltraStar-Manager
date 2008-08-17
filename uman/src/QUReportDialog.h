#ifndef QUREPORTDIALOG_H_
#define QUREPORTDIALOG_H_

#include "QU.h"
#include "QUSongTree.h"
#include "QUSongTagData.h"
#include "QUBooleanSongData.h"
#include "QUSongFileData.h"
#include "QUReportItem.h"

#include <QDialog>

#include "ui_QUReportDialog.h"

class QUReportDialog: public QDialog, private Ui::QUReportDialog {
	Q_OBJECT

public:
	QUReportDialog(QUSongTree *songTree, QWidget *parent = 0);

private:
	QUSongTree *_songTree;

	void initReportList();
	void fetchDataAndSongs(QList<QUAbstractReportData*> &data, QList<QUSongFile*> &songs);

private slots:
	void createHtmlReport();
	void createPlainTextReport();

signals:
	void finished(const QString &message, QU::EventMessageTypes type);
};

#endif /*QUREPORTDIALOG_H_*/
