#include "QUProgressDialog.h"
#include <QPixmap>
#include <QTimer>

#define HIDE_LIMIT 2 // only show this dialog above HIDE_LIMIT

/*!
 * \param thread The thread which is executed. Will be deleted after finishing.
 * \param parent parent object
 */
QUProgressDialog::QUProgressDialog(
		const QString &info,
		int maximum,
		QWidget *parent,
		bool beResponsive): QDialog(parent), _cancelled(false), _beResponsive(beResponsive), _time(0)
{
	setupUi(this);

	this->setWindowFlags(Qt::Tool | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

	progress->setRange(0, maximum);
	progress->setValue(0);

	infoTextLbl->setText(info);

	if(beResponsive) {
		connect(cancelBtn, SIGNAL(clicked()), this, SLOT(cancel()));
		timeLbl->setText("00:00:00");
		QTimer::singleShot(1000, this, SLOT(updateTime()));
	} else {
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

void QUProgressDialog::setPixmap(const QString &fileName) {
	currentSongIcon->setPixmap(fileName);
}

void QUProgressDialog::cancel() {
	_cancelled = true;
}

void QUProgressDialog::updateTime() {
	_time += 1;
	timeLbl->setText(QString("%1:%2:%3")
			.arg((int)(_time / 3600), 2, 10, QChar('0'))
			.arg((int)(_time / 60), 2, 10, QChar('0'))
			.arg((int)(_time % 60), 2, 10, QChar('0')));
	QTimer::singleShot(1000, this, SLOT(updateTime()));
}
