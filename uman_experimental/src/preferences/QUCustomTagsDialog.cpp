#include "QUCustomTagsDialog.h"

#include <QSettings>

#include "QUSongSupport.h"

QUCustomTagsDialog::QUCustomTagsDialog(QWidget *parent): QDialog(parent) {
	setupUi(this);

	connect(applyBtn, SIGNAL(clicked()), this, SLOT(save()));
	connect(cancelBtn, SIGNAL(clicked()), this, SLOT(reject()));
	connect(addBtn, SIGNAL(clicked()), this, SLOT(addTag()));
	connect(removeBtn, SIGNAL(clicked()), this, SLOT(removeCurrentTag()));

	tagList->addItems(QUSongSupport::availableCustomTags());

	for(int row = 0; row < tagList->count(); row++) {
		tagList->item(row)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled);
		tagList->item(row)->setIcon(QIcon(":/bullets/bullet_star.png"));
	}

	removeBtn->setEnabled(tagList->count() > 0);
}

void QUCustomTagsDialog::save() {
	QSettings settings;
	QStringList customTags;

	for(int row = 0; row < tagList->count(); row++) {
		QString customTag = tagList->item(row)->text().trimmed();

		// remove whitespaces
		for(int i = 0; i < customTag.length(); i++)
			if(customTag[i].isSpace())
				customTag.remove(i, 1);

		customTags << customTag;
	}

	settings.setValue("customTags", customTags.join(" "));

	this->accept();
}

void QUCustomTagsDialog::addTag() {
	QListWidgetItem *newItem = new QListWidgetItem(QIcon(":/bullets/bullet_star.png"), tr("New Custom Tag"));
	newItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled);

	tagList->addItem(newItem);
	tagList->editItem(newItem);

	removeBtn->setEnabled(true);
}

void QUCustomTagsDialog::removeCurrentTag() {
	qDeleteAll(tagList->selectedItems());
	removeBtn->setEnabled(tagList->count() > 0);
}
