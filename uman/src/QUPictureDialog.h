#ifndef QUPICTUREDIALOG_H_
#define QUPICTUREDIALOG_H_

#include <QDialog>

#include "ui_QUPictureDialog.h"

class QUPictureDialog: public QDialog, private Ui::QUPictureDialog {
	Q_OBJECT
	
public:
	QUPictureDialog(const QString &file, QWidget *parent = 0);
};

#endif /*QUPICTUREDIALOG_H_*/
