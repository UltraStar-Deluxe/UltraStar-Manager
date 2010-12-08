#ifndef QUMAINWINDOW_H
#define QUMAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QDir>
#include <QCloseEvent>
#include <QAction>
#include <QTimer>

#include "QUSongFile.h"
#include "ui_QUMainWindow.h"

#include "QU.h"

class QURibbonBar;

/*!
 * This class represents the main user interface of uman. All important tasks
 * will be offered to the user here.
 */
class QUMainWindow : public QMainWindow, private Ui::QUMainWindow {
	Q_OBJECT

public:
	QUMainWindow(QWidget *parent = 0);
	~QUMainWindow();

protected:
	virtual void closeEvent(QCloseEvent *event);

private slots:
	void initPluginManager();
	void initConfig();
	void initWindow();
	void initRibbonBar();
	void initSongTree();
	void initDetailsTable();
	void initTaskList();
	void initTaskListMenu();
	void initEventLog();
	void initMonty();
	void initMediaPlayer();
	void initStatusBar();

	void refreshAllSongs(bool force = false);

	void updateDetails();
	void updatePreviewTree();
	void updateMergeBtn();
	void updateTasksSlotButtons(int, const QString&);
	void updateViewButtons();

	void editSongSetFileLink(QTreeWidgetItem *item, int column);
	void editSongSetDetail(QTableWidgetItem *item);
	void editSongApplyTasks();
	void editSongLyrics(QUSongFile *song);
	void editSongLyrics(QUSongFile *song, int line);

	void aboutQt();
	void aboutUman();
	void aboutTagLib();
	void aboutBASS();

	void toggleRelativeSongPath(bool checked);
	void toggleCompleterChk(bool checked);
	void toggleAutoSaveChk(bool checked);
	void toggleAltSongTreeChk(bool checked);
	void toggleAlwaysOnTop(bool checked);
	void toggleFullScreenMode();

	void editTagOrder();
	void changeSongDir(const QString &path);
	void editCustomTags();

	void montyTalk(bool force = false);
	void montyTalkNow();
	void montyAsk();
	void montyAnswer();
	void montyPrev();
	void montyNext();

	void showFileContent(QTreeWidgetItem *item, int column);
	void showLyrics(QUSongFile *song);

	void addLogMsg(const QString &msg, QU::MessageTypes type = QU::Information);
	void saveLog();
	void clearLog();

	void toggleFilterFrame(bool checked);
	void applyFilter();
	void hideFilterArea();
	void removeFilter();

	void reportCreate();

	// translations
	void enableEnglish();
	void enableGerman();
	void enablePolish();

	void getCoversFromAmazon(QList<QUSongItem*> items);
	void getCovers();

	// internal easter eggs
	void copyAudioToPath();

	// interaction with media player
	void sendSelectedSongsToMediaPlayer();
	void sendAllSongsToMediaPlayer();
	void sendVisibleSongsToMediaPlayer();
	void sendCurrentPlaylistToMediaPlayer();

	// plugins
	void showPluginDialog();

	// slideshow
	void showCoverSlideShowDialog(QList<QUSongItem*> items);
	void showBackgroundSlideShowDialog(QList<QUSongItem*> items);

	// paths
	void showPathsDialog();

	// status bar
	void clearStatusMessage();

private:
	QURibbonBar *_menu;
	QTimer       _timer;
	QLabel      *_statusIconLbl;
	QLabel      *_statusMessageLbl;
	QToolButton *_toggleEventLogBtn;
};

#endif // QUMAINWINDOW_H
