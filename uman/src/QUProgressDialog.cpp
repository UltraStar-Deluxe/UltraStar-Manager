#include "QUProgressDialog.h"
#include <QPixmap>
#include <QTimer>

#define HIDE_LIMIT       2 // only show this dialog above HIDE_LIMIT
#define MINIMUM_DURATION 500 // show after milliseconds
#define STEP_SIZE        500 // show every milliseconds

/*!
 * \param thread The thread which is executed. Will be deleted after finishing.
 * \param parent parent object
 */
QUProgressDialog::QUProgressDialog(
		const QString &info,
		int maximum,
		QWidget *parent,
		bool beResponsive): QDialog(parent), _cancelled(false), _beResponsive(beResponsive), _progress(0)
{
	setupUi(this);

	this->setWindowFlags(Qt::Tool | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

	progress->setRange(0, maximum);
	progress->setValue(0);

	infoTextLbl->setText(info);

	if(beResponsive) {
		connect(cancelBtn, SIGNAL(clicked()), this, SLOT(cancel()));
//		timeLbl->setText("00:00:00");
//		QTimer::singleShot(1000, this, SLOT(updateTime()));
	} else {
		cancelBtn->setEnabled(false);
//		timeLbl->setText(tr("n/a"));
	}
}

void QUProgressDialog::show() {
	_startTime = QTime::currentTime();
//	if(progress->maximum() > HIDE_LIMIT)
//		QDialog::show();
}

/*!
 * Updates the information of which items is being processed and increases the
 * counter by one.
 */
void QUProgressDialog::update(const QString &itemText) {
	// update internal data
	_progress += 1;
	_label = itemText;

	// defer dialog appearance
	if(!isVisible()) {
		if(_startTime.msecsTo(QTime::currentTime()) >= MINIMUM_DURATION)
			QDialog::show();
		else
			return;
	}

	// only show update in certain steps
	if(_lastStep.msecsTo(QTime::currentTime()) < STEP_SIZE)
		return;

	progress->setValue(_progress);
	currentSongLbl->setText(_label);
	setWindowTitle(QString(tr("Progress (%1 of %2)")).arg(_progress).arg(progress->maximum()));
	updateTime();

	if(_beResponsive)
		QCoreApplication::processEvents(); // <-- more responsive, less performance
	else
		QWidget::repaint(); // <-- more performance, less responsive

	// reset step timer
	_lastStep = QTime::currentTime();
}

void QUProgressDialog::setPixmap(const QString &fileName) {
	currentSongIcon->setPixmap(fileName);
}

void QUProgressDialog::cancel() {
	_cancelled = true;
}

void QUProgressDialog::updateTime() {
	int time = _startTime.secsTo(QTime::currentTime());
	timeLbl->setText(QString("%1:%2:%3")
			.arg((int)(time / 3600), 2, 10, QChar('0'))
			.arg((int)(time / 60), 2, 10, QChar('0'))
			.arg((int)(time % 60), 2, 10, QChar('0')));
//	QTimer::singleShot(1000, this, SLOT(updateTime()));
}
