#ifndef QUMAINWINDOW_H
#define QUMAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QDir>

#include "QUSongItem.h"
#include "ui_QUMainWindow.h"

/*!
 * This class represents the main user interface of uman. All important tasks
 * will be offered to the user here.
 */
class QUMainWindow : public QMainWindow, private Ui::QUMainWindow {
	Q_OBJECT

public:
	QUMainWindow(QWidget *parent = 0);
	~QUMainWindow();
	
	static QDir _baseDir;
	
private slots:
	void initConfig();
	void initWindow();
	void initMenu();
	void initSongTree();
	void initSongTreeHeader();
	void initDetailsTable();
	void initTaskList();
	void initMonty();
	
	void refreshSongs();
	void createSongFiles();
	void createSongTree();
	
	void updateDetails();
	void updateStatusbar();
	void updateImage();
	void resizeToContents();
	void resetLink(QTreeWidgetItem *item, int column);
	
	void saveSongChanges(QTableWidgetItem *item);
	
	void checkAllTasks();
	void uncheckAllTasks();
	void uncheckAllExclusiveTasks(QListWidgetItem *item);
	void doTasks();
	
	void useID3TagForArtist(QUSongFile *song);
	void useID3TagForTitle(QUSongFile *song);
	void useID3TagForGenre(QUSongFile *song);
	void useID3TagForYear(QUSongFile *song);
	
	void renameSongDir(QUSongFile *song);
	void renameSongDirCheckedVideo(QUSongFile *song);
	void renameSongTxt(QUSongFile *song);
	void renameSongMp3(QUSongFile *song);
	void renameSongCover(QUSongFile *song);
	void renameSongBackground(QUSongFile *song);
	void renameSongVideo(QUSongFile *song);
	void renameSongVideogap(QUSongFile *song);
	
	void removeUnsupportedTags(QUSongFile *song);
	
	void aboutQt();
	void aboutUman();
	
	void toggleRelativeSongPath(bool checked);
	void toggleCompleterChk(bool checked);
	void editTagOrder();
	void changeSongDir();
	
	void montyTalk();
	
	
private:
	QList<QUSongFile*> _songs;
	
	void addLogMsg(const QString &msg, int type = 0);
	
	void readSongDir(QList<QDir> &dirList);
};

#endif // QUMAINWINDOW_H
