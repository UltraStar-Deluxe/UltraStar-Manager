#ifndef QUENCODINGSDIALOG_H_
#define QUENCODINGSDIALOG_H_

#include "ui_QUEncodingsDialog.h"
#include <QDialog>

class QUEncodingsDialog: public QDialog, private Ui::QUEncodingsDialog
{
	Q_OBJECT
	
public:
	QUEncodingsDialog(QWidget *parent = 0);
	~QUEncodingsDialog();

private slots:
	void saveEncodings();
	void resetEncodings();
};

#endif /*QUENCODINGSDIALOG_H_*/
