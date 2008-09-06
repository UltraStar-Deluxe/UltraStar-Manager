#include "main.h"
#include "version.h"

#include "QUMainWindow.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include <QListWidget>
#include <QListWidgetItem>

#include <QDir>
#include <QStringList>
#include <QPixmap>
#include <QIcon>
#include <QBrush>
#include <QHeaderView>
#include <QDateTime>
#include <QSettings>
#include <QMessageBox>
#include <QKeySequence>

#include <QFileDialog>
#include <QFileInfo>
#include <QFileInfoList>
#include <QTextStream>
#include <QProgressDialog>

#include <QDesktopServices>
#include <QUrl>

#include "QUSongItem.h"
#include "QUDetailItem.h"
#include "QUMonty.h"
#include "QUDropDownDelegate.h"
#include "QUDetailsTable.h"

#include "fileref.h"
#include "tag.h"
#include "tstring.h"

#include "QUTagOrderDialog.h"
#include "QUTextDialog.h"
#include "QUProgressDialog.h"
#include "QUReportDialog.h"
#include "QUMessageBox.h"
#include "QUPictureDialog.h"
#include "QUCustomTagsDialog.h"
#include "QUAmazonDialog.h"

QDir QUMainWindow::BaseDir = QDir();
QUMainWindow::QUMainWindow(QWidget *parent): QMainWindow(parent) {
	setupUi(this);

	initWindow();
	initMenu();
	initEventLog();
	initConfig();

	initSongTree();
	initDetailsTable();
	initTaskList();

	initMonty();

	playlistArea->refreshAllPlaylists(&(this->_songs));
}

/*!
 * Save configuration for next application start.
 */
QUMainWindow::~QUMainWindow() {

}

/*!
 * Overloaded to ensure that all changes are saved before closing this application.
 */
void QUMainWindow::closeEvent(QCloseEvent *event) {
	QUMessageBox::Results result;

	if(songTree->hasUnsavedChanges()) {
		result = QUMessageBox::ask(this,
				tr("Quit"),
				tr("<b>Songs</b> have been modified."),
				":/control/save_all.png", tr("Save all changed songs."),
				":/control/bin.png", tr("Discard all changes."),
				":/marks/cancel.png", tr("Cancel this action."));
		if(result == QUMessageBox::first)
			songTree->saveUnsavedChanges();
		else if(result == QUMessageBox::third) {
			event->ignore();
			return;
		}
	}

	if(playlistArea->hasUnsavedChanges()) {
		result = QUMessageBox::ask(this,
				tr("Quit"),
				tr("<b>Playlists</b> have been modified."),
				":/control/save_all.png", tr("Save all changed playlists."),
				":/control/bin.png", tr("Discard all changes."),
				":/marks/cancel.png", tr("Cancel this action."));
		if(result == QUMessageBox::first)
			playlistArea->saveUnsavedChanges();
		else if(result == QUMessageBox::third) {
			event->ignore();
			return;
		}
	}

	QSettings settings;
	settings.setValue("allowMonty", QVariant(actionAllowMonty->isChecked()));
	settings.setValue("windowState", QVariant(this->saveState()));

	settings.setValue("disableInfoMessages", QVariant(_noInfos->isChecked()));
	settings.setValue("disableWarningMessages", QVariant(_noWarnings->isChecked()));
	settings.setValue("disableSaveMessages", QVariant(_noSaveHints->isChecked()));

	settings.setValue("autoSave", QVariant(actionAutoSave->isChecked()));

	this->saveLog();

	event->accept();
}

/*!
 * Initializes the windows registry entry for uman. Lets the user
 * choose a path where the song files are located.
 */
void QUMainWindow::initConfig() {
	QSettings settings;
	QString path;
	if(!settings.contains("songPath")) {
		path = QFileDialog::getExistingDirectory(this, tr("Choose your UltraStar song directory:"));

		if(!path.isEmpty())
			settings.setValue("songPath", QVariant(path));
	} else {
		path = settings.value("songPath").toString();
	}
	BaseDir.setPath(path);

	// read other settings
	          actionAllowMonty->setChecked(settings.value("allowMonty", true).toBool());
	actionShowRelativeSongPath->setChecked(settings.value("showRelativeSongPath", true).toBool());
	         actionAltSongTree->setChecked(settings.value("altSongTree", false).toBool());
	              completerChk->setChecked(settings.value("caseSensitiveAutoCompletion", false).toBool());

	this->restoreState(settings.value("windowState", QVariant()).toByteArray());

	actionAutoSave->setChecked(settings.value("autoSave", QVariant(true)).toBool());
}

/*!
 * Set up initial window size and title text.
 */
