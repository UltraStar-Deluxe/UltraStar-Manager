#ifndef QUPLUGINDIALOG_H_
#define QUPLUGINDIALOG_H_

#include <QDialog>
#include <QList>

#include "ui_QUPluginDialog.h"

class QPluginLoader;

class QUPluginDialog: public QDialog, private Ui::QUPluginDialog {
	Q_OBJECT

public:
	QUPluginDialog(const QList<QPluginLoader*> &plugins, QWidget *parent = 0);
};

#endif // QUPLUGINDIALOG_H_
