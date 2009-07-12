#include "main.h"

#include "QUMainWindow.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include <QListWidget>
#include <QListWidgetItem>

#include <QDir>
#include <QStringList>
#include <QPixmap>
#include <QIcon>
#include <QImageReader>
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
#include "QULogService.h"

#include "QUDropDownDelegate.h"
#include "QUDetailsTable.h"

#include "fileref.h"
#include "tag.h"
#include "tstring.h"

#include "bass.h"

#include "QUTagOrderDialog.h"
#include "QUTextDialog.h"
#include "QUProgressDialog.h"
#include "QUReportDialog.h"
#include "QUMessageBox.h"
#include "QUPictureDialog.h"
#include "QUCustomTagsDialog.h"
#include "QUAmazonDialog.h"
#include "QUAboutDialog.h"
#include "QUPluginDialog.h"
#include "QUSlideShowDialog.h"
#include "QULyricsEditorDialog.h"
#include "QUPathsDialog.h"

#include "QUSongSupport.h"

#include "QUTaskFactoryProxy.h"

#include "QURibbonBar.h"

QUMainWindow::QUMainWindow(QWidget *parent): QMainWindow(parent) {
	setupUi(this);

	initWindow();
//	initMenu();
	initRibbonBar();
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
	int result;

	mediaplayer->stop();

	if(songTree->hasUnsavedChanges()) {
		result = QUMessageBox::information(this,
				tr("Quit"),
				tr("<b>Songs</b> have been modified."),
				BTN << ":/control/save_all.png" << tr("Save all changed songs.")
				    << ":/control/bin.png"      << tr("Discard all changes.")
				    << ":/marks/cancel.png"     << tr("Cancel this action."));
		if(result == 0)
			songTree->saveUnsavedChanges();
		else if(result == 2) {
			event->ignore();
			return;
		}
	}

	if(playlistArea->hasUnsavedChanges()) {
		result = QUMessageBox::information(this,
				tr("Quit"),
				tr("<b>Playlists</b> have been modified."),
				BTN << ":/control/save_all.png" << tr("Save all changed playlists.")
				    << ":/control/bin.png"      << tr("Discard all changes.")
				    << ":/marks/cancel.png"     << tr("Cancel this action."));
		if(result == 0)
			playlistArea->saveUnsavedChanges();
		else if(result == 2) {
			event->ignore();
			return;
		}
	}

	QSettings settings;
	settings.setValue("allowMonty", QVariant(_menu->montyBtn->isChecked()));
	settings.setValue("windowState", QVariant(this->saveState()));

	settings.setValue("disableInfoMessages", QVariant(_noInfos->isChecked()));
	settings.setValue("disableWarningMessages", QVariant(_noWarnings->isChecked()));
	settings.setValue("disableSaveMessages", QVariant(_noSaveHints->isChecked()));

	settings.setValue("autoSave", QVariant(_menu->autoSaveBtn->isChecked()));

	this->saveLog();

	// everything sould be fine from now on
	QFile::remove("running.app");

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
		QStringList paths = settings.value("songPaths", QStringList()).toStringList();
		if(!paths.isEmpty()) { // choose the first in the list of song paths
			path = paths.first();
			settings.setValue("songPath", path);
		} else { // request a list of song paths
			QUPathsDialog(true, this).exec();
			paths = settings.value("songPaths", QStringList()).toStringList();
			if(!paths.isEmpty()) {
				path = paths.first();
				settings.setValue("songPath", path);
			}
		}
	} else { // last song path was saved successfully
		path = settings.value("songPath").toString();
	}
	QU::BaseDir.setPath(path);

	// read other settings
			_menu->montyBtn->setChecked(settings.value("allowMonty", true).toBool());
	_menu->relativePathsChk->setChecked(settings.value("showRelativeSongPath", true).toBool());
	 _menu->otherSymbolsChk->setChecked(settings.value("altSongTree", false).toBool());
			   completerChk->setChecked(settings.value("caseSensitiveAutoCompletion", false).toBool());

	this->restoreState(settings.value("windowState").toByteArray());

	_menu->autoSaveBtn->setChecked(settings.value("autoSave", true).toBool());
	_menu->onTopChk->setChecked(settings.value("alwaysOnTop", false).toBool());

	_menu->detailsBtn->setChecked(detailsDock->isVisible());
	_menu->tasksBtn->setChecked(tasksDock->isVisible());
	_menu->playlistsBtn->setChecked(playlistDock->isVisible());
	_menu->playerBtn->setChecked(mediaPlayerDock->isVisible());
	_menu->fileInfoBtn->setChecked(previewDock->isVisible());
	_menu->eventLogBtn->setChecked(eventsDock->isVisible());

	if(QString::compare(settings.value("language").toString(), "en_EN") == 0) {
		_menu->langUsBtn->setChecked(true);
	} else if(QString::compare(settings.value("language").toString(), "de_DE") == 0) {
		_menu->langDeBtn->setChecked(true);
	} else if(QString::compare(settings.value("language").toString(), "pl_PL") == 0) {
		_menu->langPlBtn->setChecked(true);
	}

	QStringList imageFormatsNeeded;
	QStringList imageFormatsQt;
	QStringList imageFormatsUman = QUSongSupport::allowedPictureFiles().join(" ").remove("*.").split(" ");
	foreach(QByteArray ba, QImageReader::supportedImageFormats())
		imageFormatsQt << ba;
	foreach(QString neededFormat, imageFormatsUman)
		if(imageFormatsQt.indexOf(neededFormat) == -1)
			imageFormatsNeeded << neededFormat;
	if(imageFormatsNeeded.size() > 0)
		logSrv->add(QString(tr("Cannot read image formats: %1")).arg(imageFormatsNeeded.join(", ")), QU::warning);
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
	addDockWidget(Qt::RightDockWidgetArea, mediaPlayerDock); mediaPlayerDock->hide();

	connect(logSrv, SIGNAL(messageAdded(const QString&, QU::EventMessageTypes)), this, SLOT(addLogMsg(const QString&, QU::EventMessageTypes)));

	// init filter area
	filterArea->hide();

	connect(filterArea->filterEdit, SIGNAL(returnPressed()), this, SLOT(applyFilter()));
	connect(filterArea->filterClearBtn, SIGNAL(clicked()), this, SLOT(removeFilter()));
	connect(filterArea->filterDuplicatesBtn, SIGNAL(clicked()), songTree, SLOT(filterDuplicates()));

	// init media player connections
	connect(mediaplayer, SIGNAL(selectedSongsRequested()), this, SLOT(sendSelectedSongsToMediaPlayer()));
	connect(mediaplayer, SIGNAL(allSongsRequested()), this, SLOT(sendAllSongsToMediaPlayer()));
	connect(mediaplayer, SIGNAL(visibleSongsRequested()), this, SLOT(sendVisibleSongsToMediaPlayer()));
	connect(mediaplayer, SIGNAL(currentPlaylistRequested()), this, SLOT(sendCurrentPlaylistToMediaPlayer()));
}

