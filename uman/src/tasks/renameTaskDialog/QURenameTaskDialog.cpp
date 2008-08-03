#include "QURenameTaskDialog.h"
#include "QUSongFile.h"

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

QURenameTaskDialog::QURenameTaskDialog(QURenameTask *task, QWidget *parent): QDialog(parent) {
	setupUi(this);

	targetCombo->clear();
	targetCombo->addItems(QUSongFile::availableTargets());
	targetCombo->setCurrentIndex(targetCombo->findText(task->target(), Qt::MatchContains));

	descriptionEdit->setText(task->description());
	toolTipEdit->setPlainText(task->toolTip());

	iconCombo->clear();
	this->fillIconCombo(":/types");
	this->fillIconCombo(":/marks");
	this->fillIconCombo(":/control");
	iconCombo->setCurrentIndex(iconCombo->findText(QFileInfo(task->iconSource()).fileName()));

	connect(exclusiveChk, SIGNAL(stateChanged(int)), this, SLOT(controlGroupSpin(int)));
	exclusiveChk->setCheckState(task->group() < 0 ? Qt::Unchecked : Qt::Checked);
	this->controlGroupSpin(exclusiveChk->checkState());
	groupSpin->setValue(task->group());

	connect(addDataBtn, SIGNAL(clicked()), SLOT(addData()));
	connect(removeDataBtn, SIGNAL(clicked()), SLOT(removeData()));

	schemaEdit->setText(task->schema());
	foreach(QURenameData *data, task->data()) {
		dataTable->appendRow();
		dataTable->item(dataTable->rowCount() - 1, 0)->setText(data->_if.isEmpty() ? "true" : data->_if);

		if(data->_keepSuffix) {
			dataTable->item(dataTable->rowCount() - 1, 1)->setText(KEEP_SUFFIX_SOURCE);
		} else if(!data->_text.isEmpty()) {
			dataTable->item(dataTable->rowCount() - 1, 1)->setText(TEXT_SOURCE);
			dataTable->item(dataTable->rowCount() - 1, 2)->setText(data->_text);
		} else if(!data->_source.isEmpty()) {
			dataTable->item(dataTable->rowCount() - 1, 1)->setText(data->_source);
			dataTable->item(dataTable->rowCount() - 1, 2)->setText(data->_default.isEmpty() ? N_A : data->_default);
		} else if(data->_keepUnknownTags) {
			dataTable->item(dataTable->rowCount() - 1, 1)->setText(UNKNOWN_TAGS_SOURCE);
		}
	}

	this->setWindowTitle(QString("Edit Task: \"%1\"").arg(task->configFileName()));
	_fileName = task->configFileName();

	// connect buttons
	connect(saveBtn, SIGNAL(clicked()), this, SLOT(saveRenameTask()));
	connect(saveAsBtn, SIGNAL(clicked()), this, SLOT(saveRenameTaskAs()));
	connect(cancelBtn, SIGNAL(clicked()), this, SLOT(reject()));
}

/*!
 * Add all icons to the iconCombo that are found. Present icons are NOT removed.
 */
void QURenameTaskDialog::fillIconCombo(const QString &resourcePath) {
	QDir iconDir(resourcePath); // reads the resource file

	QFileInfoList iconFiList = iconDir.entryInfoList(QStringList("*.png"), QDir::Files);

	foreach(QFileInfo iconFi, iconFiList) {
		iconCombo->addItem(QIcon(iconFi.absoluteFilePath()), iconFi.fileName(), iconFi.absoluteFilePath());
	}
}

/*!
 * Enables or disables the spinbox for the group number considering the exclusive checkbox.
 */
void QURenameTaskDialog::controlGroupSpin(int exclusiveState) {
	groupSpin->setEnabled(exclusiveState == Qt::Checked);
}

/*!
 * Appends a new data field to the schema.
 */
void QURenameTaskDialog::addData() {
	dataTable->appendRow();
	schemaEdit->insert("%" + QVariant(dataTable->rowCount()).toString());
}

/*!
 * Removes the last added data field from the schema.
 */
void QURenameTaskDialog::removeData() {
	schemaEdit->setText(schemaEdit->text().remove("%" + QVariant(dataTable->rowCount()).toString()));
	dataTable->removeLastRow();
}

void QURenameTaskDialog::saveRenameTask() {
	this->saveRenameTask(QCoreApplication::applicationDirPath() + "/task-def/" + this->_fileName);
	this->accept();
}

/*!
 * Collect the info which is present in this dialog, create a DOM document and save the
 * XML config file to disk.
 */
void QURenameTaskDialog::saveRenameTask(const QString &filePath) {
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
	}
}

void QURenameTaskDialog::saveRenameTaskAs() {
	QString filePath = QFileDialog::getSaveFileName(this, tr("Save task config"), QCoreApplication::applicationDirPath() + "/task-def", tr("Task Configurations (*.xml)"));

	if(!filePath.isEmpty()) {
		this->saveRenameTask(filePath);
		this->accept();
	}
}
