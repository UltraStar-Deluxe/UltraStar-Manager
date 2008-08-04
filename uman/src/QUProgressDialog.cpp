#include "QUProgressDialog.h"
#include <QPixmap>

#define HIDE_LIMIT 2 // only show this dialog above HIDE_LIMIT

/*!
 * \param thread The thread which is executed. Will be deleted after finishing.
 * \param parent parent object
 */
QUProgressDialog::QUProgressDialog(
		const QString &info,
		int maximum,
		QWidget *parent,
		bool beResponsive): QDialog(parent), _cancelled(false), _beResponsive(beResponsive)
{
	setupUi(this);

	this->setWindowFlags(Qt::Tool | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

	progress->setRange(0, maximum);
	progress->setValue(0);

	infoTextLbl->setText(info);

	if(beResponsive)
		connect(cancelBtn, SIGNAL(clicked()), this, SLOT(cancel()));
	else {
		suspendBtn->setEnabled(false);
		cancelBtn->setEnabled(false);
		timeLbl->setText(tr("n/a"));
	}
}

void QUProgressDialog::show() {
	if(progress->maximum() > HIDE_LIMIT)
		QDialog::show();
}

/*!
 * Updates the information of which items is being processed and increases the
 * counter by one.
 */
void QUProgressDialog::update(const QString &itemText) {
	if(!this->isVisible())
		return;

	progress->setValue(progress->value() + 1);
	currentSongLbl->setText(itemText);
	setWindowTitle(QString(tr("Progress (%1 of %2)")).arg(progress->value()).arg(progress->maximum()));

	if(_beResponsive)
		QCoreApplication::processEvents(); // <-- more responsive, less performance
	else
		QWidget::repaint(); // <-- more performance, less responsive

}

void QUProgressDialog::setInformation(const QString &infoText) {
	infoTextLbl->setText(infoText);

	QWidget::repaint();
}

void QUProgressDialog::setPixmap(const QString &fileName) {
	currentSongIcon->setPixmap(fileName);
}

void QUProgressDialog::cancel() {
	_cancelled = true;
}