//void QUMainWindow::initMenu() {
//	// songs menu
//	connect(        actionRefresh, SIGNAL(triggered()), this, SLOT(refreshAllSongs()));
//
//	connect(   actionSaveSelected, SIGNAL(triggered()), songTree, SLOT(saveSelectedSongs()));
//	connect(        actionSaveAll, SIGNAL(triggered()), songTree, SLOT(saveUnsavedChanges()));
//	connect( actionDeleteSelected, SIGNAL(triggered()), songTree, SLOT(requestDeleteSelectedSongs()));
//	connect(  actionMergeSelected, SIGNAL(triggered()), songTree, SLOT(mergeSelectedSongs()));
//	connect(      actionExpandAll, SIGNAL(triggered()), songTree, SLOT(expandAll()));
//	connect(      actionExpandAll, SIGNAL(triggered()), songTree, SLOT(resizeToContents()));
//	connect(    actionCollapseAll, SIGNAL(triggered()), songTree, SLOT(collapseAll()));
//	connect(    actionCollapseAll, SIGNAL(triggered()), songTree, SLOT(resizeToContents()));
////	connect(actionRefreshSelected, SIGNAL(triggered()), songTree, SLOT(refreshSelectedItems()));
//
//	connect( actionSendToPlaylist, SIGNAL(triggered()), songTree, SLOT(sendSelectedSongsToPlaylist()));
//	connect(      actionGetCovers, SIGNAL(triggered()), songTree, SLOT(requestCoversFromAmazon()));
//	connect(     actionShowLyrics, SIGNAL(triggered()), songTree, SLOT(requestLyrics()));
//
////	actionRefreshSelected->setShortcut(Qt::Key_F5);
//	        actionRefresh->setShortcut(Qt::SHIFT + Qt::Key_F5);
//	   actionSaveSelected->setShortcut(Qt::CTRL  + Qt::Key_S);
//	        actionSaveAll->setShortcut(Qt::CTRL  + Qt::SHIFT + Qt::Key_S);
//	 actionSendToPlaylist->setShortcut(Qt::CTRL  + Qt::Key_P);
//	     actionShowLyrics->setShortcut(Qt::CTRL  + Qt::Key_L);
//	 actionDeleteSelected->setShortcut(Qt::SHIFT + Qt::Key_Delete);
//	  actionMergeSelected->setShortcut(Qt::CTRL  + Qt::Key_M);
//
//	// view menu
//	connect(actionShowRelativeSongPath, SIGNAL(toggled(bool)), this, SLOT(toggleRelativeSongPath(bool)));
//	connect(actionAltSongTree, SIGNAL(toggled(bool)), this, SLOT(toggleAltSongTreeChk(bool)));
//	connect(actionFilter, SIGNAL(toggled(bool)), this, SLOT(toggleFilterFrame(bool)));
//
//	detailsDock->toggleViewAction()->setIcon(QIcon(":/control/text_edit.png"));
//	tasksDock->toggleViewAction()->setIcon(QIcon(":/control/tasks.png"));
//	previewDock->toggleViewAction()->setIcon(QIcon(":/control/file_info.png"));
//	playlistDock->toggleViewAction()->setIcon(QIcon(":/control/playlist.png"));
//	eventsDock->toggleViewAction()->setIcon(QIcon(":/control/log.png"));
//	mediaPlayerDock->toggleViewAction()->setIcon(QIcon(":/control/mediaplayer.png"));
//
//	this->viewBar->addAction(detailsDock->toggleViewAction());
//	this->viewBar->addAction(tasksDock->toggleViewAction());
//	this->viewBar->addAction(playlistDock->toggleViewAction());
//	this->viewBar->addAction(previewDock->toggleViewAction());
//	this->viewBar->addAction(eventsDock->toggleViewAction());
//	this->viewBar->addAction(mediaPlayerDock->toggleViewAction());
//
//	this->menuView->addAction(detailsDock->toggleViewAction());
//	this->menuView->addAction(tasksDock->toggleViewAction());
//	this->menuView->addAction(playlistDock->toggleViewAction());
//	this->menuView->addAction(previewDock->toggleViewAction());
//	this->menuView->addAction(eventsDock->toggleViewAction());
//	this->menuView->addAction(mediaPlayerDock->toggleViewAction());
//
//	// insert toggle view actions for the toolbars
//	this->menuView->addSeparator();
//
//	QMenu *showToolbarMenu = this->menuView->addMenu(tr("Toolbars"));
//
//	showToolbarMenu->addAction(this->songsBar->toggleViewAction());
//	showToolbarMenu->addAction(this->viewBar->toggleViewAction());
//	showToolbarMenu->addAction(this->optionsBar->toggleViewAction());
//
//	actionFilter->setShortcut(Qt::CTRL + Qt::Key_F);
//
//	// options menu
//	connect(actionAutoSave, SIGNAL(toggled(bool)), this, SLOT(toggleAutoSaveChk(bool)));
//	connect(actionAlwaysOnTop, SIGNAL(toggled(bool)), this, SLOT(toggleAlwaysOnTop(bool)));
//
//	connect(actionTagSaveOrder, SIGNAL(triggered()), this, SLOT(editTagOrder()));
//	connect(actionChangeSongDirectory, SIGNAL(triggered()), this, SLOT(changeSongDir()));
//	connect(actionCustomTags, SIGNAL(triggered()), this, SLOT(editCustomTags()));
//	connect(actionPaths, SIGNAL(triggered()), this, SLOT(showPathsDialog()));
//
//	connect(actionLangEnglish, SIGNAL(triggered()), this, SLOT(enableEnglish()));
//	connect(actionLangGerman, SIGNAL(triggered()), this, SLOT(enableGerman()));
//	connect(actionLangPolish, SIGNAL(triggered()), this, SLOT(enablePolish()));
//
//	actionChangeSongDirectory->setShortcut(Qt::Key_F12);
//
//	// extras menu
//	connect(actionNewReport, SIGNAL(triggered()), this, SLOT(reportCreate()));
//	connect(actionBackupAudioFiles, SIGNAL(triggered()), this, SLOT(copyAudioToPath()));
//	connect(actionPlugins, SIGNAL(triggered()), this, SLOT(showPluginDialog()));
//
//	actionNewReport->setShortcut(Qt::Key_F8);
//
//	// help menu
//	connect(actionShowMonty, SIGNAL(triggered()), montyArea, SLOT(show()));
//	connect(actionQt, SIGNAL(triggered()), this, SLOT(aboutQt()));
//	connect(actionUman, SIGNAL(triggered()), this, SLOT(aboutUman()));
//	connect(actionTagLib, SIGNAL(triggered()), this, SLOT(aboutTagLib()));
//}

