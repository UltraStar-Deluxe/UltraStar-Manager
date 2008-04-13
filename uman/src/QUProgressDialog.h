#ifndef QUPROGRESSDIALOG_H_
#define QUPROGRESSDIALOG_H_

#include <QString>
#include <QDialog>

#include "QUAbstractThread.h"
#include "ui_QUProgressDialog.h"

class QUProgressDialog: public QDialog, private Ui::QUProgressDialog {
	Q_OBJECT
	
public:
	QUProgressDialog(QUAbstractThread *thread, QWidget *parent = 0);
	
	void show();
	
private slots:
	void update(const QString &itemText);
	
private:
	QUAbstractThread *t;
};

#endif /*QUPROGRESSDIALOG_H_*/