void QUMainWindow::initWindow() {
	// create window icon
	QIcon windowIcon;
	windowIcon.addFile(":/icons/cup.png", QSize(16, 16));
	windowIcon.addFile(":/icons/uman32.png", QSize(32, 32));
	windowIcon.addFile(":/icons/uman48.png", QSize(48, 48));
	windowIcon.addFile(":/icons/uman64.png", QSize(64, 64));
	windowIcon.addFile(":/icons/uman72.png", QSize(72, 72));
	windowIcon.addFile(":/icons/uman96.png", QSize(96, 96));
	windowIcon.addFile(":/icons/uman128.png", QSize(128, 128));

	setWindowIcon(windowIcon);
	setWindowTitle(QString("%1%2").arg(tr("UltraStar Manager")).arg(WIP_TEXT));
	resize(1000, 500);

	addDockWidget(Qt::LeftDockWidgetArea, detailsDock);
	addDockWidget(Qt::RightDockWidgetArea, tasksDock);
	addDockWidget(Qt::LeftDockWidgetArea, previewDock); previewDock->hide();
	addDockWidget(Qt::RightDockWidgetArea, eventsDock); eventsDock->hide();
	addDockWidget(Qt::RightDockWidgetArea, playlistDock); playlistDock->hide();

	connect(playlistArea, SIGNAL(finished(const QString&, QU::EventMessageTypes)), this, SLOT(addLogMsg(const QString&, QU::EventMessageTypes)));

	// init filter area
	filterArea->hide();

	connect(filterArea->filterEdit, SIGNAL(returnPressed()), this, SLOT(applyFilter()));
	connect(filterArea->filterBtn, SIGNAL(clicked()), this, SLOT(applyFilter()));
	connect(filterArea->filterCancelBtn, SIGNAL(clicked()), this, SLOT(hideFilterArea()));
	connect(filterArea->filterClearBtn, SIGNAL(clicked()), this, SLOT(removeFilter()));
	connect(filterArea->filterNegateBtn, SIGNAL(clicked()), this, SLOT(toggleFilterNegateBtn()));
	connect(filterArea->filterDuplicatesBtn, SIGNAL(clicked()), songTree, SLOT(filterDuplicates()));

	filterArea->filterTypeCombo->addItems(QStringList() << tr("All Tags") << tr("Information Tags") << tr("File Tags") << tr("Control Tags") << tr("Custom Tags"));
	filterArea->filterTypeCombo->setCurrentIndex(0);

	filterArea->filterNegateBtn->setChecked(false);
}

void QUMainWindow::initMenu() {
	// songs menu
	connect(      actionNewReport, SIGNAL(triggered()), this, SLOT(reportCreate()));
	connect(        actionRefresh, SIGNAL(triggered()), this, SLOT(refreshAllSongs()));

	connect(   actionSaveSelected, SIGNAL(triggered()), songTree, SLOT(saveSelectedSongs()));
	connect(        actionSaveAll, SIGNAL(triggered()), songTree, SLOT(saveUnsavedChanges()));
	connect( actionDeleteSelected, SIGNAL(triggered()), songTree, SLOT(requestDeleteSelectedSongs()));
	connect(  actionMergeSelected, SIGNAL(triggered()), songTree, SLOT(mergeSelectedSongs()));
	connect(      actionExpandAll, SIGNAL(triggered()), songTree, SLOT(expandAll()));
	connect(      actionExpandAll, SIGNAL(triggered()), songTree, SLOT(resizeToContents()));
	connect(    actionCollapseAll, SIGNAL(triggered()), songTree, SLOT(collapseAll()));
	connect(    actionCollapseAll, SIGNAL(triggered()), songTree, SLOT(resizeToContents()));
	connect(actionRefreshSelected, SIGNAL(triggered()), songTree, SLOT(refreshSelectedItems()));

	connect( actionSendToPlaylist, SIGNAL(triggered()), songTree, SLOT(sendSelectedSongsToPlaylist()));
	connect(     actionShowLyrics, SIGNAL(triggered()), songTree, SLOT(requestLyrics()));

	      actionNewReport->setShortcut(Qt::Key_F2);
	actionRefreshSelected->setShortcut(Qt::Key_F5);
	        actionRefresh->setShortcut(Qt::SHIFT + Qt::Key_F5);
	   actionSaveSelected->setShortcut(Qt::CTRL  + Qt::Key_S);
	        actionSaveAll->setShortcut(Qt::CTRL  + Qt::SHIFT + Qt::Key_S);
	 actionSendToPlaylist->setShortcut(Qt::CTRL  + Qt::Key_P);
	     actionShowLyrics->setShortcut(Qt::CTRL  + Qt::Key_L);
	 actionDeleteSelected->setShortcut(Qt::SHIFT + Qt::Key_Delete);
	  actionMergeSelected->setShortcut(Qt::CTRL  + Qt::Key_M);

	// view menu
	connect(actionShowRelativeSongPath, SIGNAL(toggled(bool)), this, SLOT(toggleRelativeSongPath(bool)));
	connect(actionAltSongTree, SIGNAL(toggled(bool)), this, SLOT(toggleAltSongTreeChk(bool)));
	connect(actionFilter, SIGNAL(toggled(bool)), this, SLOT(toggleFilterFrame(bool)));

	detailsDock->toggleViewAction()->setIcon(QIcon(":/control/text_edit.png"));
	tasksDock->toggleViewAction()->setIcon(QIcon(":/control/tasks.png"));
	previewDock->toggleViewAction()->setIcon(QIcon(":/control/file_info.png"));
	playlistDock->toggleViewAction()->setIcon(QIcon(":/control/playlist.png"));
	eventsDock->toggleViewAction()->setIcon(QIcon(":/control/log.png"));

	this->viewBar->addAction(detailsDock->toggleViewAction());
	this->viewBar->addAction(tasksDock->toggleViewAction());
	this->viewBar->addAction(playlistDock->toggleViewAction());
	this->viewBar->addAction(previewDock->toggleViewAction());
	this->viewBar->addAction(eventsDock->toggleViewAction());

	this->menuView->addAction(detailsDock->toggleViewAction());
	this->menuView->addAction(tasksDock->toggleViewAction());
	this->menuView->addAction(playlistDock->toggleViewAction());
	this->menuView->addAction(previewDock->toggleViewAction());
	this->menuView->addAction(eventsDock->toggleViewAction());

	// insert toggle view actions for the toolbars
	this->menuView->addSeparator();

	QMenu *showToolbarMenu = this->menuView->addMenu(tr("Toolbars"));

	showToolbarMenu->addAction(this->songsBar->toggleViewAction());
	showToolbarMenu->addAction(this->viewBar->toggleViewAction());
	showToolbarMenu->addAction(this->optionsBar->toggleViewAction());

	actionFilter->setShortcut(Qt::CTRL + Qt::Key_F);

	// options menu
	connect(actionAutoSave, SIGNAL(toggled(bool)), this, SLOT(toggleAutoSaveChk(bool)));
	connect(actionTagSaveOrder, SIGNAL(triggered()), this, SLOT(editTagOrder()));
	connect(actionChangeSongDirectory, SIGNAL(triggered()), this, SLOT(changeSongDir()));
	connect(actionCustomTags, SIGNAL(triggered()), this, SLOT(editCustomTags()));

	connect(actionLangEnglish, SIGNAL(triggered()), this, SLOT(enableEnglish()));
	connect(actionLangGerman, SIGNAL(triggered()), this, SLOT(enableGerman()));

	actionChangeSongDirectory->setShortcut(Qt::Key_F12);

	// help menu
	connect(actionShowMonty, SIGNAL(triggered()), helpFrame, SLOT(show()));
	connect(actionQt, SIGNAL(triggered()), this, SLOT(aboutQt()));
	connect(actionUman, SIGNAL(triggered()), this, SLOT(aboutUman()));
	connect(actionTagLib, SIGNAL(triggered()), this, SLOT(aboutTagLib()));
}