/*!
 * Create an Office2007-like menu to reduce mouse clicks.
 */
void QUMainWindow::initRibbonBar() {
	this->songsBar->hide();
	this->viewBar->hide();
	this->optionsBar->hide();

	_menu = new QURibbonBar(this);
	this->setMenuWidget(_menu);

	// songs menu
	connect(_menu->saveBtn, SIGNAL(clicked()), songTree, SLOT(saveSelectedSongs()));
	connect(_menu->deleteBtn, SIGNAL(clicked()), songTree, SLOT(requestDeleteSelectedSongs()));
	connect(_menu->mergeBtn, SIGNAL(clicked()), songTree, SLOT(mergeSelectedSongs()));

	connect(_menu->getCoversBtn, SIGNAL(clicked()), songTree, SLOT(requestCoversFromAmazon()));

	QMenu *pictureFlowMenu = new QMenu(tr("Review pictures"));
	pictureFlowMenu->addAction(QIcon(":/types/cover.png"),      tr("Covers..."),      songTree, SLOT(requestCoverFlow()));
	pictureFlowMenu->addAction(QIcon(":/types/background.png"), tr("Backgrounds..."), songTree, SLOT(requestBackgroundFlow()));
	_menu->reviewPicturesBtn->setMenu(pictureFlowMenu);

	connect(_menu->sendToPlaylistBtn, SIGNAL(clicked()), songTree, SLOT(sendSelectedSongsToPlaylist()));
	connect(_menu->calcSongSpeed, SIGNAL(clicked()), songTree, SLOT(calculateSpeed()));

	connect(_menu->showLyricsBtn, SIGNAL(clicked()), songTree, SLOT(requestLyrics()));
	connect(_menu->editLyricsBtn, SIGNAL(clicked()), songTree, SLOT(requestEditLyrics()));

	connect(_menu->openExpBtn, SIGNAL(clicked()), songTree, SLOT(openCurrentFolder()));
	connect(_menu->moreArtistBtn, SIGNAL(clicked()), songTree, SLOT(showMoreCurrentArtist()));

	_menu->setShortcut(_menu->saveBtn, Qt::CTRL + Qt::Key_S);
	_menu->setShortcut(_menu->sendToPlaylistBtn, Qt::CTRL  + Qt::Key_P);
	_menu->setShortcut(_menu->showLyricsBtn, Qt::CTRL  + Qt::Key_L);
	_menu->setShortcut(_menu->deleteBtn, Qt::SHIFT + Qt::Key_Delete);
	_menu->setShortcut(_menu->mergeBtn, Qt::CTRL  + Qt::Key_M);

	// view menu
	connect(_menu->relativePathsChk, SIGNAL(toggled(bool)), this, SLOT(toggleRelativeSongPath(bool)));
	connect(_menu->otherSymbolsChk, SIGNAL(toggled(bool)), this, SLOT(toggleAltSongTreeChk(bool)));
	connect(_menu->findSongsBtn, SIGNAL(toggled(bool)), this, SLOT(toggleFilterFrame(bool)));

	connect(_menu->detailsBtn, SIGNAL(clicked(bool)), detailsDock, SLOT(setVisible(bool)));
	connect(_menu->tasksBtn, SIGNAL(clicked(bool)), tasksDock, SLOT(setVisible(bool)));
	connect(_menu->playlistsBtn, SIGNAL(clicked(bool)), playlistDock, SLOT(setVisible(bool)));
	connect(_menu->playerBtn, SIGNAL(clicked(bool)), mediaPlayerDock, SLOT(setVisible(bool)));
	connect(_menu->fileInfoBtn, SIGNAL(clicked(bool)), previewDock, SLOT(setVisible(bool)));
	connect(_menu->eventLogBtn, SIGNAL(clicked(bool)), eventsDock, SLOT(setVisible(bool)));

	_menu->setShortcut(_menu->findSongsBtn, Qt::CTRL + Qt::Key_F);

	// options menu
	connect(_menu->autoSaveBtn, SIGNAL(toggled(bool)), this, SLOT(toggleAutoSaveChk(bool)));
	connect(_menu->onTopChk, SIGNAL(toggled(bool)), this, SLOT(toggleAlwaysOnTop(bool)));

	connect(_menu->tagSaveOrder, SIGNAL(clicked()), this, SLOT(editTagOrder()));
	connect(_menu->customTagsBtn, SIGNAL(clicked()), this, SLOT(editCustomTags()));
	connect(_menu->pathsBtn, SIGNAL(clicked()), this, SLOT(showPathsDialog()));

	connect(_menu->langUsBtn, SIGNAL(clicked()), this, SLOT(enableEnglish()));
	connect(_menu->langDeBtn, SIGNAL(clicked()), this, SLOT(enableGerman()));
	connect(_menu->langPlBtn, SIGNAL(clicked()), this, SLOT(enablePolish()));

	// extras menu
	connect(_menu->reportBtn, SIGNAL(clicked()), this, SLOT(reportCreate()));
	connect(_menu->backupAudioBtn, SIGNAL(clicked()), this, SLOT(copyAudioToPath()));
	connect(_menu->pluginsBtn, SIGNAL(clicked()), this, SLOT(showPluginDialog()));

	_menu->setShortcut(_menu->reportBtn, Qt::Key_F8);

	QMenu *hideSongsMenu = new QMenu(tr("Hide Songs"));
	hideSongsMenu->addAction(tr("Selection"), songTree, SLOT(hideSelected()));
	hideSongsMenu->addAction(tr("Selection Only"), songTree, SLOT(hideSelectedOnly()));
	hideSongsMenu->addAction(tr("Inverted Selection"), songTree, SLOT(hideAllButSelected()));
	hideSongsMenu->addSeparator();
	hideSongsMenu->addAction(tr("All"), songTree, SLOT(hideAll()));
	_menu->hideSongsBtn->setMenu(hideSongsMenu);

	connect(_menu->saveAllBtn, SIGNAL(clicked()), songTree, SLOT(saveUnsavedChanges()));
	connect(_menu->rescanSongsBtn, SIGNAL(clicked()), this, SLOT(refreshAllSongs()));
	connect(_menu->expandAllBtn, SIGNAL(clicked()), songTree, SLOT(expandAll()));
	connect(_menu->expandAllBtn, SIGNAL(clicked()), songTree, SLOT(resizeToContents()));
	connect(_menu->collapseAllBtn, SIGNAL(clicked()), songTree, SLOT(collapseAll()));
	connect(_menu->collapseAllBtn, SIGNAL(clicked()), songTree, SLOT(resizeToContents()));

	_menu->setShortcut(_menu->saveAllBtn, Qt::CTRL + Qt::SHIFT + Qt::Key_S);
	_menu->setShortcut(_menu->rescanSongsBtn, Qt::SHIFT + Qt::Key_F5);

	// about menu
	connect(_menu->aboutQtBtn, SIGNAL(clicked()), this, SLOT(aboutQt()));
	connect(_menu->aboutUmanBtn, SIGNAL(clicked()), this, SLOT(aboutUman()));
	connect(_menu->aboutTagLibBtn, SIGNAL(clicked()), this, SLOT(aboutTagLib()));
	connect(_menu->aboutBASSBtn, SIGNAL(clicked()), this, SLOT(aboutBASS()));

	// help menu
	connect(_menu->helpBtn, SIGNAL(clicked()), montyArea, SLOT(show()));
	_menu->setShortcut(_menu->helpBtn, Qt::Key_F1);
	_menu->setShortcut(_menu->montyBtn, Qt::CTRL + Qt::Key_F1);
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
	connect(songTree, SIGNAL(itemSelectionChanged()), this, SLOT(updateMergeBtn()));

	connect(songTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(editSongSetFileLink(QTreeWidgetItem*, int)));
	connect(songTree, SIGNAL(itemActivated(QTreeWidgetItem*, int)), this, SLOT(showFileContent(QTreeWidgetItem*, int)));

	connect(songTree, SIGNAL(itemExpanded(QTreeWidgetItem*)), songTree, SLOT(resizeToContents()));
	connect(songTree, SIGNAL(itemCollapsed(QTreeWidgetItem*)), songTree, SLOT(resizeToContents()));

	connect(songTree, SIGNAL(songCreated(QUSongFile*)), this, SLOT(appendSong(QUSongFile*)));

	connect(songTree, SIGNAL(songToPlaylistRequested(QUSongFile*)), playlistArea, SLOT(addSongToCurrentPlaylist(QUSongFile*)));
	connect(songTree, SIGNAL(coversFromAmazonRequested(QList<QUSongItem*>)), this, SLOT(getCoversFromAmazon(QList<QUSongItem*>)));
	connect(songTree, SIGNAL(coverFlowRequested(QList<QUSongItem*>)), this, SLOT(showCoverSlideShowDialog(QList<QUSongItem*>)));
	connect(songTree, SIGNAL(backgroundFlowRequested(QList<QUSongItem*>)), this, SLOT(showBackgroundSlideShowDialog(QList<QUSongItem*>)));
	connect(songTree, SIGNAL(deleteSongRequested(QUSongFile*)), this, SLOT(deleteSong(QUSongFile*)));

	connect(songTree, SIGNAL(showLyricsRequested(QUSongFile*)), this, SLOT(showLyrics(QUSongFile*)));
	connect(songTree, SIGNAL(editLyricsRequested(QUSongFile*)), this, SLOT(editSongLyrics(QUSongFile*)));

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
	taskList->reloadAllPlugins();

	addTaskBtn->setMenu(new QMenu);
	addTaskBtn->setPopupMode(QToolButton::InstantPopup);

	foreach(QUTaskFactoryProxy *fp, taskList->factoryProxies()) {
		if(fp->factory()->canAddConfigurations())
			addTaskBtn->menu()->addAction(QString("%1...").arg(fp->factory()->productName()), fp, SLOT(addConfiguration()));
	}

	// connect task buttons
	connect(taskBtn, SIGNAL(clicked()), this, SLOT(editSongApplyTasks()));
	connect(allTasksBtn, SIGNAL(clicked()), taskList, SLOT(checkAllTasks()));
	connect(noTasksBtn, SIGNAL(clicked()), taskList, SLOT(uncheckAllTasks()));

	QButtonGroup *grp = new QButtonGroup(this);
	grp->addButton(tasksSlot1Btn); grp->setId(tasksSlot1Btn, 0);
	grp->addButton(tasksSlot2Btn); grp->setId(tasksSlot2Btn, 1);
	grp->addButton(tasksSlot3Btn); grp->setId(tasksSlot3Btn, 2);
	grp->addButton(tasksSlot4Btn); grp->setId(tasksSlot4Btn, 3);
	grp->setExclusive(true);

	connect(grp, SIGNAL(buttonClicked(int)), taskList, SLOT(setCurrentSlot(int)));
	connect(taskList, SIGNAL(toolTipChanged(int, const QString&)), this, SLOT(updateTasksSlotButtons(int, const QString&)));

	// fetch all tooltips
	taskList->setCurrentSlot(3);
	taskList->setCurrentSlot(2);
	taskList->setCurrentSlot(1);
	taskList->setCurrentSlot(0);

	tasksSlot1Btn->click();
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
	montyArea->montyLbl->setPixmap(monty->pic(QUMonty::seated));
	montyArea->helpLbl->setText(monty->welcomeMsg(_songs.size()));

	connect(montyArea->hideMontyBtn, SIGNAL(clicked()), montyArea, SLOT(hide()));
	connect(montyArea->talkMontyBtn, SIGNAL(clicked()), this, SLOT(montyTalkNow()));

	if(!_menu->montyBtn->isChecked())
		montyArea->hide();

	montyArea->askFrame->hide();
	montyArea->answerFrame->hide();

	connect(montyArea->askMontyBtn, SIGNAL(clicked()), this, SLOT(montyAsk()));
	connect(montyArea->acceptQuestionBtn, SIGNAL(clicked()), this, SLOT(montyAnswer()));

	connect(montyArea->rejectQuestionBtn, SIGNAL(clicked()), montyArea->normalFrame, SLOT(show()));
	connect(montyArea->rejectQuestionBtn, SIGNAL(clicked()), montyArea->askFrame, SLOT(hide()));
	connect(montyArea->rejectQuestionBtn, SIGNAL(clicked()), this, SLOT(montyTalkNow()));

	connect(montyArea->doneBtn, SIGNAL(clicked()), montyArea->normalFrame, SLOT(show()));
	connect(montyArea->doneBtn, SIGNAL(clicked()), montyArea->answerFrame, SLOT(hide()));
	connect(montyArea->doneBtn, SIGNAL(clicked()), this, SLOT(montyTalkNow()));

	connect(montyArea->nextPhraseBtn, SIGNAL(clicked()), this, SLOT(montyNext()));
	connect(montyArea->prevPhraseBtn, SIGNAL(clicked()), this, SLOT(montyPrev()));

	connect(montyArea->lineEdit, SIGNAL(returnPressed()), this, SLOT(montyAnswer()));
}

