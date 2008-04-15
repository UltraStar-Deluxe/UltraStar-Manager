#include "QUProgressDialog.h"
#include "QUSongItem.h"
#include <QPixmap>

#define HIDE_LIMIT 2 // shows the dialog only ABOVE that limit of operations

/*!
 * \param thread The thread which is executed. Will be deleted after finishing.
 * \param parent parent object
 */
QUProgressDialog::QUProgressDialog(
		const QString &info, 
		const QList<QTreeWidgetItem*> &items, 
		QUAbstractThread *thread, 
		QWidget *parent): QDialog(parent)
{
	setupUi(this);
	progress->setRange(0, thread->count());
	progress->setValue(0);
	
	t = thread;
	
	connect(t, SIGNAL(finished()), this, SLOT(updateItems()));
	connect(t, SIGNAL(continued(const QString&)), this, SLOT(update(const QString &)));
	
	this->startTimer(1000);
	
	currentSongLbl->setWordWrap(true);
	currentSongLbl->setAlignment(Qt::AlignTop);
	currentSongIcon->setAlignment(Qt::AlignTop);
	
	infoTextLbl->setText(info);
	
	this->items = items;
}

void QUProgressDialog::update(const QString &itemText) {
	progress->setValue(progress->value() + 1);
	currentSongLbl->setText(itemText);
}

void QUProgressDialog::updateItems() {
	currentSongLbl->setText(tr("Refreshing song tree..."));
	currentSongIcon->setPixmap(QPixmap(":/control/refresh.png"));
	
//	foreach(QTreeWidgetItem *item, items) {
//		QUSongItem *songItem = dynamic_cast<QUSongItem*>(item);
//		
//		if(songItem)
//			songItem->update();
//	}
//	
	//this->accept();
}

int QUProgressDialog::exec() {
	t->start();
	
	if(t->count() > HIDE_LIMIT) {	
		return QDialog::exec();
	} else {
		t->wait();
	}
	
	return 1;
}

void QUProgressDialog::timerEvent(QTimerEvent *event) {
	time = time.addSecs(1);
	timeLbl->setText(QString("%1:%2:%3")
			.arg(time.hour(), 2, 10, QChar('0'))
			.arg(time.minute(), 2, 10, QChar('0'))
			.arg(time.second(), 2, 10, QChar('0')));
}