/*!
 * Set up the song tree the first time.
 * \sa createSongFiles()
 * \sa createSongTree()
 */
void QUMainWindow::initSongTree() {
	songTree->initHorizontalHeader();

	connect(songTree, SIGNAL(itemSelectionChanged()), this, SLOT(updateDetails()));
	connect(songTree, SIGNAL(itemSelectionChanged()), this, SLOT(updatePreviewTree()));

	connect(songTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(editSongSetFileLink(QTreeWidgetItem*, int)));
	connect(songTree, SIGNAL(itemActivated(QTreeWidgetItem*, int)), this, SLOT(showFileContent(QTreeWidgetItem*, int)));

	connect(songTree, SIGNAL(itemExpanded(QTreeWidgetItem*)), songTree, SLOT(resizeToContents()));
	connect(songTree, SIGNAL(itemCollapsed(QTreeWidgetItem*)), songTree, SLOT(resizeToContents()));

	connect(songTree, SIGNAL(finished(const QString&, QU::EventMessageTypes)), this, SLOT(addLogMsg(const QString&, QU::EventMessageTypes)));
	connect(songTree, SIGNAL(songCreated(QUSongFile*)), this, SLOT(appendSong(QUSongFile*)));

	connect(songTree, SIGNAL(songToPlaylistRequested(QUSongFile*)), playlistArea, SLOT(addSongToCurrentPlaylist(QUSongFile*)));
	connect(songTree, SIGNAL(showLyricsRequested(QUSongFile*)), this, SLOT(showLyrics(QUSongFile*)));
	connect(songTree, SIGNAL(coversFromAmazonRequested(QList<QUSongItem*>)), this, SLOT(getCoversFromAmazon(QList<QUSongItem*>)));

	connect(songTree, SIGNAL(deleteSongRequested(QUSongFile*)), this, SLOT(deleteSong(QUSongFile*)));

	refreshAllSongs();
}

void QUMainWindow::initDetailsTable() {
	connect(detailsTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(editSongSetDetail(QTableWidgetItem*)));

	connect(completerChk, SIGNAL(toggled(bool)), this, SLOT(toggleCompleterChk(bool)));
}

/*!
 * Initializes all available tasks.
 * \sa editSongApplyTasks()
 * \sa editSongSetDetail()
 */
void QUMainWindow::initTaskList() {
	addTaskBtn->setMenu(new QMenu);
	addTaskBtn->setPopupMode(QToolButton::InstantPopup);
	addTaskBtn->menu()->addAction(tr("Song/ID3 Tag Task..."), taskList, SLOT(addAudioTagTask()));
	addTaskBtn->menu()->addAction(tr("Rename Task..."), taskList, SLOT(addRenameTask()));

	// connect task buttons
	connect(taskBtn, SIGNAL(clicked()), this, SLOT(editSongApplyTasks()));
	connect(allTasksBtn, SIGNAL(clicked()), taskList, SLOT(checkAllTasks()));
	connect(noTasksBtn, SIGNAL(clicked()), taskList, SLOT(uncheckAllTasks()));

	connect(taskList, SIGNAL(finished(const QString&, QU::EventMessageTypes)), this, SLOT(addLogMsg(const QString&, QU::EventMessageTypes)));
}

void QUMainWindow::initEventLog() {
	logOptionsBtn->setMenu(new QMenu);
	logOptionsBtn->setPopupMode(QToolButton::InstantPopup);

	_noInfos = new QAction(QIcon(":/marks/no_info.png"), tr("Discard Information"), 0);
	_noInfos->setCheckable(true);
	logOptionsBtn->menu()->addAction(_noInfos);

	_noWarnings = new QAction(QIcon(":/marks/no_warning.png"), tr("Discard Warnings"), 0);
	_noWarnings->setCheckable(true);
	logOptionsBtn->menu()->addAction(_noWarnings);

	_noSaveHints = new QAction(QIcon(":/marks/no_save.png"), tr("Discard Save Hints"), 0);
	_noSaveHints->setCheckable(true);
	logOptionsBtn->menu()->addAction(_noSaveHints);

	QSettings settings;
	_noInfos->setChecked(settings.value("disableInfoMessages", QVariant(false)).toBool());
	_noWarnings->setChecked(settings.value("disableWarningMessages", QVariant(false)).toBool());
	_noSaveHints->setChecked(settings.value("disableSaveMessages", QVariant(false)).toBool());

	connect(logClearBtn, SIGNAL(clicked()), this, SLOT(clearLog()));
	connect(logSaveBtn, SIGNAL(clicked()), this, SLOT(saveLog()));
}

