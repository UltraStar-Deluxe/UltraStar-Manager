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
	initDialog();

	targetCombo->setCurrentIndex(targetCombo->findText(task->target(), Qt::MatchContains));

	descriptionEdit->setText(task->description());
	toolTipEdit->setPlainText(task->toolTip());

	iconCombo->setCurrentIndex(iconCombo->findText(QFileInfo(task->iconSource()).fileName()));

	exclusiveChk->setCheckState(task->group() < 0 ? Qt::Unchecked : Qt::Checked);
	this->controlGroupSpin(exclusiveChk->checkState());
	groupSpin->setValue(task->group());

	schemaEdit->setText(task->schema());

	if(dynamic_cast<QURenameTask*>(task))
		dataTable->fillData(task->data(), QU::renameTask);
	else if(dynamic_cast<QUAudioTagTask*>(task))
		dataTable->fillData(task->data(), QU::audioTagTask);

	removeDataBtn->setEnabled(task->data().size() > 0);

	this->setWindowIcon(QIcon(":/control/pencil.png"));
	this->setWindowTitle(QString("Edit Task: \"%1\"").arg(task->configFileName()));
	_fileName = task->configFileName();

	// normal save button only available in edit mode
	saveBtn->setEnabled(true);
	connect(saveBtn, SIGNAL(clicked()), this, SLOT(saveTask()));
}

QUTaskDialog::QUTaskDialog(QWidget *parent): QDialog(parent) {
	initDialog();
}

/*!
 * General setup for a blank rename task.
 */
void QUTaskDialog::initDialog() {
	setupUi(this);

	this->setWindowIcon(QIcon(":/marks/plus.png"));
	this->setWindowTitle(QString("Add Task: \"%1\"").arg("unnamed.xml"));

	targetCombo->clear();

	iconCombo->clear();
	this->fillIconCombo(":/types");
	this->fillIconCombo(":/marks");
	this->fillIconCombo(":/control");

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
	if(this->saveTask(QCoreApplication::applicationDirPath() + "/task-def/" + this->_fileName))
		this->accept();
	else
		this->reject();
}

void QUTaskDialog::saveTaskAs() {
	QString filePath = QFileDialog::getSaveFileName(this, tr("Save task config"), QCoreApplication::applicationDirPath() + "/task-def", tr("Task Configurations (*.xml)"));

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
