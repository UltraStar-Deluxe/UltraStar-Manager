#include "QUMainWindow.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDir>
#include <QStringList>
#include <QPixmap>
#include <QIcon>
#include <QBrush>
#include <QHeaderView>
#include <QDateTime>
#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>

#include "QUSongFile.h"
#include "QUDetailItem.h"

QUMainWindow::QUMainWindow(QWidget *parent): QMainWindow(parent) {
	QSettings settings("HPI", "UltraStar Manager");
	QString path = settings.value("songPath", QVariant("D:/Programme/UltraStar/songs")).toString();
	
	path = QFileDialog::getExistingDirectory(this, "Choose your UltraStar song directory", path);
	settings.setValue("songPath", QVariant(path));
	
	_baseDir.setPath(path);
		
	setupUi(this);
	
	setWindowTitle("UltraStar Manager");
	
	initTable();
	
	connect(expandBtn, SIGNAL(clicked()), songTable, SLOT(expandAll()));
	connect(expandBtn, SIGNAL(clicked()), this, SLOT(resizeToContents()));
	connect(collapseBtn, SIGNAL(clicked()), songTable, SLOT(collapseAll()));
	connect(collapseBtn, SIGNAL(clicked()), this, SLOT(resizeToContents()));
	
	connect(taskBtn, SIGNAL(clicked()), this, SLOT(doTasks()));
	
	detailsTable->verticalHeader()->hide();
	detailsTable->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
	detailsTable->setHorizontalHeaderLabels(QStringList() << "Tag" << "Value");
	
	detailsTable->horizontalHeader()->setResizeMode(0, QHeaderView::Interactive);
	detailsTable->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
	
	resize(1000, 600);
	QList<int> sizes;
	sizes << 700 << 300;
	splitter->setSizes(sizes);
	
	connect(detailsTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(saveSongChanges(QTableWidgetItem*)));
	
	connect(actionQt, SIGNAL(triggered()), this, SLOT(aboutQt()));
	connect(actionUman, SIGNAL(triggered()), this, SLOT(aboutUman()));
}

void QUMainWindow::initTable() {
	initSongTableHeader();
	
	connect(songTable, SIGNAL(itemSelectionChanged()), this, SLOT(updateDetails()));

	connect(songTable, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(resetLink(QTreeWidgetItem*, int))); 
	connect(songTable, SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(resizeToContents()));
	connect(songTable, SIGNAL(itemCollapsed(QTreeWidgetItem*)), this, SLOT(resizeToContents()));
	
	createSongFiles();
	createSongTree();
	
	resizeToContents();
	songTable->sortItems(0, Qt::AscendingOrder);
}

void QUMainWindow::initSongTableHeader() {
	QTreeWidgetItem *header = new QTreeWidgetItem();
	header->setText(0, "Folder");
	header->setIcon(0, QIcon(":/types/folder.png"));
	header->setText(1, "Artist");
	header->setIcon(1, QIcon(":/types/user.png"));
	header->setText(2, "Title");
	header->setIcon(2, QIcon(":/types/font.png"));	
	header->setText(3, "MP3");
	header->setIcon(3, QIcon(":/types/music.png"));
	header->setText(4, "Cover");
	header->setIcon(4, QIcon(":/types/picture.png"));
	header->setText(5, "Background");
	header->setIcon(5, QIcon(":/types/picture.png"));
	header->setText(6, "Video");
	header->setIcon(6, QIcon(":/types/film.png"));
	
	songTable->setHeaderItem(header);	
}

void QUMainWindow::createSongFiles() {
	QStringList entries = _baseDir.entryList(QDir::NoDotAndDotDot | QDir::Dirs, QDir::Name);
	
	foreach(QString sub, entries) {
		QDir subDir(_baseDir.path() + "/" + sub);
		QStringList files = subDir.entryList(QStringList("*.txt"), QDir::Files);
		
		if(!files.isEmpty()) {
			_songs.append(new QUSongFile(subDir.path() + "/" + files.first()));
		}
	}
	
	statusBar()->showMessage(QVariant(_songs.size()).toString() + " songs found.");
}

