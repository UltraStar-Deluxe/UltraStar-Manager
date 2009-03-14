#include "QURenameTaskSourceDelegate.h"
#include "QURenameTask.h"

QURenameTaskSourceDelegate::QURenameTaskSourceDelegate(QObject *parent): QUTaskSourceDelegate(parent) {}

void QURenameTaskSourceDelegate::setItems(QComboBox *comboBox) const {
	comboBox->addItems(QURenameTask::availableSpecialSources());
	for(int i = 0; i < comboBox->count(); i++)
		comboBox->setItemData(i, Qt::darkGray, Qt::ForegroundRole);

	comboBox->addItems(QURenameTask::availableCommonSources());
	int i = comboBox->count();
	comboBox->addItems(QUScriptableTask::availableCustomSources());
	for(; i < comboBox->count(); i++)
		comboBox->setItemData(i, Qt::blue, Qt::ForegroundRole);
}
