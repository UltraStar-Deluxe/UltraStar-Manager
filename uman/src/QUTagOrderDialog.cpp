#include "QUTagOrderDialog.h"
#include "QUSongFile.h"

#include <QSettings>
#include <QMessageBox>


QUTagOrderDialog::QUTagOrderDialog(QWidget *parent): QDialog(parent) {
	setupUi(this);
	
	QSettings settings;
	QStringList items = settings.value("tagOrder", QVariant(QUSongFile::tags())).toStringList();
	
	QUSongFile::verifyTags(items);
		
	tagList->addItems(items);
	
	connect(applyBtn, SIGNAL(clicked()), SLOT(saveOrder()));
	connect(cancelBtn, SIGNAL(clicked()), SLOT(reject()));
}

QUTagOrderDialog::~QUTagOrderDialog() {
}

void QUTagOrderDialog::saveOrder() {
	QStringList tags;
	for(int i = 0; i < tagList->count(); i++)
		tags << tagList->item(i)->text();
		
	QSettings settings;
	settings.setValue("tagOrder", QVariant(tags));
	
	accept();
}