void QUMainWindow::createSongTree() {
	int index = -1;
	
	if(songTable->currentItem()) {
		QTreeWidgetItem *parent = dynamic_cast<QTreeWidgetItem*>(songTable->currentItem()->parent());

		if(parent) {
			// parent = toplevel
			index = songTable->indexOfTopLevelItem(parent);
		} else {
			index = songTable->indexOfTopLevelItem(songTable->currentItem());
		}
	}
	songTable->clear();
	
	foreach(QUSongFile* song, _songs) {
		QUSongItem *top = createSongTreeTopLevelItem(song);
		createSongTreeTxtItem(top, song);
		createSongTreeMp3Items(top, song);
		createSongTreeJpgItems(top, song);
		createSongTreeMpgItems(top, song);
	}
	
	if(index >= 0) {
		songTable->topLevelItem(index)->setSelected(true);
		songTable->topLevelItem(index)->setExpanded(true);
		songTable->setCurrentItem(songTable->topLevelItem(index));
		updateDetails();
	}
}

QUSongItem* QUMainWindow::createSongTreeTopLevelItem(QUSongFile *song) {
	QUSongItem *item = new QUSongItem(song);
				
	item->setText(0, song->songFileInfo().dir().dirName());
	item->setIcon(0, QIcon(":/types/folder.png"));

	if(song->artist() == song->songFileInfo().dir().dirName().section(" - ", 0, 0))
		item->setIcon(1, QIcon(":/marks/tick.png"));
	else if(song->artist().toLower() == song->songFileInfo().dir().dirName().section(" - ", 0, 0).toLower())
		item->setIcon(1, QIcon(":/marks/tick_blue.png"));
	else
		item->setIcon(1, QIcon(":/marks/cross.png"));

	if(song->title() == song->songFileInfo().dir().dirName().section(" - ", 1, 1))
		item->setIcon(2, QIcon(":/marks/tick.png"));
	else if(song->title().toLower() == song->songFileInfo().dir().dirName().section(" - ", 1, 1).toLower())
		item->setIcon(2, QIcon(":/marks/tick_blue.png"));
	else
		item->setIcon(2, QIcon(":/marks/cross.png"));

	if(song->hasMp3()) item->setIcon(3, QIcon(":/marks/tick.png")); else item->setIcon(3, QIcon(":/marks/cross.png")); 
	if(song->hasCover()) item->setIcon(4, QIcon(":/marks/tick.png")); else item->setIcon(4, QIcon(":/marks/cross.png"));
	if(song->hasBackground()) item->setIcon(5, QIcon(":/marks/tick.png")); else item->setIcon(5, QIcon(":/marks/cross.png"));
	if(song->hasVideo()) item->setIcon(6, QIcon(":/marks/tick.png")); else item->setIcon(6, QIcon(":/marks/cross.png"));

	songTable->addTopLevelItem(item);
	
	return item;
}

void QUMainWindow::createSongTreeTxtItem(QTreeWidgetItem *parent, QUSongFile *song) {
	QUSongItem *txtItem = new QUSongItem(song);
	
	txtItem->setText(0, song->songFileInfo().fileName());
	txtItem->setIcon(0, QIcon(":/types/text.png"));
	txtItem->setFontColor(Qt::blue);
	
	parent->addChild(txtItem);
	txtItem->setFlags(Qt::ItemIsEnabled);
}

void QUMainWindow::createSongTreeMp3Items(QTreeWidgetItem *parent, QUSongFile *song) {
	QUSongItem *mp3Item;
	QStringList mp3Files = song->songFileInfo().dir().entryList(QStringList("*.mp3"), QDir::Files);
	
	foreach(QString mp3File, mp3Files) {
		mp3Item = new QUSongItem(song);
		mp3Item->setText(0, mp3File);
		mp3Item->setIcon(0, QIcon(":/types/music.png"));
		
		if(song->mp3().toLower() == mp3File.toLower())
			mp3Item->setIcon(3, QIcon(":/marks/link.png"));
		else
			mp3Item->setFontColor(Qt::gray);
		
		parent->addChild(mp3Item);
		mp3Item->setFlags(Qt::ItemIsEnabled);
	}	
}

void QUMainWindow::createSongTreeJpgItems(QTreeWidgetItem *parent, QUSongFile *song) {
	QUSongItem *jpgItem;
	QStringList jpgFiles = song->songFileInfo().dir().entryList(QStringList("*.jpg"), QDir::Files);
	
	foreach(QString jpgFile, jpgFiles) {
		jpgItem = new QUSongItem(song);
		jpgItem->setText(0, jpgFile);
		jpgItem->setIcon(0, QIcon(":/types/picture.png"));
		
		if(song->cover().toLower() == jpgFile.toLower()) {
			jpgItem->setIcon(4, QIcon(":/marks/link.png"));
			if(song->background().toLower() == jpgFile.toLower())
				jpgItem->setIcon(5, QIcon(":/marks/link.png"));
		} else if(song->background().toLower() == jpgFile.toLower())
			jpgItem->setIcon(5, QIcon(":/marks/link.png"));
		else
			jpgItem->setFontColor(Qt::gray);

		parent->addChild(jpgItem);
		jpgItem->setFlags(Qt::ItemIsEnabled);
	}	
}

