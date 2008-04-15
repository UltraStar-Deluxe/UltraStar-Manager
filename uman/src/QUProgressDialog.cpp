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
		QWidget *parent): QDialog(parent)
{
	setupUi(this);
	
	this->setWindowFlags(Qt::Tool | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
	
	progress->setRange(0, maximum);
	progress->setValue(0);
		
	infoTextLbl->setText(info);
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
	timeLbl->setText(QString("%1 of %2").arg(progress->value()).arg(progress->maximum()));
	
	QWidget::repaint();
}

void QUProgressDialog::setInformation(const QString &infoText) {
	infoTextLbl->setText(infoText);
	
	QWidget::repaint();
}

void QUProgressDialog::setPixmap(const QString &fileName) {
	currentSongIcon->setPixmap(fileName);
}
