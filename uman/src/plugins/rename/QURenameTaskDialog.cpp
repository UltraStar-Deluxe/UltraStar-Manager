#include "QURenameTaskDialog.h"
#include "QUTaskConditionDelegate.h"
#include "QURenameTaskSourceDelegate.h"
#include "QUTaskTextDelegate.h"

#include <QIcon>
#include <QDomDocument>
#include <QDomElement>
#include <QDomAttr>
#include <QDomCDATASection>
#include <QCoreApplication>

QURenameTaskDialog::QURenameTaskDialog(QURenameTask *task, QWidget *parent): QUTaskDialog(task, parent) {
	dataTable->setDelegates(
			new QUTaskConditionDelegate(dataTable),
			new QURenameTaskSourceDelegate(dataTable),
			new QUTaskTextDelegate(dataTable));

	targetCombo->addItems(QURenameTask::availableTargets());
	infoLbl->setText(tr("Select a proper <b>target</b> and create a custom <b>schema</b> for the renaming operation. You can use <b>custom tags</b> as sources."));

	if(task) {
		targetCombo->setCurrentIndex(targetCombo->findText(task->target(), Qt::MatchContains));
	} else {
		this->setWindowTitle(tr("Add Rename Task"));
	}
}

QDir QURenameTaskDialog::configurationDirectory() const {
	QDir dir = QCoreApplication::applicationDirPath();
	dir.cd("plugins");
	dir.cd("config");
	dir.cd("rename");
	return dir;
}

/*!
 * Collect the info which is present in this dialog, create a DOM document and save the
 * XML config file to disk.
 */
bool QURenameTaskDialog::saveTask(const QString &filePath) {
	_doc.clear();
	QDomElement task = _doc.createElement("task");
	_doc.appendChild(task);

	this->appendGeneral(task, QUScriptableTask::RenameTask);

	QDomElement rename = _doc.createElement("rename");
	task.appendChild(rename);

	rename.setAttribute("target", targetCombo->currentText());
	rename.setAttribute("schema", schemaEdit->text());

	for(int row = 0; row < dataTable->rowCount(); row++) {
		QDomElement data = _doc.createElement("data");

		if(dataTable->item(row, 0)->text() != "true")
			data.setAttribute("if", dataTable->item(row, 0)->text());

		if(dataTable->item(row, 1)->text() == KEEP_SUFFIX_SOURCE)
			data.setAttribute("keepSuffix", "true");
		else if(dataTable->item(row, 1)->text() == TEXT_SOURCE)
			data.setAttribute("text", dataTable->item(row, 2)->text());
		else if(dataTable->item(row, 1)->text() == UNKNOWN_TAGS_SOURCE)
			data.setAttribute("keepUnknownTags", "true");
		else if(!dataTable->item(row, 1)->text().isEmpty()) {
			data.setAttribute("source", dataTable->item(row, 1)->text());
			if(dataTable->item(row, 2)->text() != N_A)
				data.setAttribute("default", dataTable->item(row, 2)->text());
		}

		rename.appendChild(data);
	}

	return this->saveDocument(filePath);
}