void QUMainWindow::initMonty() {
	montyLbl->setPixmap(monty->pic(QUMonty::seated));
	helpLbl->setText(monty->welcomeMsg(_songs.size()));

	connect(hideMontyBtn, SIGNAL(clicked()), helpFrame, SLOT(hide()));
	connect(talkMontyBtn, SIGNAL(clicked()), this, SLOT(montyTalk()));

	if(!actionAllowMonty->isChecked())
		helpFrame->hide();
}

void QUMainWindow::appendSong(QUSongFile *song) {
	_songs.append(song);
}

/*!
 * Deletes the given song (the whole directory). Song item has to be removed
 * from song tree before you call this function!
 */
void QUMainWindow::deleteSong(QUSongFile *song) {
	QDir dir(song->songFileInfo().dir());
	QString artist = song->artist();
	QString title = song->title();

	QFileInfoList fiList = dir.entryInfoList(QStringList("*.*"), QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot, QDir::Name);

	foreach(QFileInfo fi, fiList) {
		if(!QFile::remove(fi.filePath()))
			addLogMsg(QString(tr("Could NOT delete file: \"%1\"")).arg(fi.filePath()), QU::warning);
		else
			addLogMsg(QString(tr("File was deleted successfully: \"%1\"")).arg(fi.filePath()), QU::information);
	}

	QString dirName = dir.dirName();
	dir.cdUp();

	if(!dir.rmdir(dirName))
		addLogMsg(QString(tr("Could NOT delete directory: \"%1\". Maybe it is not empty.")).arg(song->songFileInfo().path()), QU::warning);
	else
		addLogMsg(QString(tr("Directory was deleted successfully: \"%1\"")).arg(song->songFileInfo().path()), QU::information);

	_songs.removeAll(song);
	delete song;

	addLogMsg(QString(tr("Song was deleted successfully: \"%1 - %2\"")).arg(artist).arg(title), QU::information);
}

/*!
 * Re-reads all possible song files and builds a new song tree.
 */
void QUMainWindow::refreshAllSongs(bool force) {
	if(!force && songTree->hasUnsavedChanges()) {
		QUMessageBox::Results result = QUMessageBox::ask(this,
				tr("Rebuild Song Tree"),
				tr("Songs have been modified."),
				":/control/save_all.png", tr("Save all changes."),
				":/control/bin.png", tr("Discard all changes."),
				":/marks/cancel.png", tr("Cancel this action."));
		if(result == QUMessageBox::third)
			return;
		else if(result == QUMessageBox::first)
			songTree->saveUnsavedChanges();
	}

	// -------------------------------------

	playlistArea->disconnectPlaylists();

	songTree->clear();
	updateDetails();

	qDeleteAll(_songs);
	_songs.clear();

	createSongFiles();
	songTree->fill(_songs);
	monty->setSongCount(_songs.size()); // sometimes, Monty talks about your song count...

	updatePreviewTree();
	playlistArea->updateAll();
}

/*!
 * Creates all instances of QUSongFile to fill the song tree.
 * \sa CreateSongTree();
 */
void QUMainWindow::createSongFiles() {
	QList<QDir> dirList;
	dirList.append(QDir(BaseDir));

	this->readSongDir(dirList);

	QUProgressDialog dlg(tr("Reading song files..."), dirList.size(), this);
	dlg.setPixmap(":/types/folder.png");
	dlg.show();

	foreach(QDir dir, dirList) {
		QFileInfoList songFiList = dir.entryInfoList(QU::allowedSongFiles(), QDir::Files, QDir::Name);
		qStableSort(songFiList.begin(), songFiList.end(), QU::fileTypeLessThan);

		dlg.update(dir.dirName());
		if(dlg.cancelled()) break;

		if(!songFiList.isEmpty()) {
			QUSongFile *newSong = new QUSongFile(songFiList.first().filePath());
			// TODO: What about more song files in a folder? Really choose the first one? Hmmm...
			_songs.append(newSong);
			// enable event log
			connect(newSong, SIGNAL(finished(const QString&, QU::EventMessageTypes)), this, SLOT(addLogMsg(const QString&, QU::EventMessageTypes)));
			// connect changes in song files with an update in the playlist area
			connect(newSong, SIGNAL(dataChanged()), playlistArea, SLOT(update()));
			// react to changes
			connect(newSong, SIGNAL(externalSongFileChangeDetected(QUSongFile*)), this, SLOT(processExternalSongFileChange(QUSongFile*)));
		}
	}
}

/*!
 * Reads recursively all available directories and puts them in the dirList.
 */
void QUMainWindow::readSongDir(QList<QDir> &dirList) {
	if(dirList.isEmpty())
		return;

	if(dirList.last().entryList(QDir::NoDotAndDotDot | QDir::Dirs, QDir::Name).isEmpty()) {
		return;
	} else {
		QDir thisDir(dirList.last());
		QStringList subDirs = dirList.last().entryList(QDir::NoDotAndDotDot | QDir::Dirs, QDir::Name);

		foreach(QString dir, subDirs) {
			QDir newDir(thisDir);
			newDir.cd(dir);

			dirList.append(newDir);
			this->readSongDir(dirList);
		}
	}
}

void QUMainWindow::updateDetails() {
//	addLogMsg("QUMainWindow::updateDetails()", QU::help);

	disconnect(detailsTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(editSongSetDetail(QTableWidgetItem*)));

	detailsTable->updateValueColumn(songTree->selectedSongs());

	connect(detailsTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(editSongSetDetail(QTableWidgetItem*)));
}

