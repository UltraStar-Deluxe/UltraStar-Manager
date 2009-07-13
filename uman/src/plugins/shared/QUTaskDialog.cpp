#include "QUTaskDialog.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>
#include <QFileDialog>
#include <QTextStream>
#include <QIcon>

QUTaskDialog::QUTaskDialog(QUScriptableTask *task, QWidget *parent): QDialog(parent) {
	initForTask();

	if(task)
		initForTask(task);
}

/*!
 * General setup for a blank rename task.
 */
void QUTaskDialog::initForTask() {
	setupUi(this);

	this->setWindowIcon(QIcon(":/control/tasks_add.png"));
	this->setWindowTitle(tr("Add Task"));

	targetCombo->clear();

	iconCombo->clear();
	this->fillIconCombo(":/types");
	this->fillIconCombo(":/marks");
	this->fillIconCombo(":/control");
	this->fillIconCombo(":/bullets");

	exclusiveChk->setCheckState(Qt::Unchecked);
	groupSpin->setEnabled(false);

	removeDataBtn->setEnabled(false);

	connect(exclusiveChk, SIGNAL(stateChanged(int)), this, SLOT(controlGroupSpin(int)));
	connect(addDataBtn, SIGNAL(clicked()), SLOT(addData()));
	connect(removeDataBtn, SIGNAL(clicked()), SLOT(removeData()));

	// connect buttons
	saveBtn->setEnabled(false);
	connect(saveAsBtn, SIGNAL(clicked()), this, SLOT(saveTaskAs()));
	connect(cancelBtn, SIGNAL(clicked()), this, SLOT(reject()));
	connect(moveUpBtn, SIGNAL(clicked()), dataTable, SLOT(moveUpCurrentRow()));
	connect(moveDownBtn, SIGNAL(clicked()), dataTable, SLOT(moveDownCurrentRow()));

	connect(dataTable, SIGNAL(currentCellChanged(int, int, int, int)), this, SLOT(updateMoveButtons(int, int)));
	this->updateMoveButtons(0, 0);
}

/*!
 * Use given task to fill the dialog. Used for editing.
 */
void QUTaskDialog::initForTask(QUScriptableTask *task) {
	// use the basic implementation for untranslated text (should be english)
	descriptionEdit->setText(task->QUSimpleTask::description());
	toolTipEdit->setPlainText(task->QUSimpleTask::toolTip());

	iconCombo->setCurrentIndex(iconCombo->findText(QFileInfo(task->iconSource()).fileName()));

	exclusiveChk->setCheckState(task->group() < 0 ? Qt::Unchecked : Qt::Checked);
	this->controlGroupSpin(exclusiveChk->checkState());
	groupSpin->setValue(task->group());

	schemaEdit->setText(task->schema());

	dataTable->fillData(task->data());

	removeDataBtn->setEnabled(task->data().size() > 0);

	this->setWindowIcon(QIcon(":/control/tasks_edit.png"));
	this->setWindowTitle(QString(tr("Edit Task: \"%1\"")).arg(task->configFileName()));
	_fileName = task->configFileName();

	// normal save button only available in edit mode
	saveBtn->setEnabled(true);
	connect(saveBtn, SIGNAL(clicked()), this, SLOT(saveTask()));
}

/*!
 * Add all icons to the iconCombo that are found. Present icons are NOT removed.
 */
void QUTaskDialog::fillIconCombo(const QString &resourcePath) {
	QDir iconDir(resourcePath); // reads the resource file

	QFileInfoList iconFiList = iconDir.entryInfoList(QStringList("*.png"), QDir::Files);

	foreach(QFileInfo iconFi, iconFiList) {
		iconCombo->addItem(QIcon(iconFi.absoluteFilePath()), iconFi.fileName(), iconFi.absoluteFilePath());
	}
}

/*!
 * Enables or disables the spinbox for the group number considering the exclusive checkbox.
 */
void QUTaskDialog::controlGroupSpin(int exclusiveState) {
	groupSpin->setEnabled(exclusiveState == Qt::Checked);
}

/*!
 * Appends a new data field to the schema.
 */
void QUTaskDialog::addData() {
	dataTable->appendRow();
	schemaEdit->insert("%" + QVariant(dataTable->rowCount()).toString());

	this->removeDataBtn->setEnabled(true);
	this->updateMoveButtons(dataTable->currentRow(), dataTable->currentColumn());
}

/*!
 * Removes the last added data field from the schema.
 */
void QUTaskDialog::removeData() {
	schemaEdit->setText(schemaEdit->text().remove("%" + QVariant(dataTable->rowCount()).toString()));
	dataTable->removeLastRow();

	this->removeDataBtn->setEnabled(dataTable->rowCount() > 0);
	this->updateMoveButtons(dataTable->currentRow(), dataTable->currentColumn());
}

void QUTaskDialog::saveTask() {
	if(this->saveTask(this->configurationDirectory().filePath(_fileName)))
		this->accept();
	else
		this->reject();
}

void QUTaskDialog::saveTaskAs() {
	QString filePath = QFileDialog::getSaveFileName(this, tr("Save task config"), configurationDirectory().path(), tr("Task Configurations (*.xml)"));

	if(!filePath.isEmpty()) {
		if(this->saveTask(filePath))
			this->accept();
		else
			this->reject();
	}
}

/*!
 * Enable or disable the move up/down buttons, according to which action is currently
 * possible.
 */
void QUTaskDialog::updateMoveButtons(int row, int column) {
	moveUpBtn->setEnabled(!(row < 1) and (row < dataTable->rowCount()));
	moveDownBtn->setEnabled(!(row >= dataTable->rowCount() - 1) and (row >= 0));
}

/*!
 * Save the contents of a DOM document into a file.
 */
bool QUTaskDialog::saveDocument(const QString &filePath) {
	QFile file(filePath);

	if(file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
		QTextStream out(&file);
		out << _doc.toString(4);
		file.close();

//		logSrv->add(QString(tr("The task file \"%1\" was saved successfully.")).arg(filePath), QU::Saving);
		return true;
	} else {
//		logSrv->add(QString(tr("The task file \"%1\" was NOT saved.")).arg(filePath), QU::Warning);
		return false;
	}
}

/*!
 * Append the general part of a task config file to a given parent element.
 */
void QUTaskDialog::appendGeneral(QDomElement &parent, QUScriptableTask::TaskTypes type) {
	QDomElement general = _doc.createElement("general");

	QDomElement icon = _doc.createElement("icon");               general.appendChild(icon);
	QDomElement description = _doc.createElement("description"); general.appendChild(description);
	QDomElement tooltip = _doc.createElement("tooltip");         general.appendChild(tooltip);

	if(type == QUScriptableTask::RenameTask)
		general.setAttribute("type", "rename");
	else if(type == QUScriptableTask::AudioTagTask)
		general.setAttribute("type", "id3");

	if(exclusiveChk->checkState() == Qt::Checked)
		general.setAttribute("group", QVariant(groupSpin->value()).toString());
	icon.setAttribute("resource", iconCombo->itemData(iconCombo->currentIndex()).toString());

	if(!descriptionEdit->text().isEmpty()) {
		QDomCDATASection cdataDescr = _doc.createCDATASection(descriptionEdit->text());
		description.appendChild(cdataDescr);
	}
	if(!toolTipEdit->toPlainText().isEmpty()) {
		QDomCDATASection cdataToolt = _doc.createCDATASection(toolTipEdit->toPlainText());
		tooltip.appendChild(cdataToolt);
	}

	parent.appendChild(general);
}
