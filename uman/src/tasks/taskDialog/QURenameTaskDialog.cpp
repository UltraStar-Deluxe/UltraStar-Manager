#include "QURenameTaskDialog.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>
#include <QFileDialog>
#include <QTextStream>
#include <QIcon>
#include <QDomDocument>
#include <QDomElement>
#include <QDomAttr>
#include <QDomCDATASection>

QURenameTaskDialog::QURenameTaskDialog(QURenameTask *task, QWidget *parent): QUTaskDialog(task, parent) {
	targetCombo->addItems(QURenameTask::availableTargets());
}

QURenameTaskDialog::QURenameTaskDialog(QWidget *parent): QUTaskDialog(parent) {
	targetCombo->addItems(QURenameTask::availableTargets());

	dataTable->fillData(QList<QUScriptData*>(), QU::renameTask); // for setting up custom delegates
}

/*!
 * Collect the info which is present in this dialog, create a DOM document and save the
 * XML config file to disk.
 */
bool QURenameTaskDialog::saveTask(const QString &filePath) {
	QDomDocument doc;
	QDomElement task = doc.createElement("task");                   doc.appendChild(task);
	QDomElement general = doc.createElement("general");            task.appendChild(general);
	QDomElement rename = doc.createElement("rename");              task.appendChild(rename);
	QDomElement icon = doc.createElement("icon");               general.appendChild(icon);
	QDomElement description = doc.createElement("description"); general.appendChild(description);
	QDomElement tooltip = doc.createElement("tooltip");         general.appendChild(tooltip);

	general.setAttribute("type", "rename");
	if(exclusiveChk->checkState() == Qt::Checked)
		general.setAttribute("group", QVariant(groupSpin->value()).toString());
	icon.setAttribute("resource", iconCombo->itemData(iconCombo->currentIndex()).toString());

	if(!descriptionEdit->text().isEmpty()) {
		QDomCDATASection cdataDescr = doc.createCDATASection(descriptionEdit->text());
		description.appendChild(cdataDescr);
	}
	if(!toolTipEdit->toPlainText().isEmpty()) {
		QDomCDATASection cdataToolt = doc.createCDATASection(toolTipEdit->toPlainText());
		tooltip.appendChild(cdataToolt);
	}

	rename.setAttribute("target", targetCombo->currentText());
	rename.setAttribute("schema", schemaEdit->text());

	for(int row = 0; row < dataTable->rowCount(); row++) {
		QDomElement data = doc.createElement("data");

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

	// save the generated content to disk
	QFile file(filePath);

	if(file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
		QTextStream out(&file);
		out << doc.toString(4);
		file.close();

		emit finished(QString(tr("The task file \"%1\" was saved successfully.")).arg(filePath), QU::saving);
		return true;
	} else {
		emit finished(QString(tr("The task file \"%1\" was NOT saved.")).arg(filePath), QU::warning);
		return false;
	}
}