/*!
 * Updates all preview information according to the current selection or dimension of the
 * song tree.
 */
void QUMainWindow::updatePreviewTree() {
	previewTree->setSongCount(_songs.size());
	previewTree->setSelectedSongCount(songTree->currentItem() ? qMax(songTree->selectedItems().size(), 1) : songTree->selectedItems().size());
	previewTree->setVisibleSongCount(songTree->topLevelItemCount());
	previewTree->setHiddenSongCount(songTree->hiddenItemsCount());

	if(calculateTimeChk->checkState() == Qt::Checked) {
		int totalSongTime = 0;
		int totalAudioTime = 0;
		int totalTotalTime = 0;
		foreach(QUSongFile *song, songTree->selectedSongs()) {
			totalSongTime += song->length();
			totalAudioTime += song->lengthMp3();
			totalTotalTime += song->lengthEffective();
		}
		previewTree->setSelectedSongLength(totalSongTime);
		previewTree->setSelectedAudioLength(totalAudioTime);
		previewTree->setSelectedTotalLength(totalTotalTime);
	} else {
		previewTree->setSelectedSongLength();
		previewTree->setSelectedAudioLength();
		previewTree->setSelectedTotalLength();
	}

	QUSongItem *item = dynamic_cast<QUSongItem*>(songTree->currentItem());

	if(item) {
		QFileInfo fi(item->song()->songFileInfo().dir(), item->text(FOLDER_COLUMN));
		previewTree->showFileInformation(fi);
	} else
		previewTree->showFileInformation(QFileInfo());
}

/*!
 * Save all changes of the details into the song file.
 * \sa updateDetails()
 */
void QUMainWindow::editSongSetFileLink(QTreeWidgetItem *item, int column) {
	if(column < MP3_COLUMN or column > VIDEO_COLUMN)
		return;

	QUSongItem *songItem = dynamic_cast<QUSongItem*>(item);

	if(!songItem || songItem->isToplevel())
		return;

	QUSongFile *song = songItem->song();

	QString fileScheme("*." + QFileInfo(songItem->text(FOLDER_COLUMN)).suffix());

	if( songItem->icon(MP3_COLUMN).isNull()
			and QU::allowedAudioFiles().contains(fileScheme, Qt::CaseInsensitive)
			and column == MP3_COLUMN ) {
		addLogMsg(QString(tr("Audio file changed from \"%1\" to: \"%2\".")).arg(song->mp3()).arg(songItem->text(FOLDER_COLUMN)));
		song->setInfo(MP3_TAG, songItem->text(FOLDER_COLUMN));
		song->save();
	} else if( songItem->icon(COVER_COLUMN).isNull()
			and QU::allowedPictureFiles().contains(fileScheme, Qt::CaseInsensitive)
			and column == COVER_COLUMN ) {
		addLogMsg(QString(tr("Cover changed from \"%1\" to: \"%2\".")).arg(song->cover()).arg(songItem->text(FOLDER_COLUMN)));
		song->setInfo(COVER_TAG, songItem->text(FOLDER_COLUMN));
		song->save();
	} else if( songItem->icon(BACKGROUND_COLUMN).isNull()
			and QU::allowedPictureFiles().contains(fileScheme, Qt::CaseInsensitive)
			and column == BACKGROUND_COLUMN ) {
		addLogMsg(QString(tr("Background changed from \"%1\" to: \"%2\".")).arg(song->background()).arg(songItem->text(FOLDER_COLUMN)));
		song->setInfo(BACKGROUND_TAG, songItem->text(FOLDER_COLUMN));
		song->save();
	} else if( songItem->icon(VIDEO_COLUMN).isNull()
			and QU::allowedVideoFiles().contains(fileScheme, Qt::CaseInsensitive)
			and column == VIDEO_COLUMN ) {
		addLogMsg(QString(tr("Video file changed from \"%1\" to: \"%2\".")).arg(song->video()).arg(songItem->text(FOLDER_COLUMN)));
		song->setInfo(VIDEO_TAG, songItem->text(FOLDER_COLUMN));
		song->save();
	}

	updateDetails();
	(dynamic_cast<QUSongItem*>(songItem->parent()))->update();

	montyTalk();
}


void QUMainWindow::editSongSetDetail(QTableWidgetItem *item) {
	QUDetailItem *detailItem = dynamic_cast<QUDetailItem*>(detailsTable->currentItem());

	if(!detailItem)
		return;

	QUProgressDialog dlg(QString(tr("Applying new value for %1 to all selected songs...")).arg(detailItem->tag()),
			detailItem->songs().size(), this);
	dlg.show();

	// save changes for each song
	foreach(QUSongFile *song, detailItem->songs()) {
		dlg.update(QString("%1 - %2").arg(song->artist()).arg(song->title()));
		if(dlg.cancelled()) break;

		song->setInfo(detailItem->tag(), detailItem->text());
		song->save();
	}
	dlg.hide();

	// update all selected items with these new details in the song tree
	songTree->refreshSelectedItems();
}

/*!
 * Does all checked tasks for all selected songs. You can only select
 * toplevel items (folders) which represent songs.
 * \sa initTaskList();
 */
