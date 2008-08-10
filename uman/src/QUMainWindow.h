#ifndef QUMAINWINDOW_H
#define QUMAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QDir>
#include <QCloseEvent>

#include "QUSongFile.h"
#include "QUPlaylistFile.h"
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
	void initMonty();

	void appendSong(QUSongFile *song);

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

	void editTagOrder();
	void changeSongDir();

	void montyTalk();

	void showFileContent(QTreeWidgetItem *item, int column);

	void addLogMsg(const QString &msg, QU::EventMessageTypes type = QU::information);

	void toggleFilterFrame(bool checked);
	void toggleFilterNegateBtn();
	void applyFilter();
	void removeFilter();

	void reportCreate();

	// translations
	void enableEnglish();
	void enableGerman();

	// playlists
	void initPlayList();
	void refreshAllPlaylists();
	void createPlaylistFiles();
	void setCurrentPlaylist(int index);
	void updatePlaylistCombo();
	void updateCurrentPlaylistConnections();
	void updateCurrentPlaylistName(const QString &newName);
	void saveCurrentPlaylist();
	void saveCurrentPlaylistAs();

private:
	QList<QUSongFile*>     _songs;
	QList<QUPlaylistFile*> _playlists;

	void readSongDir(QList<QDir> &dirList);
};

#endif // QUMAINWINDOW_H