void QUMainWindow::appendSong(QUSongFile *song) {
	_songs.append(song);

	// connect changes in song files with an update in the playlist area
	connect(song, SIGNAL(dataChanged()), playlistArea, SLOT(update()));
	// react to changes
	connect(song, SIGNAL(externalSongFileChangeDetected(QUSongFile*)), this, SLOT(processExternalSongFileChange(QUSongFile*)));

	//TODO: What about friends?
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
			logSrv->add(QString(tr("Could NOT delete file: \"%1\"")).arg(fi.filePath()), QU::warning);
		else
			logSrv->add(QString(tr("File was deleted successfully: \"%1\"")).arg(fi.filePath()), QU::information);
	}

	QString dirName = dir.dirName();
	dir.cdUp();

	if(!dir.rmdir(dirName))
		logSrv->add(QString(tr("Could NOT delete directory: \"%1\". Maybe it is not empty.")).arg(song->songFileInfo().path()), QU::warning);
	else
		logSrv->add(QString(tr("Directory was deleted successfully: \"%1\"")).arg(song->songFileInfo().path()), QU::information);

	_songs.removeAll(song);
	delete song;

	logSrv->add(QString(tr("Song was deleted successfully: \"%1 - %2\"")).arg(artist).arg(title), QU::information);
}