void QUMainWindow::editSongApplyTasks() {
	QList<QUSongItem*> songItems = songTree->selectedSongItems();
	QList<bool>        itemExpandedStates;

	if(songItems.isEmpty())
		return;

	QUProgressDialog dlg(tr("Applying all checked tasks to all selected songs..."), songItems.size(), this);
	dlg.show();

	songTree->clearSelection();

	foreach(QUSongItem *songItem, songItems) {
		QUSongFile *song = songItem->song();
		itemExpandedStates.append(songItem->isExpanded());

		dlg.update(QString("%1 - %2").arg(song->artist()).arg(song->title()));
		if(dlg.cancelled()) break;

		taskList->doTasksOn(song);

		song->save();

		if(songItem->isToplevel()) { // for performance reasons
			songTree->takeTopLevelItem(songTree->indexOfTopLevelItem(songItem));
			songItem->update();
			songTree->addTopLevelItem(songItem);
		} else
			songItem->update();
	}

	// restore selection
	songTree->setCurrentItem(songItems.first());
	foreach(QUSongItem *item, songItems) {
		if(!itemExpandedStates.isEmpty()) {
			item->setExpanded(itemExpandedStates.first());
			itemExpandedStates.pop_front();
		} else
			break;
	}
	songTree->scrollToItem(songTree->currentItem());
	songTree->restoreSelection(songItems);

	updateDetails();
	montyTalk();
}

void QUMainWindow::addLogMsg(const QString &msg, QU::EventMessageTypes type) {
	if(type == QU::information and _noInfos->isChecked())
		return;
	if(type == QU::warning and _noWarnings->isChecked())
		return;
	if(type == QU::saving and _noSaveHints->isChecked())
		return;

	log->insertItem(0, QDateTime::currentDateTime().toString("[hh:mm:ss] ") + msg);
	QListWidgetItem *lastItem = log->item(0);

	switch(type) {
	case 0: lastItem->setIcon(QIcon(":/marks/information.png")); lastItem->setData(Qt::UserRole, "I"); break;
	case 1: lastItem->setIcon(QIcon(":/marks/error.png")); lastItem->setData(Qt::UserRole, "!"); break;
	case 2: lastItem->setIcon(QIcon(":/marks/help.png")); lastItem->setData(Qt::UserRole, "?"); break;
	case 3: lastItem->setIcon(QIcon(":/marks/disk.png")); lastItem->setData(Qt::UserRole, "S"); break;
	}

}

/*!
 * Save the log a default location.
 */
void QUMainWindow::saveLog() {
	if(log->count() == 0)
		return;

	QDir logDir = QDir(QCoreApplication::applicationDirPath()); logDir.mkdir("logs"); logDir.cd("logs");
	QString filePath = QFileInfo(logDir, QString("%1.txt").arg(QDateTime::currentDateTime().toString("yyMMdd_hhmmss"))).filePath();
	QFile file(filePath);

	if(file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
		QTextStream out(&file);

		for(int row = 0; row < log->count(); row++) {
			out << QString("%1 %2").arg(log->item(row)->data(Qt::UserRole).toString()).arg(log->item(row)->text()) << endl;
		}
		file.close();

		addLogMsg(QString(tr("The log file was saved to: \"%1\"")).arg(filePath), QU::saving);
	} else
		addLogMsg(QString(tr("The log file COULD NOT be saved.")).arg(filePath), QU::warning);
}

void QUMainWindow::clearLog() {
	log->clear();
}

void QUMainWindow::aboutQt() {
	QMessageBox::aboutQt(this, "About Qt");
}

void QUMainWindow::aboutUman() {
	QString aboutStr(tr("<b>UltraStar Manager</b><br>"
			"Version %1.%2.%3 #%4<br>"
			"<br>"
			"©2008 by Marcel Taeumel<br>"
			"<br>"
			"<i>Tested By</i><br>"
			"Michael Grünewald"));

	QMessageBox::about(this, "About", aboutStr
			.arg(MAJOR_VERSION)
			.arg(MINOR_VERSION)
			.arg(PATCH_VERSION)
			.arg(QString(revision).remove(QRegExp("(.*:)|\\D"))));
}

void QUMainWindow::aboutTagLib() {
	QUMessageBox::ask(this,
			tr("About TagLib"),
			QString(tr("<b>TagLib Audio Meta-Data Library</b><br><br>"
					"TagLib is a library for reading and editing the meta-data of several popular audio formats.<br><br>"
					"Version: <b>%1.%2.%3</b><br><br>"
					"Visit: <a href=\"http://developer.kde.org/~wheeler/taglib.html\">TagLib Homepage</a>"))
					.arg(TAGLIB_MAJOR_VERSION)
					.arg(TAGLIB_MINOR_VERSION)
					.arg(TAGLIB_PATCH_VERSION),
			":/marks/accept.png", tr("Continue."));
}

void QUMainWindow::editTagOrder() {
	QUTagOrderDialog *dlg = new QUTagOrderDialog(this);
	dlg->exec();
	delete dlg;

	montyTalk();
}

/*!
 * Enables the user to set up a new location where all
 * UltraStar songs can be found.
 */
void QUMainWindow::changeSongDir() {
	if(songTree->hasUnsavedChanges()) {
		QUMessageBox::Results result = QUMessageBox::ask(this,
				tr("Change Song Directory"),
				tr("Songs have been modified."),
				":/control/save_all.png", tr("Save all changes."),
				":/control/bin.png", tr("Discard all changes."),
				":/marks/cancel.png", tr("Cancel this action."));
		if(result == QUMessageBox::third)
			return;
		else if(result == QUMessageBox::first)
			songTree->saveUnsavedChanges();
	}

	// ---------------------------------------------------------

	QSettings settings;
	QString path = settings.value("songPath").toString();

	path = QFileDialog::getExistingDirectory(this, tr("Choose your UltraStar song directory:"), path);

	if(!path.isEmpty()) {
		settings.setValue("songPath", QVariant(path));
		BaseDir.setPath(path);
		refreshAllSongs(true);

		montyTalk();

		addLogMsg(QString(tr("UltraStar song directory changed to: \"%1\".")).arg(BaseDir.path()));

		songTree->headerItem()->setText(FOLDER_COLUMN, QString(tr("Folder (%1)")).arg(BaseDir.path()));
	}
}

