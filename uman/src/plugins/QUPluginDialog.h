#ifndef QUPLUGINDIALOG_H_
#define QUPLUGINDIALOG_H_

#include <QDialog>
#include <QList>

#include "ui_QUPluginDialog.h"

#define PLUGIN_FACTORY_COL    0
#define PLUGIN_PRODUCT_COL    1
#define PLUGIN_OPERATIONS_COL 2
#define PLUGIN_QUANTITY_COL   3
#define PLUGIN_LANG_COL       4
#define PLUGIN_VERSION_COL    5
#define PLUGIN_API_COL        6
#define PLUGIN_PATH_COL       7

class QPluginLoader;

class QUPluginDialog: public QDialog, private Ui::QUPluginDialog {
	Q_OBJECT

public:
	QUPluginDialog(QWidget *parent = 0);

public slots:
	void updatePluginTable();

private:
	QTableWidgetItem* createItem(const QString &text);
};

#endif // QUPLUGINDIALOG_H_