/*!
 * Re-reads all possible song files and builds a new song tree.
 */
void QUMainWindow::refreshAllSongs(bool force) {
	if(!force && songTree->hasUnsavedChanges()) {
		int result = QUMessageBox::information(this,
				tr("Rebuild Song Tree"),
				tr("Songs have been modified."),
				BTN << ":/control/save_all.png" << tr("Save all changes.")
				    << ":/control/bin.png"      << tr("Discard all changes.")
				    << ":/marks/cancel.png"     << tr("Cancel this action."));
		if(result == 2)
			return;
		else if(result == 0)
			songTree->saveUnsavedChanges();
	}

	// -------------------------------------

	playlistArea->disconnectPlaylists();

	songTree->clear();
	updateDetails();

	qDeleteAll(_songs);
	_songs.clear();

	createSongFiles();
	monty->setSongCount(_songs.size()); // sometimes, Monty talks about your song count...

	updatePreviewTree();
	playlistArea->updateAll();
}

/*!
 * Creates all instances of QUSongFile to fill the song tree. Fills the song tree.
 * \sa CreateSongTree();
 */
void QUMainWindow::createSongFiles() {
	QList<QDir> dirList;
	dirList.append(QDir(QU::BaseDir));

	// create a list of all sub-directories
	this->readSongDir(dirList);
	// initialize the header of the song tree
	songTree->initHorizontalHeader();

	QUProgressDialog dlg(tr("Reading song files..."), dirList.size(), this);
	dlg.setPixmap(":/types/folder.png");
	dlg.show();

	foreach(QDir dir, dirList) {
		QFileInfoList songFiList = dir.entryInfoList(QUSongSupport::allowedSongFiles(), QDir::Files, QDir::Name);
		qStableSort(songFiList.begin(), songFiList.end(), QU::fileTypeLessThan);

		dlg.update(dir.dirName());
		if(dlg.cancelled()) break;

		QUSongFile *newSong = 0;

		foreach(QFileInfo fi, songFiList) {
			if( QUSongSupport::allowedLicenseFiles().contains(fi.fileName(), Qt::CaseInsensitive) )
				continue; // skip license files (txt)

			if(!newSong)
				newSong = new QUSongFile(fi.filePath());
			else { // found a friend!
				QUSongFile *friendSong = new QUSongFile(fi.filePath());
				newSong->addFriend(friendSong);
				// see "this->appendSong(...)"
				connect(friendSong, SIGNAL(dataChanged()), playlistArea, SLOT(update()));
				connect(friendSong, SIGNAL(externalSongFileChangeDetected(QUSongFile*)), this, SLOT(processExternalSongFileChange(QUSongFile*)));
			}
		}

		if(newSong) { // some song found
			this->appendSong(newSong);
			songTree->addTopLevelItem(new QUSongItem(newSong, true));
		}
	}

	// finish song tree initialization
	songTree->resizeToContents();
	songTree->sortItems(FOLDER_COLUMN, Qt::AscendingOrder);
}

/*!
 * Reads recursively all available directories and puts them in the dirList.
 */
void QUMainWindow::readSongDir(QList<QDir> &dirList) {
	if(dirList.isEmpty())
		return;

	if(dirList.last().entryList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::NoSymLinks, QDir::Name).isEmpty()) {
		return;
	} else {
		QDir thisDir(dirList.last());
		QStringList subDirs = dirList.last().entryList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::NoSymLinks, QDir::Name);

		foreach(QString dir, subDirs) {
			QDir newDir(thisDir);
			newDir.cd(dir);

			dirList.append(newDir);
			this->readSongDir(dirList);
		}
	}
}

/*!
 * Update the details of the selected songs. Disconnect to avoid dead-lock.
 */