/*!
 * Enables the user to setup custom tags. Modified songs need to be saved first, because
 * the whole song tree (database) will be rebuild. This is an easy way to update the (un-)supported tags
 * for each song. :)
 */
void QUMainWindow::editCustomTags() {
	if(songTree->hasUnsavedChanges()) {
		QUMessageBox::Results result = QUMessageBox::ask(this,
				tr("Custom Tags"),
				tr("Songs have been modified."),
				":/control/save_all.png", tr("Save all changes."),
				":/control/bin.png", tr("Discard all changes."),
				":/marks/cancel.png", tr("Cancel this action."));
		if(result == QUMessageBox::third)
			return;
		else if(result == QUMessageBox::first)
			songTree->saveUnsavedChanges();
	}

	// ---------------------------------------------------------

	QUCustomTagsDialog *dlg = new QUCustomTagsDialog(this);

	if(dlg->exec()) {
		this->refreshAllSongs(true);
		detailsTable->reset();

		this->addLogMsg(QString(tr("Custom tags changed to: \"%1\"")).arg(QUSongFile::customTags().join(", ")), QU::information);
	}

	delete dlg;

	montyTalk();
}

void QUMainWindow::montyTalk() {
	if(!actionAllowMonty->isChecked())
		return;

	helpFrame->show();
	monty->talk(montyLbl, helpLbl);
}

/*!
 * Updates all toplevel items to display the relative song path
 * or only the song directory - like selected.
 */
void QUMainWindow::toggleRelativeSongPath(bool checked) {
	QSettings settings;
	settings.setValue("showRelativeSongPath", QVariant(checked));

	QList<QUSongItem*> tmpList; // tmp list needed so avoid sorting problems

	QUProgressDialog dlg(checked ? tr("Show relative song path for each song...") : tr("Show song folder for each song..."), songTree->topLevelItemCount(), this, false);
	dlg.setPixmap(":/types/folder.png");
	dlg.show();

	for(int i = 0; i < songTree->topLevelItemCount(); i++)
		tmpList.append(dynamic_cast<QUSongItem*>(songTree->topLevelItem(i)));

	foreach(QUSongItem *item, tmpList) {
		dlg.update(item->song()->songFileInfo().dir().dirName());

		item->updateAsDirectory(checked);
	}

	if(checked)
		addLogMsg(tr("Relative song paths are displayed in the song tree now."));
	else
		addLogMsg(tr("Only song directories are displayed in the song tree now."));
}

/*!
 * Toggle whether the completion for detail edition should be case-sensitive.
 */
void QUMainWindow::toggleCompleterChk(bool checked) {
	QSettings settings;
	settings.setValue("caseSensitiveAutoCompletion", QVariant(checked));
}

void QUMainWindow::toggleAutoSaveChk(bool checked) {
	QSettings settings;
	settings.setValue("autoSave", QVariant(checked));

	actionSaveAll->setEnabled(!checked);
}

void QUMainWindow::toggleAltSongTreeChk(bool checked) {
	QSettings settings;
	if(settings.value("altSongTree", false) == checked)
		return; // no change

	settings.setValue("altSongTree", checked);

	QUProgressDialog dlg(tr("Repainting song tree icons..."), songTree->topLevelItemCount(), this, false);
	dlg.setPixmap(":/types/folder.png");
	dlg.show();

	for(int i = 0; i < songTree->topLevelItemCount(); i++) {
		QUSongItem *songItem = dynamic_cast<QUSongItem*>(songTree->topLevelItem(i));

		if(songItem) {
			dlg.update(songItem->song()->songFileInfo().dir().dirName());

			songItem->updateSpellFileCheckColumns();
		}
	}
}

/*!
 * Shows the content of the current file. Another text-file will be set as new song text
 * file.
 */
void QUMainWindow::showFileContent(QTreeWidgetItem *item, int column) {
	if(column != FOLDER_COLUMN)
		return;

	QUSongItem *songItem = dynamic_cast<QUSongItem*>(item);

	if(!songItem || songItem->isToplevel())
		return;

	QString fileScheme("*." + QFileInfo(songItem->text(FOLDER_COLUMN)).suffix());

	if(QString::compare(item->text(FOLDER_COLUMN), songItem->song()->songFileInfo().fileName(), Qt::CaseInsensitive) == 0) {
		// show the (raw) content of the current song text file
		QUTextDialog *dlg = new QUTextDialog(songItem->song(), this);
		dlg->exec();
		delete dlg;
	} else if(QU::allowedSongFiles().contains(fileScheme, Qt::CaseInsensitive)) {
		// use this song text file for the folder now
		// --> useful for more than one valid song text file per folder
		songItem->song()->setFile(QFileInfo(songItem->song()->songFileInfo().dir(), item->text(FOLDER_COLUMN)).filePath());

		songTree->setCurrentItem(songItem->parent());
		songItem->update();
	} else if(QU::allowedPictureFiles().contains(fileScheme, Qt::CaseInsensitive)) {
		QUPictureDialog dlg(QFileInfo(songItem->song()->songFileInfo().dir(), songItem->text(FOLDER_COLUMN)).filePath(), this);
		dlg.exec();
	} else if(QU::allowedAudioFiles().contains(fileScheme, Qt::CaseInsensitive) or QU::allowedVideoFiles().contains(fileScheme, Qt::CaseInsensitive)) {
		QFileInfo fi(songItem->song()->path(), songItem->text(FOLDER_COLUMN));
		if( !QDesktopServices::openUrl(QUrl(fi.filePath())) )
			addLogMsg(QString(tr("Could NOT open file: \"%1\".")).arg(fi.filePath()), QU::warning);
		else
			addLogMsg(QString(tr("File was opened successfully: \"%1\".")).arg(fi.filePath()), QU::information);
	}
}

