#ifndef QUMAINWINDOW_H
#define QUMAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QDir>
#include <QCloseEvent>

#include "QUSongItem.h"
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
	
	static QDir BaseDir;
	
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
	
	void refreshAllSongs();
	void createSongFiles();
	void createSongTree();
	
	void updateDetails();
	void updateStatusbar();
	void updateImage();
	void resizeToContents();
	
	void editSongSetFileLink(QTreeWidgetItem *item, int column);
	void editSongSetDetail(QTableWidgetItem *item);
	void editSongApplyTasks();
	
	void aboutQt();
	void aboutUman();
	
	void toggleRelativeSongPath(bool checked);
	void toggleCompleterChk(bool checked);
	void toggleAutoSaveChk(bool checked);
	
	void editTagOrder();
	void changeSongDir();
	
	void montyTalk();
	
	void showSongTextFile(QTreeWidgetItem *item, int column);
	
	void addLogMsg(const QString &msg, QU::EventMessageTypes type = QU::information);
	
	void toggleFilterFrame(bool checked);
	void toggleFilterNegateBtn();
	void applyFilter();
	void removeFilter();
	
	void reportCreate();
	
private:
	QList<QUSongFile*> _songs;
	
	void readSongDir(QList<QDir> &dirList);
	
	QList<QUSongFile*> selectedSongs();
};

#endif // QUMAINWINDOW_H