void QUMainWindow::updateDetails() {
	disconnect(detailsTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(editSongSetDetail(QTableWidgetItem*)));
	detailsTable->updateValueColumn(songTree->selectedSongItems());
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

void QUMainWindow::updateMergeBtn() {
	_menu->mergeBtn->setEnabled(songTree->selectedItems().size() > 1);
}

/*!
 * Change the tooltip of the tasks-slot-button with the given index.
 */
void QUMainWindow::updateTasksSlotButtons(int index, const QString& tooltip) {
	switch(index) {
		case 0: tasksSlot1Btn->setToolTip(tooltip); break;
		case 1: tasksSlot2Btn->setToolTip(tooltip); break;
		case 2: tasksSlot3Btn->setToolTip(tooltip); break;
		case 3: tasksSlot4Btn->setToolTip(tooltip); break;
	}
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
			and QUSongSupport::allowedAudioFiles().contains(fileScheme, Qt::CaseInsensitive)
			and column == MP3_COLUMN ) {
		logSrv->add(QString(tr("Audio file changed from \"%1\" to: \"%2\".")).arg(song->mp3()).arg(songItem->text(FOLDER_COLUMN)), QU::information);
		song->setInfo(MP3_TAG, songItem->text(FOLDER_COLUMN));
		song->save();
	} else if( songItem->icon(COVER_COLUMN).isNull()
			and QUSongSupport::allowedPictureFiles().contains(fileScheme, Qt::CaseInsensitive)
			and column == COVER_COLUMN ) {
		logSrv->add(QString(tr("Cover changed from \"%1\" to: \"%2\".")).arg(song->cover()).arg(songItem->text(FOLDER_COLUMN)), QU::information);
		song->setInfo(COVER_TAG, songItem->text(FOLDER_COLUMN));
		song->save();
	} else if( songItem->icon(BACKGROUND_COLUMN).isNull()
			and QUSongSupport::allowedPictureFiles().contains(fileScheme, Qt::CaseInsensitive)
			and column == BACKGROUND_COLUMN ) {
		logSrv->add(QString(tr("Background changed from \"%1\" to: \"%2\".")).arg(song->background()).arg(songItem->text(FOLDER_COLUMN)), QU::information);
		song->setInfo(BACKGROUND_TAG, songItem->text(FOLDER_COLUMN));
		song->save();
	} else if( songItem->icon(VIDEO_COLUMN).isNull()
			and QUSongSupport::allowedVideoFiles().contains(fileScheme, Qt::CaseInsensitive)
			and column == VIDEO_COLUMN ) {
		logSrv->add(QString(tr("Video file changed from \"%1\" to: \"%2\".")).arg(song->video()).arg(songItem->text(FOLDER_COLUMN)), QU::information);
		song->setInfo(VIDEO_TAG, songItem->text(FOLDER_COLUMN));
		song->save();
	}

	if(songItem->parent()) {
		QUSongItem *parentItem = dynamic_cast<QUSongItem*>(songItem->parent());
		if(parentItem) {
			parentItem->update(); // songItem becomes invalid after this; see QUSongItem::update()
			songTree->setCurrentItem(parentItem);
		}
	}

	updateDetails();
	montyTalk();
}


void QUMainWindow::editSongSetDetail(QTableWidgetItem *item) {
	QUDetailItem *detailItem = dynamic_cast<QUDetailItem*>(detailsTable->currentItem());

	if(!detailItem)
		return;

	QUProgressDialog dlg(QString(tr("Applying new value for %1 to all selected songs...")).arg(detailItem->tag()),
			detailItem->songItems().size(), this);
	dlg.show();

	// capture contents, because they change if you change the selection
	QList<QUSongItem*> selectedItems = detailItem->songItems();
	QString tag = detailItem->tag();
	QString text = detailItem->text();

	songTree->clearSelection();

	// save changes for each song
	foreach(QUSongItem *songItem, selectedItems) {
		dlg.update(QString("%1 - %2").arg(songItem->song()->artist()).arg(songItem->song()->title()));
		if(dlg.cancelled()) break;

		songItem->song()->setInfo(tag, text);
		songItem->song()->save();

		songItem->update();
	}
	songTree->restoreSelection(selectedItems);
	songTree->scrollToItem(songTree->currentItem(), QAbstractItemView::EnsureVisible);
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

void QUMainWindow::editSongLyrics(QUSongFile *song) {
	if(!song)
		return;

	QULyricsEditorDialog dlg(song, this);

	if(dlg.exec())
		song->save();
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

		logSrv->add(QString(tr("The log file was saved to: \"%1\"")).arg(filePath), QU::saving);
	} else
		logSrv->add(QString(tr("The log file COULD NOT be saved.")).arg(filePath), QU::warning);
}

void QUMainWindow::clearLog() {
	log->clear();
}

void QUMainWindow::aboutQt() {
	QMessageBox::aboutQt(this, "About Qt");
}

void QUMainWindow::aboutUman() {
	QUAboutDialog(this).exec();
}

void QUMainWindow::aboutTagLib() {
	QUMessageBox::information(this,
			tr("About TagLib"),
			QString(tr("<b>TagLib Audio Meta-Data Library</b><br><br>"
					"TagLib is a library for reading and editing the meta-data of several popular audio formats.<br><br>"
					"Version: <b>%1.%2.%3</b><br><br>"
					"Visit: <a href=\"http://developer.kde.org/~wheeler/taglib.html\">TagLib Homepage</a>"))
					.arg(TAGLIB_MAJOR_VERSION)
					.arg(TAGLIB_MINOR_VERSION)
					.arg(TAGLIB_PATCH_VERSION));
}

