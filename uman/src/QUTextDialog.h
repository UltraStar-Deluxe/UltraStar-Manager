#ifndef QUTEXTDIALOG_H_
#define QUTEXTDIALOG_H_

#include "QUSongFile.h"

#include "ui_QUTextDialog.h"

#include <QDialog>

class QUTextDialog: public QDialog, private Ui::QUTextDialog {
	Q_OBJECT

public:
	QUTextDialog(QUSongFile *song, QWidget *parent = 0, bool showLyrics = false);

private:
	void showLyrics(QUSongFile *song);
	void showFile(QUSongFile *song);

	void initFile(QUSongFile *song);
	void initLyrics(QUSongFile *song);
};

#endif /*QUTEXTDIALOG_H_*/
