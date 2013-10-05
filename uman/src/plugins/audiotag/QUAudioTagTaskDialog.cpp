#include "QUAudioTagTaskDialog.h"
#include "QUTaskModifierDelegate.h"
#include "QUTaskConditionDelegate.h"
#include "QUAudioTagTaskSourceDelegate.h"
#include "QUDefaultDelegate.h"

#include <QCoreApplication>

QUAudioTagTaskDialog::QUAudioTagTaskDialog(QUAudioTagTask *task, QWidget *parent): QUTaskDialog(task, parent) {
	dataTable->setDelegates(
			new QUTaskModifierDelegate(dataTable),
			new QUTaskConditionDelegate(dataTable),
			new QUAudioTagTaskSourceDelegate(dataTable),
			new QUDefaultDelegate(dataTable));

	targetCombo->addItems(QUAudioTagTask::availableInfoTargets());

	int i = targetCombo->count();
	targetCombo->addItems(QUAudioTagTask::availableFileTargets());
	for(; i < targetCombo->count(); i++)
		targetCombo->setItemData(i, QColor(Qt::red), Qt::ForegroundRole);

	i = targetCombo->count();
	targetCombo->addItems(QUScriptableTask::availableCustomSources());
	for(; i < targetCombo->count(); i++)
		targetCombo->setItemData(i, QColor(Qt::blue), Qt::ForegroundRole);

	infoLbl->setText(tr("Select a proper <b>target tag</b> and create a custom <b>schema</b> for the operation. You can use <b>custom tags</b> as targets."));

	if(task) {
		targetCombo->setCurrentIndex(targetCombo->findText(task->target(), Qt::MatchContains));
	} else {
		this->setWindowTitle(tr("Add Song/ID3 Tag Task"));
	}
}

QDir QUAudioTagTaskDialog::configurationDirectory() const {
	QDir dir = QCoreApplication::applicationDirPath();
	dir.cd("plugins");
	dir.cd("config");
	dir.cd("audiotag");
	return dir;
}

/*!
 * Collect the info which is present in this dialog, create a DOM document and save the
 * XML config file to disk.
 */
bool QUAudioTagTaskDialog::saveTask(const QString &filePath) {
	_doc.clear();
	QDomElement task = _doc.createElement("task");
	_doc.appendChild(task);

	this->appendGeneral(task, QUScriptableTask::AudioTagTask);

	QDomElement id3 = _doc.createElement("id3");
	task.appendChild(id3);

	id3.setAttribute("target", targetCombo->currentText());
	id3.setAttribute("schema", schemaEdit->text());

	for(int row = 0; row < dataTable->rowCount(); row++) {
		QDomElement data = _doc.createElement("data");

		if(dataTable->item(row, MODIFIER_COL)->text() == NEGATE_CONDITION)
			data.setAttribute("modifier", dataTable->item(row, MODIFIER_COL)->text());

		if(dataTable->item(row, CONDITION_COL)->text() != "true")
			data.setAttribute("if", dataTable->item(row, CONDITION_COL)->text());

		if(dataTable->item(row, SOURCE_COL)->text() == TEXT_SOURCE)
			data.setAttribute("text", dataTable->item(row, DEFAULT_COL)->text());
		else {
			data.setAttribute("source", dataTable->item(row, SOURCE_COL)->text());

			if(QUAudioTagTask::availableSpecialSources().contains(dataTable->item(row, DEFAULT_COL)->text(), Qt::CaseInsensitive))
				data.setAttribute("ignoreEmpty", dataTable->item(row, DEFAULT_COL)->text());
			else if(dataTable->item(row, DEFAULT_COL)->text() != N_A)
				data.setAttribute("default", dataTable->item(row, DEFAULT_COL)->text());
		}

		id3.appendChild(data);
	}

	return this->saveDocument(filePath);
}