void QUMainWindow::aboutBASS() {
	QUMessageBox::information(this,
			tr("About BASS"),
			QString(tr("<b>BASS Audio Library</b><br><br>"
					"BASS is an audio library for use in Windows and MacOSX software. Its purpose is to provide the most powerful and efficient (yet easy to use), sample, stream, MOD music, and recording functions. All in a tiny DLL, under 100KB in size.<br><br>"
					"Version: <b>%1</b><br><br>"
					"Copyright (c) 1999-2008<br><a href=\"http://www.un4seen.com/bass.html\">Un4seen Developments Ltd.</a> All rights reserved."))
					.arg(BASSVERSIONTEXT),
			QStringList() << ":/marks/accept.png" << "OK",
			330);
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
		int result = QUMessageBox::information(this,
				tr("Change Song Directory"),
				tr("Songs have been modified."),
				BTN << ":/control/save_all.png" << tr("Save all changes.")
				    << ":/control/bin.png"      << tr("Discard all changes.")
				    << ":/marks/cancel.png"     << tr("Cancel this action."));
		if(result == 2)
			return;
		else if(result == 0)
			songTree->saveUnsavedChanges();
	}

	// ---------------------------------------------------------

	QSettings settings;
	QString path = settings.value("songPath").toString();

	path = QFileDialog::getExistingDirectory(this, tr("Choose your UltraStar song directory:"), path);

	if(!path.isEmpty()) {
		settings.setValue("songPath", QVariant(path));
		QU::BaseDir.setPath(path);
		refreshAllSongs(true);

		montyTalk();

		logSrv->add(QString(tr("UltraStar song directory changed to: \"%1\".")).arg(QU::BaseDir.path()), QU::information);

		songTree->headerItem()->setText(FOLDER_COLUMN, QString(tr("Folder (%1)")).arg(QU::BaseDir.path()));
	}
}

/*!
 * Enables the user to setup custom tags. Modified songs need to be saved first, because
 * the whole song tree (database) will be rebuild. This is an easy way to update the (un-)supported tags
 * for each song. :)
 */
void QUMainWindow::editCustomTags() {
	if(songTree->hasUnsavedChanges()) {
		int result = QUMessageBox::information(this,
				tr("Custom Tags"),
				tr("Songs have been modified."),
				BTN << ":/control/save_all.png" << tr("Save all changes.")
				    << ":/control/bin.png"      << tr("Discard all changes.")
				    << ":/marks/cancel.png"     << tr("Cancel this action."));
		if(result == 2)
			return;
		else if(result == 0)
			songTree->saveUnsavedChanges();
	}

	// ---------------------------------------------------------

	QUCustomTagsDialog *dlg = new QUCustomTagsDialog(this);

	if(dlg->exec()) {
		this->refreshAllSongs(true);
		detailsTable->reset();

		logSrv->add(QString(tr("Custom tags changed to: \"%1\"")).arg(QUSongSupport::availableCustomTags().join(", ")), QU::information);
	}

	delete dlg;

	montyTalk();
}

void QUMainWindow::montyTalk(bool force) {
	if(!force and !_menu->montyBtn->isChecked())
		return;

	montyArea->show();
	monty->talk(montyArea->montyLbl, montyArea->helpLbl);
}

void QUMainWindow::montyTalkNow() {
	montyTalk(true);
}

void QUMainWindow::montyAsk() {
	montyArea->normalFrame->hide();
	montyArea->askFrame->show();

	montyArea->lineEdit->selectAll();
	montyArea->lineEdit->setFocus();

	montyArea->helpLbl->setText(tr("You can ask me something if you put some keywords in the <i>line edit</i> below and <i>accept</i>.<br><br>I'll try to understand and answer you. Multiple answers may be possible."));
}

void QUMainWindow::montyAnswer() {
	montyArea->askFrame->hide();
	montyArea->answerFrame->show();

	monty->answer(montyArea->montyLbl, montyArea->helpLbl, montyArea->lineEdit->text());
}

void QUMainWindow::montyPrev() {
	monty->answer(montyArea->montyLbl, montyArea->helpLbl, "", true);
}

