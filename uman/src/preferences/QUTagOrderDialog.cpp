#include "QUTagOrderDialog.h"
#include "QUSongFile.h"

#include <QSettings>
#include <QMessageBox>

#include "QUSongSupport.h"

QUTagOrderDialog::QUTagOrderDialog(QWidget *parent): QDialog(parent) {
	setupUi(this);

	QSettings settings;
	QStringList items = settings.value("tagOrder", QVariant(QUSongSupport::availableTags())).toStringList();

	QUSongFile::verifyTags(items);

	foreach(QString item, items) {
		tagList->addItem(item.toUpper());
	}

	connect(applyBtn, SIGNAL(clicked()), SLOT(saveOrder()));
	connect(cancelBtn, SIGNAL(clicked()), SLOT(reject()));
	connect(defaultBtn, SIGNAL(clicked()), SLOT(resetOrder()));
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

/*!
 * Reset the order to the default value.
 */
void QUTagOrderDialog::resetOrder() {
	tagList->clear();
	tagList->addItems(QUSongSupport::availableTags());
}
