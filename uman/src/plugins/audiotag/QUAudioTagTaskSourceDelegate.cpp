#include "QUAudioTagTaskSourceDelegate.h"
#include "QUAudioTagTask.h"

QUAudioTagTaskSourceDelegate::QUAudioTagTaskSourceDelegate(QObject *parent): QUTaskSourceDelegate(parent) {}

void QUAudioTagTaskSourceDelegate::setItems(QComboBox *comboBox) const {
	comboBox->addItems(QUScriptableTask::availableSources() + QUAudioTagTask::availableSpecialSources());
	for(int i = 0; i < comboBox->count(); i++)
		comboBox->setItemData(i, Qt::darkGray, Qt::ForegroundRole);

	foreach(QString commonSource, QUAudioTagTask::availableCommonSources()) {
		comboBox->addItem(QIcon(":/control/id3.png"), commonSource);
	}

	comboBox->addItems(QUAudioTagTask::availableSongSources());
}