void QUMainWindow::createSongTreeMpgItems(QTreeWidgetItem *parent, QUSongFile *song) {
	QUSongItem *mpgItem;
	QStringList mpgFiles = song->songFileInfo().dir().entryList(QStringList("*.mpg"), QDir::Files);
	
	foreach(QString mpgFile, mpgFiles) {
		mpgItem = new QUSongItem(song);
		mpgItem->setText(0, mpgFile);
		mpgItem->setIcon(0, QIcon(":/types/film.png"));
		
		if(song->video().toLower() == mpgFile.toLower())
			mpgItem->setIcon(6, QIcon(":/marks/link.png"));
		else
			mpgItem->setFontColor(Qt::gray);				
		
		parent->addChild(mpgItem);
		mpgItem->setFlags(Qt::ItemIsEnabled);
	}	
}

void QUMainWindow::updateImage() {
//	QUSongItem *songItem = dynamic_cast<QUSongItem*>(songTable->currentItem());
//	
//	if(!songItem)
//		return;
//	
//	if(songTable->currentColumn() == 4) { // show cover
//		imageLbl->setPixmap(QPixmap(songItem->song()->coverFileInfo().filePath()));		
//	} else if(songTable->currentColumn() == 5) { // show background
//		imageLbl->setPixmap(QPixmap(songItem->song()->backgroundFileInfo().filePath()));		
//	}
}

void QUMainWindow::resizeToContents() {
	for(int i = 0; i < songTable->columnCount(); i++)
		songTable->resizeColumnToContents(i);
}

void QUMainWindow::resetLink(QTreeWidgetItem *item, int column) {
	QUSongItem *songItem = dynamic_cast<QUSongItem*>(item);
	
	if(!songItem)
		return;
	
	QUSongFile *song = songItem->song();
	
	if( songItem->icon(3).isNull() and songItem->text(0).endsWith(".mp3") and column == 3 ) {
		song->setInfo("MP3", songItem->text(0));
		song->save();
	} else if( songItem->icon(4).isNull() and songItem->text(0).endsWith(".jpg") and column == 4 ) {
		song->setInfo("COVER", songItem->text(0));
		song->save();
	} else if( songItem->icon(5).isNull() and songItem->text(0).endsWith(".jpg") and column == 5 ) {
		song->setInfo("BACKGROUND", songItem->text(0));
		song->save();
	} else if( songItem->icon(6).isNull() and songItem->text(0).endsWith(".mpg") and column == 6 ) {
		song->setInfo("VIDEO", songItem->text(0));
		song->save();
	}


	createSongTree();
}

