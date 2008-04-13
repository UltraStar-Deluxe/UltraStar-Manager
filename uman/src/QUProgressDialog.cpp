#include "QUProgressDialog.h"

/*!
 * \param thread The thread which is executed. Will be deleted after finishing.
 * \param parent parent object
 */
QUProgressDialog::QUProgressDialog(QUAbstractThread *thread, QWidget *parent): QDialog(parent) {
	setupUi(this);
	progress->setRange(0, thread->count());
	progress->setValue(0);
	
	t = thread;
	
	connect(t, SIGNAL(continued(const QString&)), this, SLOT(update(const QString &)));
	//connect(t, SIGNAL(finished()), this, SLOT(accept()));
}

void QUProgressDialog::update(const QString &itemText) {
	progress->setValue(progress->value() + 1);
	currentSongLbl->setText(itemText);
}

void QUProgressDialog::show() {
	t->start();
	
	QDialog::show();
}