void QUMainWindow::showLyrics(QUSongFile *song) {
	if(!song)
		return;

	QUTextDialog *dlg = new QUTextDialog(song, this, true);
	dlg->exec();
	delete dlg;
}

void QUMainWindow::toggleFilterFrame(bool checked) {
	filterArea->setVisible(checked);

	if(checked) {
		filterArea->filterEdit->setFocus();
		filterArea->filterEdit->selectAll();
	}
}

void QUMainWindow::toggleFilterNegateBtn() {
	if(filterArea->filterNegateBtn->text() == "negate") {
		filterArea->filterNegateBtn->setText("");
		filterArea->filterNegateBtn->setIcon(QIcon(":/marks/plus.png"));
	} else {
		filterArea->filterNegateBtn->setText("negate");
		filterArea->filterNegateBtn->setIcon(QIcon(":/marks/minus.png"));
	}
}

void QUMainWindow::applyFilter() {
	int modes = 0;

	if(filterArea->filterNegateBtn->text() == "negate")
		modes |= QU::negateFilter;

	if(filterArea->filterTypeCombo->currentIndex() == 0)
		modes |= QU::informationTags | QU::fileTags | QU::controlTags | QU::customTags;

	if(filterArea->filterTypeCombo->currentIndex() == 1)
		modes |= QU::informationTags;

	if(filterArea->filterTypeCombo->currentIndex() == 2)
		modes |= QU::fileTags;

	if(filterArea->filterTypeCombo->currentIndex() == 3)
		modes |= QU::controlTags;

	if(filterArea->filterTypeCombo->currentIndex() == 4)
		modes |= QU::customTags;

	songTree->filterItems(filterArea->filterEdit->text(), (QU::FilterModes) modes);
}

void QUMainWindow::hideFilterArea() {
	actionFilter->setChecked(false);
}

/*!
 * Shows all items in the song tree again.
 */
void QUMainWindow::removeFilter() {
	filterArea->filterEdit->setText("");
	songTree->filterItems("");
}

void QUMainWindow::reportCreate() {
	QUReportDialog *dlg = new QUReportDialog(_songs, songTree->visibleSongs(), playlistArea->playlists(), this);

	connect(dlg, SIGNAL(finished(const QString&, QU::EventMessageTypes)), this, SLOT(addLogMsg(const QString&, QU::EventMessageTypes)));
	dlg->exec();
	disconnect(dlg, SIGNAL(finished(const QString&, QU::EventMessageTypes)), this, SLOT(addLogMsg(const QString&, QU::EventMessageTypes)));

	delete dlg;
}

/*!
 * Changes the application language to english.
 */
void QUMainWindow::enableEnglish() {
	actionLangGerman->setChecked(false);
	actionLangEnglish->setChecked(true);

	QSettings settings;
	settings.setValue("language", QVariant("en_EN"));

	// ---------------

	QUMessageBox::Results result = QUMessageBox::ask(this,
			tr("Change Language"),
			tr("Application language changed to <b>English</b>. You need to restart UltraStar Manager to take effect."),
			":/control/quit.png", tr("Quit UltraStar Manager."),
			":/marks/accept.png", tr("Continue."));
	if(result == QUMessageBox::first)
		this->close();
}

/*!
 * Changes the application language to german.
 */
void QUMainWindow::enableGerman() {
	actionLangGerman->setChecked(true);
	actionLangEnglish->setChecked(false);

	QSettings settings;
	settings.setValue("language", QVariant("de_DE"));

	// ---------------

	QUMessageBox::Results result = QUMessageBox::ask(this,
			tr("Change Language"),
			tr("Application language changed to <b>German</b>. You need to restart UltraStar Manager to take effect."),
			":/control/quit.png", tr("Quit UltraStar Manager."),
			":/marks/accept.png", tr("Continue."));
	if(result == QUMessageBox::first)
		this->close();
}

void QUMainWindow::getCoversFromAmazon(QList<QUSongItem*> items) {
	QUAmazonDialog *dlg = new QUAmazonDialog(items, this);
	connect(dlg, SIGNAL(finished(const QString&, QU::EventMessageTypes)), this, SLOT(addLogMsg(const QString&, QU::EventMessageTypes)));

	if(dlg->exec()) {
		updateDetails();
		montyTalk();
	}

	disconnect(dlg, 0, 0, 0);
	delete dlg;
}

/*!
 * Do everything to get back to a consistent state with the file system.
 */
void QUMainWindow::processExternalSongFileChange(QUSongFile *song) {
	if(song->hasUnsavedChanges()) {
		addLogMsg(QString("INCONSISTENT STATE! The song \"%1 - %2\" has unsaved changes and its persistent song file \"%3\" was modified externally. Save your changes or rebuild the tree manually.").arg(song->artist()).arg(song->title()).arg(song->songFileInfo().filePath()), QU::warning);
		return;
	}

	foreach(QUSongItem *songItem, songTree->allSongItems()) {
		if(songItem->song() == song) {
			song->updateCache();
			songItem->update();

			updateDetails();
			break;
		}
	}

	addLogMsg(QString("Song file changed: \"%1\"").arg(song->songFileInfo().filePath()), QU::information);
}
