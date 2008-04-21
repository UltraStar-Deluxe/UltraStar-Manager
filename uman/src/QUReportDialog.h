#ifndef QUREPORTDIALOG_H_
#define QUREPORTDIALOG_H_

#include "QU.h"
#include "QUSongTree.h"
#include "QUSongTagData.h"
#include "QUBooleanSongData.h"
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
	
private slots:
	void createHtmlReport();
	
signals:
	void finished(const QString &message, QU::EventMessageTypes type);
};

#endif /*QUREPORTDIALOG_H_*/
