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
	void checkUsPath();
	void checkPlaylistPath();
	void checkCoverPath();
	void checkLanguagePath();
	void checkSongPath(QListWidgetItem *item);

	void chooseUsPath();
	void choosePlaylistPath();
	void chooseCoverPath();
	void chooseLanguagePath();
	void chooseSongPath();

	void removeSongPath();

	void autoDetectPaths();

private:
	bool _firstTimeSetup;

	bool isUltraStar(const QDir &d) const;
	bool isUltraStarDeluxe(const QDir &d) const;
	bool hasConfigFile(const QDir &d) const;
	bool hasCoversFile(const QDir &d) const;

	void showOk(QLabel *lbl);
	void showError(QLabel *lbl, const QString &tooltip);
	void showProblems(QLabel *lbl, const QString &tooltip);

	void updateHelpText();
};

#endif // QUPATHSDIALOG_H
