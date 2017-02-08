#include "QUEncodingsDialog.h"
#include "QUSongFile.h"

#include <QSettings>
#include <QMessageBox>

#include "QUSongSupport.h"

QUEncodingsDialog::QUEncodingsDialog(QWidget *parent): QDialog(parent) {
	setupUi(this);

	//QSettings settings;
	QStringList items = QUSongSupport::allowedEncodingTypes();

	foreach(QString item, items) {
		inputEncodingsComboBox->addItem(item);
		outputEncodingsComboBox->addItem(item);
	}

	inputEncodingsComboBox->setCurrentIndex(inputEncodingsComboBox->findText(QUSongSupport::defaultInputEncoding()));
	outputEncodingsComboBox->setCurrentIndex(outputEncodingsComboBox->findText(QUSongSupport::defaultOutputEncoding()));

	connect(applyBtn, SIGNAL(clicked()), SLOT(saveEncodings()));
	connect(cancelBtn, SIGNAL(clicked()), SLOT(reject()));
	connect(defaultBtn, SIGNAL(clicked()), SLOT(resetEncodings()));
}

QUEncodingsDialog::~QUEncodingsDialog() {
}

void QUEncodingsDialog::saveEncodings() {
	QSettings settings;
	settings.setValue("defaultInputEncoding", inputEncodingsComboBox->currentText());
	settings.setValue("defaultOutputEncoding", outputEncodingsComboBox->currentText());

	accept();
}

/*!
 * Reset the order to the default value.
 */
void QUEncodingsDialog::resetEncodings() {
	inputEncodingsComboBox->setCurrentIndex(0);
	outputEncodingsComboBox->setCurrentIndex(0);
}
