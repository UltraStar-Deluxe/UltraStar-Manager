#include "QUAboutDialog.h"
#include "main.h"
#include "version.h"
#include <QTimer>
#include <QScrollBar>
#include <QFile>

QUAboutDialog::QUAboutDialog(QWidget *parent): QDialog(parent) {
	setupUi(this);

	QFile f(":/txt/credits");

	if(f.open(QIODevice::ReadOnly | QIODevice::Text)) {
		credits->setHtml(QString(f.readAll()).replace("\n", "<br>"));
		f.close();
	}

	versionLbl->setText(QString(tr("Version: <b>%1.%2.%3</b> #%4"))
			.arg(MAJOR_VERSION)
			.arg(MINOR_VERSION)
			.arg(PATCH_VERSION)
			.arg(QString(revision).remove(QRegExp("(.*:)|\\D"))));

	resetText();
}

void QUAboutDialog::scrollDown() {
	QScrollBar *bar = credits->verticalScrollBar();

	if(bar->value() >= bar->maximum())
		QTimer::singleShot(2000, this, SLOT(resetText()));
	else {
		bar->setValue(bar->value() + 1);
		QTimer::singleShot(100, this, SLOT(scrollDown()));
	}
}

void QUAboutDialog::resetText() {
	QScrollBar *bar = credits->verticalScrollBar();
	bar->setValue(bar->minimum());
	QTimer::singleShot(100, this, SLOT(scrollDown()));
}