void QUMainWindow::updateDetails() {
	QUSongItem *songItem = dynamic_cast<QUSongItem*>(songTable->currentItem());
	
	if(!songItem)
		return;
	
	QUSongFile *song = songItem->song();

	detailsTable->clearContents();
	
	detailsTable->setItem(0, 0, new QTableWidgetItem(QIcon(":/types/user.png"), "Artist"));
	detailsTable->setItem(1, 0, new QTableWidgetItem(QIcon(":/types/font.png"), "Title"));
	detailsTable->setItem(2, 0, new QTableWidgetItem(QIcon(":/types/music.png"), "MP3"));
	detailsTable->setItem(3, 0, new QTableWidgetItem(QIcon(":/types/picture.png"), "Cover"));
	detailsTable->setItem(4, 0, new QTableWidgetItem(QIcon(":/types/picture.png"), "Background"));
	detailsTable->setItem(5, 0, new QTableWidgetItem(QIcon(":/types/film.png"), "Video"));
	
	for(int i = 0; i < 6; i++)
		detailsTable->item(i, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
	
	detailsTable->setItem(0, 1, new QUDetailItem(song->artist(), "ARTIST", song));
	detailsTable->setItem(1, 1, new QUDetailItem(song->title(), "TITLE", song));
	detailsTable->setItem(2, 1, new QUDetailItem(song->mp3(), "MP3", song));
	detailsTable->setItem(3, 1, new QUDetailItem(song->cover(), "COVER", song));
	detailsTable->setItem(4, 1, new QUDetailItem(song->background(), "BACKGROUND", song));
	detailsTable->setItem(5, 1, new QUDetailItem(song->video(), "VIDEO", song));

	for(int i = 0; i < 6; i++)
		detailsTable->item(i, 1)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
}

void QUMainWindow::saveSongChanges(QTableWidgetItem *item) {
	QUDetailItem *detailItem = dynamic_cast<QUDetailItem*>(detailsTable->currentItem());
	
	if(!detailItem)
		return;
	
	QUSongFile *song = detailItem->song();
	
	song->setInfo(detailItem->tag(), detailItem->text());
	song->save();
	
	createSongTree();
}

void QUMainWindow::doTasks() {
	foreach(QTreeWidgetItem *item, songTable->selectedItems()) {
		QUSongItem *songItem = dynamic_cast<QUSongItem*>(item);

		if(songItem) {	
			QUSongFile *song = songItem->song();

			if(renameDirChk->checkState() == Qt::Checked)
				renameSongDir(song);
			if(renameTxtChk->checkState() == Qt::Checked)
				renameSongTxt(song);
			if(renameMp3Chk->checkState() == Qt::Checked)
				renameSongMp3(song);
			if(renameCoverChk->checkState() == Qt::Checked)
				renameSongCover(song);
			if(renameBackgroundChk->checkState() == Qt::Checked)
				renameSongBackground(song);
			if(renameVideoChk->checkState() == Qt::Checked)
				renameSongVideo(song);

			song->save();
		}
	}
	createSongTree();
}

void QUMainWindow::renameSongDir(QUSongFile *song) {
	QString oldName(song->songFileInfo().dir().dirName());
	
	if(song->renameSongDir(song->artist() + " - " + song->title())) {
		addLogMsg("Song directory renamed from: \"" + oldName + "\" to: \"" + song->songFileInfo().dir().dirName() + "\".");
	} else {
		addLogMsg("Song directory: \"" + oldName + "\" was NOT renamed.");
	}
}

void QUMainWindow::renameSongTxt(QUSongFile *song) {

	QString oldName(song->songFileInfo().fileName());
	
	if(song->renameSongTxt(song->artist() + " - " + song->title() + ".txt")) {
		addLogMsg("Song text file renamed from: \"" + oldName + "\" to: \"" + song->songFileInfo().fileName() + "\".");
	} else {
		addLogMsg("Song text file: \"" + oldName + "\" was NOT renamed.");
	}	
}

void QUMainWindow::renameSongMp3(QUSongFile *song) {
	QString oldName(song->mp3());
	
	if(song->renameSongMp3(song->artist() + " - " + song->title() + ".mp3")) {
		addLogMsg("MP3 file renamed from: \"" + oldName + "\" to: \"" + song->mp3() + "\".");
	} else {
		addLogMsg("MP3 file: \"" + oldName + "\" was NOT renamed.");
	}	
}

void QUMainWindow::renameSongCover(QUSongFile *song) {
	QString oldName(song->cover());
	
	if(song->renameSongCover(song->artist() + " - " + song->title() + " [CO].jpg")) {
		addLogMsg("Cover file renamed from: \"" + oldName + "\" to: \"" + song->cover() + "\".");
	} else {
		addLogMsg("Cover file: \"" + oldName + "\" was NOT renamed.");
	}	
}

void QUMainWindow::renameSongBackground(QUSongFile *song) {
	QString oldName(song->background());
	
	if(song->renameSongBackground(song->artist() + " - " + song->title() + " [BG].jpg")) {
		addLogMsg("Background file renamed from: \"" + oldName + "\" to: \"" + song->background() + "\".");
	} else {
		addLogMsg("Background file: \"" + oldName + "\" was NOT renamed.");
	}	
}

void QUMainWindow::renameSongVideo(QUSongFile *song) {
	QString oldName(song->video());
	
	if(song->renameSongVideo(song->artist() + " - " + song->title() + ".mpg")) {
		addLogMsg("Video file renamed from: \"" + oldName + "\" to: \"" + song->video() + "\".");
	} else {
		addLogMsg("Video file: \"" + oldName + "\" was NOT renamed.");
	}	
}

void QUMainWindow::addLogMsg(const QString &msg) {
	log->insertItem(0, QDateTime::currentDateTime().toString("[hh:mm:ss] ") + msg);
}

void QUMainWindow::aboutQt() {
	QMessageBox::aboutQt(this, "About Qt");
}

void QUMainWindow::aboutUman() {
	QMessageBox::about(this, "About", "<b>UltraStar Manager</b><br>Version 1.2<br><br>©2008 by Marcel Taeumel<br><br><i>Tested By</i><br>Michael Grünewald");
}
