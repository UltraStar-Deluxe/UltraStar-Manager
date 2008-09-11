#include "QUAboutDialog.h"
#include "main.h"
#include "version.h"
#include <QTimer>
#include <QScrollBar>
#include <QFile>

#define RESET_GAP   2000
#define SCROLL_STEP 1
#define SCROLL_GAP  100

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
		QTimer::singleShot(RESET_GAP, this, SLOT(resetText()));
	else {
		bar->setValue(bar->value() + SCROLL_STEP);
		QTimer::singleShot(SCROLL_GAP, this, SLOT(scrollDown()));
	}
}

void QUAboutDialog::resetText() {
	QScrollBar *bar = credits->verticalScrollBar();
	bar->setValue(bar->minimum());
	QTimer::singleShot(SCROLL_GAP, this, SLOT(scrollDown()));
}
