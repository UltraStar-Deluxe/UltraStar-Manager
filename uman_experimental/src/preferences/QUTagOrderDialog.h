#ifndef QUTAGORDERDIALOG_H_
#define QUTAGORDERDIALOG_H_

#include "ui_QUTagOrderDialog.h"
#include <QDialog>

class QUTagOrderDialog: public QDialog, private Ui::QUTagOrderDialog
{
	Q_OBJECT
	
public:
	QUTagOrderDialog(QWidget *parent = 0);
	~QUTagOrderDialog();

private slots:
	void saveOrder();
	void resetOrder();
};

#endif /*QUTAGORDERDIALOG_H_*/
