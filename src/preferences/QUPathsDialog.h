#ifndef QUPATHSDIALOG_H
#define QUPATHSDIALOG_H

#include "QU.h"

#include <QDialog>
#include <QDir>
#include <QListWidgetItem>

#include "ui_QUPathsDialog.h"

class QUPathsDialog: public QDialog, private Ui::QUPathsDialog {
	Q_OBJECT

public:
	QUPathsDialog(bool firstTimeSetup = false, QWidget *parent = 0);

public slots:
	virtual void accept();

private slots:
	void checkPlaylistPath();
	void checkSongPath(QListWidgetItem *item);

	void choosePlaylistPath();
	void chooseSongPath();

	void removeSongPath();


private:
	bool _firstTimeSetup;

	void showOk(QLabel *lbl);
	void showError(QLabel *lbl, const QString &tooltip);
	void showProblems(QLabel *lbl, const QString &tooltip);

	void updateHelpText();
};

#endif // QUPATHSDIALOG_H
