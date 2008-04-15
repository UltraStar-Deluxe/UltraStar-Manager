#ifndef QUPROGRESSDIALOG_H_
#define QUPROGRESSDIALOG_H_

#include <QString>
#include <QDialog>
#include <QTimerEvent>
#include <QTime>
#include <QTreeWidgetItem>
#include <QList>

#include "QUAbstractThread.h"
#include "ui_QUProgressDialog.h"

class QUProgressDialog: public QDialog, private Ui::QUProgressDialog {
	Q_OBJECT
	
public:
	QUProgressDialog(const QString &info, const QList<QTreeWidgetItem*> &items, QUAbstractThread *thread, QWidget *parent = 0);
	
	int exec();
	
protected:
	void timerEvent(QTimerEvent *event);
	
private slots:
	void update(const QString &itemText);
	void updateItems();
	
private:
	QUAbstractThread *t;
	QTime time;
	QList<QTreeWidgetItem*> items;
};

#endif /*QUPROGRESSDIALOG_H_*/