void QUMainWindow::montyNext() {
	monty->answer(montyArea->montyLbl, montyArea->helpLbl);
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
		logSrv->add(tr("Relative song paths are displayed in the song tree now."), QU::information);
	else
		logSrv->add(tr("Only song directories are displayed in the song tree now."), QU::information);
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

	_menu->saveAllBtn->setEnabled(!checked);
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
 * Let the main window stay always in foreground.
 */
void QUMainWindow::toggleAlwaysOnTop(bool checked) {
	QSettings settings;
	if(settings.value("alwaysOnTop", false) != checked)
		settings.setValue("alwaysOnTop", checked);

	if(checked)
		this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
	else
		this->setWindowFlags(this->windowFlags() & !Qt::WindowStaysOnTopHint);

	this->show();
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
	} else if(QUSongSupport::allowedLicenseFiles().contains(songItem->text(FOLDER_COLUMN), Qt::CaseInsensitive)) {
		// show the contents of the license
		QUTextDialog *dlg = new QUTextDialog(QFileInfo(songItem->song()->songFileInfo().dir(), songItem->text(FOLDER_COLUMN)).filePath(), this);
		dlg->exec();
		delete dlg;
	} else if(songItem->song()->isFriend(item->text(FOLDER_COLUMN))) {
		// show (raw) content of friend song
		QUTextDialog dlg(songItem->song()->friendAt(item->text(FOLDER_COLUMN)), this);
		dlg.exec();
	} else if(QUSongSupport::allowedPictureFiles().contains(fileScheme, Qt::CaseInsensitive)) {
		QUPictureDialog dlg(QFileInfo(songItem->song()->songFileInfo().dir(), songItem->text(FOLDER_COLUMN)).filePath(), this);
		dlg.exec();
	} else if(QUSongSupport::allowedAudioFiles().contains(fileScheme, Qt::CaseInsensitive)
		or QUSongSupport::allowedVideoFiles().contains(fileScheme, Qt::CaseInsensitive)
		or QUSongSupport::allowedKaraokeFiles().contains(fileScheme, Qt::CaseInsensitive)
		or QUSongSupport::allowedMidiFiles().contains(fileScheme, Qt::CaseInsensitive)) {
		QFileInfo fi(songItem->song()->path(), songItem->text(FOLDER_COLUMN));
		if( !QDesktopServices::openUrl(QUrl(fi.filePath())) )
			logSrv->add(QString(tr("Could NOT open file: \"%1\".")).arg(fi.filePath()), QU::warning);
		else
			logSrv->add(QString(tr("File was opened successfully: \"%1\".")).arg(fi.filePath()), QU::information);
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

void QUMainWindow::applyFilter() {
	int modes = 0;

	if(filterArea->doInvertedSearch())
		modes |= QU::negateFilter;

	if(filterArea->tagGroupActions().at(0)->isChecked())
		modes |= QU::informationTags | QU::fileTags | QU::controlTags | QU::customTags;

	if(filterArea->tagGroupActions().at(1)->isChecked())
		modes |= QU::informationTags;

	if(filterArea->tagGroupActions().at(2)->isChecked())
		modes |= QU::fileTags;

	if(filterArea->tagGroupActions().at(3)->isChecked())
		modes |= QU::controlTags;

	if(filterArea->tagGroupActions().at(4)->isChecked())
		modes |= QU::customTags;

	filterArea->filterClearBtn->show();
	songTree->filterItems(filterArea->filterEdit->text(), (QU::FilterModes) modes);
}

/*!
 * Show all songs, if filter area is hidden.
 */
void QUMainWindow::hideFilterArea() {
	_menu->findSongsBtn->setChecked(false);
	removeFilter();
}

/*!
 * Shows all items in the song tree again.
 */
void QUMainWindow::removeFilter() {
	filterArea->filterEdit->setText("");
	songTree->filterItems("");
	filterArea->filterClearBtn->hide();
}

void QUMainWindow::reportCreate() {
	QUReportDialog *dlg = new QUReportDialog(_songs, songTree->visibleSongs(), playlistArea->playlists(), this);

	dlg->exec();

	delete dlg;
}

/*!
 * Changes the application language to english.
 */
void QUMainWindow::enableEnglish() {
	_menu->langUsBtn->setChecked(true);

	QSettings settings;
	settings.setValue("language", QLocale(QLocale::English, QLocale::UnitedStates).name());

	// ---------------

	int result = QUMessageBox::information(this,
			tr("Change Language"),
			tr("Application language changed to <b>English</b>. You need to restart UltraStar Manager to take effect."),
			BTN << ":/control/quit.png" << tr("Quit UltraStar Manager.")
			    << ":/marks/accept.png" << tr("Continue."));
	if(result == 0)
		this->close();
}

/*!
 * Changes the application language to german.
 */
void QUMainWindow::enableGerman() {
	_menu->langDeBtn->setChecked(true);

	QSettings settings;
	settings.setValue("language", QLocale(QLocale::German, QLocale::Germany).name());

	// ---------------

	int result = QUMessageBox::information(this,
			tr("Change Language"),
			tr("Application language changed to <b>German</b>. You need to restart UltraStar Manager to take effect."),
			BTN << ":/control/quit.png" << tr("Quit UltraStar Manager.")
			    << ":/marks/accept.png" << tr("Continue."));
	if(result == 0)
		this->close();
}

/*!
 * Changes the application language to polish.
 */
void QUMainWindow::enablePolish() {
	_menu->langPlBtn->setChecked(true);

	QSettings settings;
	settings.setValue("language", QLocale(QLocale::Polish, QLocale::Poland).name());

	// ---------------

	int result = QUMessageBox::information(this,
			tr("Change Language"),
			tr("Application language changed to <b>Polish</b>. You need to restart UltraStar Manager to take effect."),
			BTN << ":/control/quit.png" << tr("Quit UltraStar Manager.")
			    << ":/marks/accept.png" << tr("Continue."));
	if(result == 0)
		this->close();
}

void QUMainWindow::getCoversFromAmazon(QList<QUSongItem*> items) {
	QUAmazonDialog *dlg = new QUAmazonDialog(items, this);

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
		logSrv->add(QString("INCONSISTENT STATE! The song \"%1 - %2\" has unsaved changes and its persistent song file \"%3\" was modified externally. Save your changes or rebuild the tree manually.").arg(song->artist()).arg(song->title()).arg(song->songFileInfo().filePath()), QU::warning);
		return;
	}

	foreach(QUSongItem *songItem, songTree->allSongItems()) {
		if(songItem->song() == song or songItem->song()->isFriend(song)) {
			song->updateCache();
			songItem->update();
			songTree->setCurrentItem(songItem);

			updateDetails();
			break;
		}
	}

	logSrv->add(QString("Song file changed: \"%1\"").arg(song->songFileInfo().filePath()), QU::information);
}

/*!
 * Select a path and copy all audio files of all songs to that path. (no public feature)
 */
void QUMainWindow::copyAudioToPath() {
	QString target = QFileDialog::getExistingDirectory(this, tr("Choose backup destination..."));

	if(target.isEmpty())
		return;

	QUProgressDialog dlg(tr("Backup audio files..."), _songs.size(), this);
	dlg.setPixmap(":/types/music.png");
	dlg.show();

	foreach(QUSongFile *song, _songs) {
		if(!song->hasMp3())
			continue;

		dlg.update(song->mp3FileInfo().fileName());
		if(dlg.cancelled()) break;

		// TODO: copy files
		QFile::copy(song->mp3FileInfo().filePath(), QFileInfo(QDir(target), song->mp3FileInfo().fileName()).filePath());
	}

	logSrv->add(tr("Backup for audio files finished."), QU::information);
}

/*!
 * This is used to necessary information to the media player so that a song can be played.
 */
void QUMainWindow::sendSelectedSongsToMediaPlayer() {
	mediaplayer->setSongs(songTree->selectedSongs());
	mediaplayer->play();
}

void QUMainWindow::sendAllSongsToMediaPlayer() {
	mediaplayer->setSongs(_songs);
	mediaplayer->play();
}

void QUMainWindow::sendVisibleSongsToMediaPlayer() {
	mediaplayer->setSongs(songTree->visibleSongs());
	mediaplayer->play();
}

void QUMainWindow::sendCurrentPlaylistToMediaPlayer() {
	mediaplayer->setSongs(playlistArea->currentPlaylist()->connectedSongs());
	mediaplayer->play();
}

/*!
 * Displays a dialog to manage plugins generally.
 */
void QUMainWindow::showPluginDialog() {
	QUPluginDialog dlg(taskList->plugins(), this);
	connect(&dlg, SIGNAL(pluginReloadRequested()), taskList, SLOT(reloadAllPlugins()));
	connect(taskList, SIGNAL(pluginsReloaded(const QList<QPluginLoader*>&)), &dlg, SLOT(updatePluginTable(const QList<QPluginLoader*>&)));
	dlg.exec();
}

void QUMainWindow::showCoverSlideShowDialog(QList<QUSongItem*> items) {
	QUSlideShowDialog dlg(items, QUSlideShowDialog::coverflow, this);

	if(dlg.exec()) {
		updateDetails();
		montyTalk();
	}
}

void QUMainWindow::showBackgroundSlideShowDialog(QList<QUSongItem*> items) {
	QUSlideShowDialog dlg(items, QUSlideShowDialog::backgroundflow, this);

	if(dlg.exec()) {
		updateDetails();
		montyTalk();
	}
}

void QUMainWindow::showPathsDialog() {
	QUPathsDialog(false, this).exec();
}
