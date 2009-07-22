#ifndef QULYRICSEDITORDIALOG_H
#define QULYRICSEDITORDIALOG_H

#include <QDialog>

#include "ui_QULyricsEditorDialog.h"

#include "QUSongFile.h"

class QULyricsEditorDialog: public QDialog, private Ui::QULyricsEditorDialog {
	Q_OBJECT

public:
	QULyricsEditorDialog(QUSongFile *song, QWidget *parent = 0);

	int execAt(int line);

public slots:
	virtual void accept();
	virtual void reject();

	void toggleWhitespace(int state);
	void search(const QString &keyword);

};

#endif // QULYRICSEDITORDIALOG_H
