#ifndef QUMAINWINDOW_H
#define QUMAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QDir>
#include <QCloseEvent>
#include <QAction>

#include "QUSongFile.h"
#include "ui_QUMainWindow.h"

#include "QU.h"

/*!
 * This class represents the main user interface of uman. All important tasks
 * will be offered to the user here.
 */
class QUMainWindow : public QMainWindow, private Ui::QUMainWindow {
	Q_OBJECT

public:
	QUMainWindow(QWidget *parent = 0);
	~QUMainWindow();

	static QDir BaseDir; // UltraStar song folder

protected:
	virtual void closeEvent(QCloseEvent *event);

private slots:
	void initConfig();
	void initWindow();
	void initMenu();
	void initSongTree();
	void initDetailsTable();
	void initTaskList();
	void initEventLog();
	void initMonty();

	void appendSong(QUSongFile *song);
	void deleteSong(QUSongFile *song);

	void refreshAllSongs(bool force = false);
	void createSongFiles();

	void updateDetails();
	void updatePreviewTree();

	void editSongSetFileLink(QTreeWidgetItem *item, int column);
	void editSongSetDetail(QTableWidgetItem *item);
	void editSongApplyTasks();

	void aboutQt();
	void aboutUman();
	void aboutTagLib();

	void toggleRelativeSongPath(bool checked);
	void toggleCompleterChk(bool checked);
	void toggleAutoSaveChk(bool checked);
	void toggleAltSongTreeChk(bool checked);
	void toggleAlwaysOnTop(bool checked);

	void editTagOrder();
	void changeSongDir();
	void editCustomTags();

	void montyTalk(bool force = false);
	void montyTalkNow();
	void montyAsk();
	void montyAnswer();
	void montyPrev();
	void montyNext();

	void showFileContent(QTreeWidgetItem *item, int column);
	void showLyrics(QUSongFile *song);

	void addLogMsg(const QString &msg, QU::EventMessageTypes type = QU::information);
	void saveLog();
	void clearLog();

	void toggleFilterFrame(bool checked);
	void toggleFilterNegateBtn();
	void applyFilter();
	void hideFilterArea();
	void removeFilter();

	void reportCreate();

	// translations
	void enableEnglish();
	void enableGerman();
	void enablePolish();

	void getCoversFromAmazon(QList<QUSongItem*> items);

	// handle external changes
	void processExternalSongFileChange(QUSongFile *song);

	// internal easter eggs
	void copyAudioToPath();

private:
	QList<QUSongFile*>     _songs;

	QAction               *_noInfos;
	QAction               *_noWarnings;
	QAction               *_noSaveHints;

	void readSongDir(QList<QDir> &dirList);
};

#endif // QUMAINWINDOW_H
