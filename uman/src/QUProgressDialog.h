#ifndef QUPROGRESSDIALOG_H_
#define QUPROGRESSDIALOG_H_

#include <QString>
#include <QDialog>

#include "ui_QUProgressDialog.h"

class QUProgressDialog: public QDialog, private Ui::QUProgressDialog {
	Q_OBJECT
	
public:
	QUProgressDialog(const QString &info, int maximum, QWidget *parent = 0);
	
	virtual void show();
	
public slots:
	void update(const QString &itemText);
	void setInformation(const QString &infoText);
	void setMaximum(int maximum) { progress->setMaximum(maximum); }
	
	void setPixmap(const QString &fileName);
	
};

#endif /*QUPROGRESSDIALOG_H_*/
