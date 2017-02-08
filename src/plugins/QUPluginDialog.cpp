#include "QUPluginDialog.h"

#include "QUPluginManager.h"
#include "QUPluginModel.h"
#include "QUPluginDelegate.h"

QUPluginDialog::QUPluginDialog(QWidget *parent): QDialog(parent) {
	setupUi(this);

	connect(okBtn, SIGNAL(clicked()), this, SLOT(accept()));
	connect(reloadBtn, SIGNAL(clicked()), pluginMGR, SLOT(reload()));

//	apiLbl->setText(QString(tr("<b>API:</b> Song v%1, Plugin v%2")).arg(SONG_API_VERSION).arg(PLUGIN_API_VERSION));
	apiLbl->setText("");

	pluginList->setModel(new QUPluginModel(this));
	pluginList->setItemDelegate(new QUPluginDelegate(this));
}
