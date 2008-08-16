#include "QUCustomTagsDialog.h"

#include <QSettings>

QUCustomTagsDialog::QUCustomTagsDialog(QWidget *parent): QDialog(parent) {
	setupUi(this);

	connect(applyBtn, SIGNAL(clicked()), this, SLOT(save()));
	connect(cancelBtn, SIGNAL(clicked()), this, SLOT(reject()));
	connect(addBtn, SIGNAL(clicked()), this, SLOT(addTag()));
	connect(removeBtn, SIGNAL(clicked()), this, SLOT(removeCurrentTag()));

	tagList->addItems(QUSongFile::customTags());

	for(int row = 0; row < tagList->count(); row++)
		tagList->item(row)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled);

	removeBtn->setEnabled(tagList->count() > 0);
}

void QUCustomTagsDialog::save() {
	QSettings settings;
	QStringList customTags;

	for(int row = 0; row < tagList->count(); row++)
		customTags << tagList->item(row)->text();

	settings.setValue("customTags", customTags);

	this->accept();
}

void QUCustomTagsDialog::addTag() {
	QListWidgetItem *newItem = new QListWidgetItem(tr("New Custom Tag"));
	newItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled);

	tagList->addItem(newItem);
	tagList->editItem(newItem);

	removeBtn->setEnabled(true);
}

void QUCustomTagsDialog::removeCurrentTag() {
	qDeleteAll(tagList->selectedItems());
	removeBtn->setEnabled(tagList->count() > 0);
}
