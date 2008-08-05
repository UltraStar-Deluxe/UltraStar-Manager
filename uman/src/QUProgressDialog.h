#ifndef QUPROGRESSDIALOG_H_
#define QUPROGRESSDIALOG_H_

#include <QString>
#include <QDialog>

#include "ui_QUProgressDialog.h"

class QUProgressDialog: public QDialog, private Ui::QUProgressDialog {
	Q_OBJECT

public:
	QUProgressDialog(const QString &info, int maximum, QWidget *parent = 0, bool beResponsive = true);

	virtual void show();

	bool cancelled() const { return _cancelled; }

public slots:
	void update(const QString &itemText);
	void setMaximum(int maximum) { progress->setMaximum(maximum); }

	void setPixmap(const QString &fileName);

private slots:
	void cancel();
	void updateTime();

private:
	bool _cancelled;
	bool _beResponsive; // you can only cancel an operation with a responsive progress dialog
	int  _time;
};

#endif /*QUPROGRESSDIALOG_H_*/
