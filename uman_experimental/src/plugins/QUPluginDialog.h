#ifndef QUPLUGINDIALOG_H_
#define QUPLUGINDIALOG_H_

#include <QDialog>

#include "ui_QUPluginDialog.h"

class QUPluginDialog: public QDialog, private Ui::QUPluginDialog {
	Q_OBJECT

public:
	QUPluginDialog(QWidget *parent = 0);
};

#endif // QUPLUGINDIALOG_H_
