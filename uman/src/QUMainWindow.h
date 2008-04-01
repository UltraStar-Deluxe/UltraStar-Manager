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
	~QUMainWindow() {}
	
private slots:
	void initTable();
	void initSongTableHeader();
	
	void createSongFiles();
	void createSongTree();
	
	QUSongItem* createSongTreeTopLevelItem(QUSongFile *song);
	void createSongTreeTxtItem(QTreeWidgetItem *parent, QUSongFile *song);
	void createSongTreeMp3Items(QTreeWidgetItem *parent, QUSongFile *song);
	void createSongTreeJpgItems(QTreeWidgetItem *parent, QUSongFile *song);
	void createSongTreeMpgItems(QTreeWidgetItem *parent, QUSongFile *song);
	
	void updateDetails();
	void updateImage();
	void resizeToContents();
	void resetLink(QTreeWidgetItem *item, int column);
	
	void saveSongChanges(QTableWidgetItem *item);
	
	void doTasks();
	
	void renameSongDir(QUSongFile *song);
	void renameSongTxt(QUSongFile *song);
	void renameSongMp3(QUSongFile *song);
	void renameSongCover(QUSongFile *song);
	void renameSongBackground(QUSongFile *song);
	void renameSongVideo(QUSongFile *song);
	
	void aboutQt();
	void aboutUman();
	
private:
	QDir _baseDir;
	QList<QUSongFile*> _songs;
	
	void addLogMsg(const QString &msg);
};

#endif // QUMAINWINDOW_H
