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
#include <QTextCodec>
#include <QProgressDialog>

#include <QDesktopServices>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTemporaryFile>

#include "QUSongItem.h"
#include "QUDetailItem.h"

#include "QUMonty.h"
#include "QULogService.h"
#include "QUSongDatabase.h"
#include "QUPlaylistDatabase.h"
#include "QUPluginManager.h"

#include "taglib.h"
#include "bass.h"

#include "QUTagOrderDialog.h"
#include "QUTextDialog.h"
#include "QUProgressDialog.h"
#include "QUReportDialog.h"
#include "QUMessageBox.h"
#include "QUPictureDialog.h"
#include "QUCustomTagsDialog.h"
#include "QUEncodingsDialog.h"
#include "QUAboutDialog.h"
#include "QUPluginDialog.h"
#include "QUSlideShowDialog.h"
#include "QULyricsEditorDialog.h"
#include "QUPathsDialog.h"
#include "QURemoteImageDialog.h"

#include "QUSongSupport.h"

#include "QUTaskFactoryProxy.h"

#include "QURibbonBar.h"

QUMainWindow::QUMainWindow(QWidget *parent): QMainWindow(parent) {
	setupUi(this);

	songDB->setParentWidget(this);
	playlistDB->setParentWidget(this);

	initWindow();
	initRibbonBar();

	initStatusBar();
	initEventLog();
	initConfig();
	initPluginManager();

	initSongTree();
	initDetailsTable();
	initTaskList();

	initMonty();
	initMediaPlayer();

	playlistDB->reload();

	QSettings settings;
	if (settings.value("allowUpdateCheck", QVariant(false)).toBool()) {
		this->checkForUpdate(true);
	}

	addLogMsg(tr("Ready."), QU::Information);
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
				BTN << ":/control/save_all.png"	<< tr("Save all changed songs.")
					<< ":/control/bin.png"		<< tr("Discard all changes.")
					<< ":/marks/cancel.png"		<< tr("Cancel this action."));
		if(result == 0)
			songTree->saveUnsavedChanges();
		else if(result == 2) {
			event->ignore();
			return;
		}
	}

	if(playlistDB->hasUnsavedChanges()) {
		result = QUMessageBox::information(this,
				tr("Quit"),
				tr("<b>Playlists</b> have been modified."),
				BTN << ":/control/save_all.png"	<< tr("Save all changed playlists.")
					<< ":/control/bin.png"		<< tr("Discard all changes.")
					<< ":/marks/cancel.png"		<< tr("Cancel this action."));
		if(result == 0)
			playlistDB->saveUnsavedChanges();
		else if(result == 2) {
			event->ignore();
			return;
		}
	}

	QSettings settings;
	settings.setValue("allowMonty", QVariant(_menu->montyBtn->isChecked()));
	settings.setValue("geometry", QVariant(saveGeometry()));
	settings.setValue("windowState", QVariant(saveState()));

	settings.setValue("showInfoMessages", showInfosBtn->isChecked());
	settings.setValue("showHelpMessages", showHelpBtn->isChecked());
	settings.setValue("showSaveMessages", showSaveHintsBtn->isChecked());
	settings.setValue("showWarningMessages", showWarningsBtn->isChecked());
	settings.setValue("showErrorMessages", showErrorsBtn->isChecked());

	settings.setValue("autoSave", QVariant(_menu->autoSaveBtn->isChecked()));

	this->saveLog();

	// everything sould be fine from now on
	QFile::remove("running.app");

	event->accept();
}

void QUMainWindow::initPluginManager() {
	pluginMGR->setUiParent(this);
	pluginMGR->reload(); // initial loading of plugins

	connect(pluginMGR, SIGNAL(reloaded()), this, SLOT(initTaskListMenu()));
	connect(pluginMGR, SIGNAL(reloaded()), taskList, SLOT(resetTaskList()));
	connect(pluginMGR, SIGNAL(configurationAdded()), taskList, SLOT(resetTaskList()));
}

/*!
 * Initializes the windows registry entry for uman. Lets the user
 * choose a path where the song files are located.
 */
void QUMainWindow::initConfig() {
	QSettings settings;
	QString path;
	if(!settings.contains("songPath")) {
		if(!QUPathsDialog(true, this).exec())
			logSrv->add(tr("No song directory was selected."), QU::Warning);
	}

	path = settings.value("songPath").toString();
	QU::BaseDir.setPath(path);

	// read other settings
	_menu->montyBtn->setChecked(settings.value("allowMonty", true).toBool());
	_menu->relativePathsChk->setChecked(settings.value("showRelativeSongPath", true).toBool());
	_menu->otherSymbolsChk->setChecked(settings.value("altSongTree", false).toBool());
	completerChk->setChecked(settings.value("caseSensitiveAutoCompletion", false).toBool());

	_menu->autoSaveBtn->setChecked(settings.value("autoSave", true).toBool());
	_menu->onTopChk->setChecked(settings.value("alwaysOnTop", false).toBool());

	restoreGeometry(settings.value("geometry").toByteArray());
	restoreState(settings.value("windowState").toByteArray());

	updateViewButtons();

	connect(detailsDock, SIGNAL(visibilityChanged(bool)), this, SLOT(updateViewButtons()));
	connect(tasksDock, SIGNAL(visibilityChanged(bool)), this, SLOT(updateViewButtons()));
	connect(playlistDock, SIGNAL(visibilityChanged(bool)), this, SLOT(updateViewButtons()));
	connect(mediaPlayerDock, SIGNAL(visibilityChanged(bool)), this, SLOT(updateViewButtons()));
	connect(previewDock, SIGNAL(visibilityChanged(bool)), this, SLOT(updateViewButtons()));
	connect(eventsDock, SIGNAL(visibilityChanged(bool)), this, SLOT(updateViewButtons()));

	if(QLocale(settings.value("language").toString()).language() == QLocale::English) {
		_menu->langUsBtn->setChecked(true);
	} else if(QLocale(settings.value("language").toString()).language() == QLocale::German) {
		_menu->langDeBtn->setChecked(true);
	} else if(QLocale(settings.value("language").toString()).language() == QLocale::Polish) {
		_menu->langPlBtn->setChecked(true);
	} else if(QLocale(settings.value("language").toString()).language() == QLocale::French) {
		_menu->langFrBtn->setChecked(true);
	} else if(QLocale(settings.value("language").toString()).language() == QLocale::Spanish) {
		_menu->langEsBtn->setChecked(true);
	} else if(QLocale(settings.value("language").toString()).language() == QLocale::Portuguese) {
		_menu->langPtBtn->setChecked(true);
	}

	QStringList imageFormatsNeeded;
	QStringList imageFormatsQt;
	QStringList imageFormatsUman = QUSongSupport::allowedImageFiles().join(" ").remove("*.").split(" ");
	foreach(QByteArray ba, QImageReader::supportedImageFormats())
		imageFormatsQt << ba;
	foreach(QString neededFormat, imageFormatsUman)
		if(imageFormatsQt.indexOf(neededFormat) == -1)
			imageFormatsNeeded << neededFormat;
	if(imageFormatsNeeded.size() > 0)
		logSrv->add(QString(tr("Cannot read image formats: %1")).arg(imageFormatsNeeded.join(", ")), QU::Warning);
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
	addDockWidget(Qt::LeftDockWidgetArea, previewDock);

	addDockWidget(Qt::RightDockWidgetArea, tasksDock);
	addDockWidget(Qt::RightDockWidgetArea, playlistDock);
	addDockWidget(Qt::RightDockWidgetArea, mediaPlayerDock);

	addDockWidget(Qt::RightDockWidgetArea, eventsDock); eventsDock->setFloating(true); eventsDock->hide();

	tabifyDockWidget(previewDock, detailsDock);
	tabifyDockWidget(playlistDock, mediaPlayerDock);
	tabifyDockWidget(mediaPlayerDock, tasksDock);

	connect(logSrv, SIGNAL(messageAdded(const QString&, QU::MessageTypes)), this, SLOT(addLogMsg(const QString&, QU::MessageTypes)));

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

	// other things
	QAction *a = new QAction(this);
	a->setShortcut(Qt::CTRL + Qt::Key_F11);
	connect(a, SIGNAL(triggered()), this, SLOT(toggleFullScreenMode()));
	addAction(a);
}

/*!
 * Create an Office2007-like menu to reduce mouse clicks.
 */
void QUMainWindow::initRibbonBar() {
	_menu = new QURibbonBar(this);
	this->setMenuWidget(_menu);

	// songs menu
	connect(_menu->saveBtn, SIGNAL(clicked()), songTree, SLOT(saveSelectedSongs()));
	connect(_menu->deleteBtn, SIGNAL(clicked()), songTree, SLOT(requestDeleteSelectedSongs()));
	connect(_menu->mergeBtn, SIGNAL(clicked()), songTree, SLOT(mergeSelectedSongs()));

	connect(_menu->getCoversBtn, SIGNAL(clicked()), this, SLOT(getCovers()));

	QMenu *pictureFlowMenu = new QMenu(tr("Review pictures"));
	pictureFlowMenu->addAction(QIcon(":/types/cover.png"),	  tr("Covers..."),	  songTree, SLOT(requestCoverFlow()));
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
	_menu->setShortcut(_menu->openExpBtn, Qt::CTRL + Qt::Key_Return);
	_menu->setShortcut(_menu->editLyricsBtn, Qt::CTRL + Qt::Key_E);

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

	_menu->setShortcut(_menu->detailsBtn,   Qt::CTRL + Qt::Key_1);
	_menu->setShortcut(_menu->tasksBtn,	 Qt::CTRL + Qt::Key_2);
	_menu->setShortcut(_menu->playlistsBtn, Qt::CTRL + Qt::Key_3);
	_menu->setShortcut(_menu->playerBtn,	Qt::CTRL + Qt::Key_4);
	_menu->setShortcut(_menu->fileInfoBtn,  Qt::CTRL + Qt::Key_5);
	_menu->setShortcut(_menu->eventLogBtn,  Qt::CTRL + Qt::Key_6);

	// options menu
	_menu->updateBaseDirMenu();

	connect(_menu->autoSaveBtn, SIGNAL(toggled(bool)), this, SLOT(toggleAutoSaveChk(bool)));
	connect(_menu->onTopChk, SIGNAL(toggled(bool)), this, SLOT(toggleAlwaysOnTop(bool)));

	connect(_menu->tagSaveOrder, SIGNAL(clicked()), this, SLOT(editTagOrder()));
	connect(_menu->customTagsBtn, SIGNAL(clicked()), this, SLOT(editCustomTags()));
	connect(_menu->pathsBtn, SIGNAL(clicked()), this, SLOT(showPathsDialog()));
	connect(_menu->defaultEncodingsBtn, SIGNAL(clicked()), this, SLOT(setDefaultEncodings()));

	_menu->mediumMp3QualityComboBox->setCurrentIndex(_menu->mediumMp3QualityComboBox->findText(QString::number(QUSongSupport::mediumMp3Quality()), Qt::MatchStartsWith));
	_menu->highMp3QualityComboBox->setCurrentIndex(_menu->highMp3QualityComboBox->findText(QString::number(QUSongSupport::highMp3Quality()), Qt::MatchStartsWith));
	_menu->mediumCoverQualityComboBox->setCurrentIndex(_menu->mediumCoverQualityComboBox->findText(QString::number(QUSongSupport::mediumCoverQuality()), Qt::MatchStartsWith));
	_menu->highCoverQualityComboBox->setCurrentIndex(_menu->highCoverQualityComboBox->findText(QString::number(QUSongSupport::highCoverQuality()), Qt::MatchStartsWith));
	_menu->mediumBackgroundQualityComboBox->setCurrentIndex(_menu->mediumBackgroundQualityComboBox->findText(QString::number(QUSongSupport::mediumBackgroundQuality()), Qt::MatchStartsWith));
	_menu->highBackgroundQualityComboBox->setCurrentIndex(_menu->highBackgroundQualityComboBox->findText(QString::number(QUSongSupport::highBackgroundQuality()), Qt::MatchStartsWith));

	connect(_menu->mediumMp3QualityComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(setMediumMp3Quality(QString)));
	connect(_menu->highMp3QualityComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(setHighMp3Quality(QString)));
	connect(_menu->mediumCoverQualityComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(setMediumCoverQuality(QString)));
	connect(_menu->highCoverQualityComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(setHighCoverQuality(QString)));
	connect(_menu->mediumBackgroundQualityComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(setMediumBackgroundQuality(QString)));
	connect(_menu->highBackgroundQualityComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(setHighBackgroundQuality(QString)));

	connect(_menu->langUsBtn, SIGNAL(clicked()), this, SLOT(enableEnglish()));
	connect(_menu->langDeBtn, SIGNAL(clicked()), this, SLOT(enableGerman()));
	connect(_menu->langPlBtn, SIGNAL(clicked()), this, SLOT(enablePolish()));
	connect(_menu->langFrBtn, SIGNAL(clicked()), this, SLOT(enableFrench()));
	connect(_menu->langEsBtn, SIGNAL(clicked()), this, SLOT(enableSpanish()));
	connect(_menu->langPtBtn, SIGNAL(clicked()), this, SLOT(enablePortuguese()));

	connect(_menu, SIGNAL(changeSongPathRequested(QString)), this, SLOT(changeSongDir(QString)));

	// extras menu
	connect(_menu->reportBtn, SIGNAL(clicked()), this, SLOT(reportCreate()));
	connect(_menu->backupAudioBtn, SIGNAL(clicked()), this, SLOT(copyAudioToPath()));
	connect(_menu->pluginsBtn, SIGNAL(clicked()), this, SLOT(showPluginDialog()));

	_menu->setShortcut(_menu->reportBtn, Qt::Key_F8);

	_menu->hideSongsBtn->setMenu(songTree->hideMenu());

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
	connect(_menu->checkForUpdateBtn, SIGNAL(clicked(bool)), this, SLOT(checkForUpdate(bool)));

	// help menu
	connect(_menu->helpBtn, SIGNAL(clicked()), montyArea, SLOT(show()));
	_menu->setShortcut(_menu->helpBtn, Qt::Key_F1);
	_menu->setShortcut(_menu->montyBtn, Qt::SHIFT + Qt::Key_F1);
	_menu->setShortcut(_menu->hideBtn, Qt::CTRL + Qt::Key_F1);
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
	connect(songTree, SIGNAL(itemActivated(QTreeWidgetItem*, int)), this, SLOT(applyDefaultAction(QTreeWidgetItem*, int)));

	connect(songTree, SIGNAL(itemExpanded(QTreeWidgetItem*)), songTree, SLOT(resizeToContents()));
	connect(songTree, SIGNAL(itemCollapsed(QTreeWidgetItem*)), songTree, SLOT(resizeToContents()));

	connect(songTree, SIGNAL(songToPlaylistRequested(QUSongFile*)), playlistArea, SLOT(addSongToCurrentPlaylist(QUSongFile*)));
	connect(songTree, SIGNAL(coversRequested(QList<QUSongItem*>)), this, SLOT(getCovers(QList<QUSongItem*>)));
	connect(songTree, SIGNAL(coverFlowRequested(QList<QUSongItem*>)), this, SLOT(showCoverSlideShowDialog(QList<QUSongItem*>)));
	connect(songTree, SIGNAL(backgroundFlowRequested(QList<QUSongItem*>)), this, SLOT(showBackgroundSlideShowDialog(QList<QUSongItem*>)));

	connect(songTree, SIGNAL(showLyricsRequested(QUSongFile*)), this, SLOT(showLyrics(QUSongFile*)));
	connect(songTree, SIGNAL(editLyricsRequested(QUSongFile*)), this, SLOT(editSongLyrics(QUSongFile*)));

	connect(songTree, SIGNAL(playSelectedSongsRequested()), this, SLOT(sendSelectedSongsToMediaPlayer()));

	connect(songTree, SIGNAL(openCurrentFileExternalRequested(QTreeWidgetItem*)), this, SLOT(openExternally(QTreeWidgetItem*)));
	connect(songTree, SIGNAL(openCurrentFileInternalRequested(QTreeWidgetItem*)), this, SLOT(openInternally(QTreeWidgetItem*)));
	connect(songTree, SIGNAL(setCurrentFilePrimaryRequested(QTreeWidgetItem*)), this, SLOT(setPrimary(QTreeWidgetItem*)));

	refreshAllSongs();
	updateMergeBtn();
}

void QUMainWindow::initDetailsTable() {
	connect(detailsTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(editSongSetDetail(QTableWidgetItem*)));

	connect(completerChk, SIGNAL(toggled(bool)), this, SLOT(toggleCompleterChk(bool)));
}

/*!
 * Initializes all available tasks. Plugin Manager should have loaded all plugins before
 * calling this method.
 *
 * \sa editSongApplyTasks()
 * \sa editSongSetDetail()
 */
void QUMainWindow::initTaskList() {
	initTaskListMenu();

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

	taskList->resetTaskList();
}

void QUMainWindow::initTaskListMenu() {
	if(addTaskBtn->menu())
		delete addTaskBtn->menu();
	addTaskBtn->setMenu(new QMenu);
	addTaskBtn->setPopupMode(QToolButton::InstantPopup);

	foreach(QUTaskFactoryProxy *fp, pluginMGR->taskFactoryProxies()) {
		if(fp->factory()->canAddConfigurations())
			addTaskBtn->menu()->addAction(QString("%1...").arg(fp->factory()->productName()), fp, SLOT(addConfiguration()));
	}
}

void QUMainWindow::initEventLog() {
	QSettings settings;
	showInfosBtn->setChecked(settings.value("showInfoMessages", true).toBool());
	showHelpBtn->setChecked(settings.value("showHelpMessages", true).toBool());
	showSaveHintsBtn->setChecked(settings.value("showSaveMessages", true).toBool());
	showWarningsBtn->setChecked(settings.value("showWarningMessages", true).toBool());
	showErrorsBtn->setChecked(settings.value("showErrorMessages", true).toBool());

	connect(logClearBtn, SIGNAL(clicked()), this, SLOT(clearLog()));
	connect(logSaveBtn, SIGNAL(clicked()), this, SLOT(saveLog()));
}

void QUMainWindow::initMonty() {
	montyArea->montyLbl->setPixmap(monty->pic(QUMonty::seated));
	montyArea->helpLbl->setText(monty->welcomeMsg());

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

void QUMainWindow::initMediaPlayer() {
	connect(mediaplayer, SIGNAL(editSongRequested(QUSongFile*,int)), this, SLOT(editSongLyrics(QUSongFile*,int)));
}

void QUMainWindow::initStatusBar() {
	_timer.setSingleShot(true);
	_timer.setInterval(10000);
	connect(&_timer, SIGNAL(timeout()), this, SLOT(clearStatusMessage()));

	_statusIconLbl = new QLabel();
	_statusMessageLbl = new QLabel();

	statusBar()->addWidget(_statusIconLbl);
	statusBar()->addWidget(_statusMessageLbl);

	_statusIconLbl->hide();
	_statusMessageLbl->hide();

	_toggleEventLogBtn = new QToolButton();
	_toggleEventLogBtn->setIcon(QIcon(":/control/log.png"));
	_toggleEventLogBtn->setAutoRaise(true);
	_toggleEventLogBtn->setCheckable(true);
	connect(_toggleEventLogBtn, SIGNAL(clicked(bool)), eventsDock, SLOT(setVisible(bool)));
	connect(_menu->eventLogBtn, SIGNAL(toggled(bool)), _toggleEventLogBtn, SLOT(setChecked(bool)));

	statusBar()->addPermanentWidget(_toggleEventLogBtn);
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
					<< ":/control/bin.png"	  << tr("Discard all changes.")
					<< ":/marks/cancel.png"	 << tr("Cancel this action."));
		if(result == 2)
			return;
		else if(result == 0)
			songTree->saveUnsavedChanges();
	}

	// -------------------------------------

	songDB->reload();
	updateDetails();
	updatePreviewTree();
	pluginMGR->reload();
}

/*!
 * Update the details of the selected songs. Disconnect to avoid dead-lock.
 */
void QUMainWindow::updateDetails() {
//	logSrv->add("updateDetails", QU::Error);

	disconnect(detailsTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(editSongSetDetail(QTableWidgetItem*)));
	detailsTable->updateValueColumn(songTree->selectedSongItems());
	connect(detailsTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(editSongSetDetail(QTableWidgetItem*)));
}

/*!
 * Updates all preview information according to the current selection or dimension of the
 * song tree.
 */
void QUMainWindow::updatePreviewTree() {
	previewTree->setSongCount(songDB->songCountWithoutFriends(), songDB->songFriendsCount());
	previewTree->setSelectedSongCount(songTree->selectedSongItems().size(), songTree->selectedSongFriendsCount());
	previewTree->setVisibleSongCount(songTree->topLevelItemCount(), songTree->visibleSongFriendsCount());
	previewTree->setHiddenSongCount(songTree->hiddenItemsCount(), songTree->hiddenSongFriendsCount());

	if(calculateTimeChk->checkState() == Qt::Checked) {
		int totalSongTime = 0;
		int totalAudioTime = 0;
		int totalTotalTime = 0;
		foreach(QUSongFile *song, songTree->selectedSongs()) {
			totalSongTime += song->length();
			totalAudioTime += song->lengthMp3();
			totalTotalTime += song->lengthEffective();

			foreach(QUSongFile *song, song->friends()) {
				totalSongTime += song->length();
				totalAudioTime += song->lengthMp3();
				totalTotalTime += song->lengthEffective();
			}
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

void QUMainWindow::updateViewButtons() {
	_menu->detailsBtn->setChecked(detailsDock->isVisible() || !tabifiedDockWidgets(detailsDock).isEmpty());
	_menu->tasksBtn->setChecked(tasksDock->isVisible() || !tabifiedDockWidgets(tasksDock).isEmpty());
	_menu->playlistsBtn->setChecked(playlistDock->isVisible() || !tabifiedDockWidgets(playlistDock).isEmpty());
	_menu->playerBtn->setChecked(mediaPlayerDock->isVisible() || !tabifiedDockWidgets(mediaPlayerDock).isEmpty());
	_menu->fileInfoBtn->setChecked(previewDock->isVisible() || !tabifiedDockWidgets(previewDock).isEmpty());
	_menu->eventLogBtn->setChecked(eventsDock->isVisible() || !tabifiedDockWidgets(eventsDock).isEmpty());
}

/*!
 * Save all changes of the details into the song file.
 * \sa updateDetails()
 */
void QUMainWindow::editSongSetFileLink(QTreeWidgetItem *item, int column) {
	if(column < MP3_COLUMN || column > VIDEO_COLUMN)
		return;

	QUSongItem *songItem = dynamic_cast<QUSongItem*>(item);

	if(!songItem || songItem->isToplevel())
		return;

	QUSongFile *song = songItem->song();

	QString fileScheme("*." + QFileInfo(songItem->text(FOLDER_COLUMN)).suffix());

	if( songItem->icon(MP3_COLUMN).isNull()
			&& QUSongSupport::allowedAudioFiles().contains(fileScheme, Qt::CaseInsensitive)
			&& column == MP3_COLUMN ) {
		logSrv->add(QString(tr("Audio file changed from \"%1\" to: \"%2\".")).arg(song->mp3()).arg(songItem->text(FOLDER_COLUMN)), QU::Information);
		song->setInfo(MP3_TAG, songItem->text(FOLDER_COLUMN));
		song->save();
	} else if( songItem->icon(COVER_COLUMN).isNull()
			&& QUSongSupport::allowedImageFiles().contains(fileScheme, Qt::CaseInsensitive)
			&& column == COVER_COLUMN ) {
		logSrv->add(QString(tr("Cover changed from \"%1\" to: \"%2\".")).arg(song->cover()).arg(songItem->text(FOLDER_COLUMN)), QU::Information);
		song->setInfo(COVER_TAG, songItem->text(FOLDER_COLUMN));
		song->save();
	} else if( songItem->icon(BACKGROUND_COLUMN).isNull()
			&& QUSongSupport::allowedImageFiles().contains(fileScheme, Qt::CaseInsensitive)
			&& column == BACKGROUND_COLUMN ) {
		logSrv->add(QString(tr("Background changed from \"%1\" to: \"%2\".")).arg(song->background()).arg(songItem->text(FOLDER_COLUMN)), QU::Information);
		song->setInfo(BACKGROUND_TAG, songItem->text(FOLDER_COLUMN));
		song->save();
	} else if( songItem->icon(VIDEO_COLUMN).isNull()
			&& QUSongSupport::allowedVideoFiles().contains(fileScheme, Qt::CaseInsensitive)
			&& column == VIDEO_COLUMN ) {
		logSrv->add(QString(tr("Video file changed from \"%1\" to: \"%2\".")).arg(song->video()).arg(songItem->text(FOLDER_COLUMN)), QU::Information);
		song->setInfo(VIDEO_TAG, songItem->text(FOLDER_COLUMN));
		song->save();
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

	disconnect(songTree, SIGNAL(itemSelectionChanged()), this, SLOT(updateDetails()));

	songTree->clearSelection();

	// save changes for each song
	foreach(QUSongItem *songItem, selectedItems) {
		dlg.update(QString("%1 - %2").arg(songItem->song()->artist()).arg(songItem->song()->title()));
		if(dlg.cancelled()) break;

		// delete medley tags if CALCMEDLEY_TAG is set to OFF
		songItem->song()->setInfo(tag, text);
		if(tag == CALCMEDLEY_TAG && text == "OFF") {
			songItem->song()->setInfo(MEDLEYSTARTBEAT_TAG, "");
			songItem->song()->setInfo(MEDLEYENDBEAT_TAG, "");
		}

		songItem->song()->save();

		songItem->update();
	}
	songTree->restoreSelection(selectedItems);
	songTree->scrollToItem(songTree->currentItem(), QAbstractItemView::EnsureVisible);

	connect(songTree, SIGNAL(itemSelectionChanged()), this, SLOT(updateDetails()));
	updateDetails();
}

/*!
 * Does all checked tasks for all selected songs. You can only select
 * toplevel items (folders) which represent songs.
 * \sa initTaskList();
 */
void QUMainWindow::editSongApplyTasks() {
	QList<QUSongItem*> songItems = songTree->selectedSongItems();
	QList<bool>		itemExpandedStates;

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

		songDB->ignoreChangesForSong(song);
		taskList->doTasksOn(song);
		songDB->processChangesForSong(song);

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

	if(dlg.execAt(0))
		song->save();
}

void QUMainWindow::editSongLyrics(QUSongFile *song, int line) {
	if(!song)
		return;

	QULyricsEditorDialog dlg(song, this);

	if(dlg.execAt(line))
		song->save();
}

void QUMainWindow::addLogMsg(const QString &msg, QU::MessageTypes type) {
	if(type == QU::Information && !showInfosBtn->isChecked())
		return;
	if(type == QU::Warning && !showWarningsBtn->isChecked())
		return;
	if(type == QU::Saving && !showSaveHintsBtn->isChecked())
		return;
	if(type == QU::Help && !showHelpBtn->isChecked())
		return;
	if(type == QU::Error && !showErrorsBtn->isChecked())
		return;

	log->insertItem(0, QDateTime::currentDateTime().toString("[hh:mm:ss] ") + msg);
	QListWidgetItem *lastItem = log->item(0);

	switch(type) {
	case 0: lastItem->setIcon(QIcon(":/marks/information.png")); lastItem->setData(Qt::UserRole, "I"); break;
	case 1: lastItem->setIcon(QIcon(":/marks/error.png")); lastItem->setData(Qt::UserRole, "!"); break;
	case 2: lastItem->setIcon(QIcon(":/marks/help.png")); lastItem->setData(Qt::UserRole, "?"); break;
	case 3: lastItem->setIcon(QIcon(":/marks/disk.png")); lastItem->setData(Qt::UserRole, "S"); break;
	case 4: lastItem->setIcon(QIcon(":/marks/cancel.png")); lastItem->setData(Qt::UserRole, "E"); break;
	}

	// show it in the status bar
	_timer.stop();

	_statusIconLbl->setPixmap(lastItem->icon().pixmap(16, 16));
	_statusMessageLbl->setText(msg);

	_statusIconLbl->show();
	_statusMessageLbl->show();

	_timer.start();
}

/*!
 * Save the log to a default location.
 */
void QUMainWindow::saveLog() {
	if(log->count() == 0)
		return;

	QDir logDir = QDir(QCoreApplication::applicationDirPath()); logDir.mkdir("logs"); logDir.cd("logs");
	QString filePath = QFileInfo(logDir, QString("%1.txt").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd_hhmmss"))).filePath();
	QFile file(filePath);

	if(file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
		QTextStream out(&file);
		out.setCodec(QTextCodec::codecForName("UTF-8"));
		out.setGenerateByteOrderMark(true);

		for(int row = 0; row < log->count(); ++row) {
			out << QString("%1 %2").arg(log->item(row)->data(Qt::UserRole).toString()).arg(log->item(row)->text()) << endl;
		}
		file.close();

		logSrv->add(QString(tr("The log file was saved to: \"%1\"")).arg(filePath), QU::Saving);
	} else
		logSrv->add(QString(tr("The log file COULD NOT be saved.")).arg(filePath), QU::Warning);
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
	BYTE BASS_MAJOR_VERSION = (BASS_GetVersion()    >> 24) & 0xFF;
	BYTE BASS_MINOR_VERSION = (BASS_GetVersion()    >> 16) & 0xFF;
	BYTE BASS_PATCH_VERSION = (BASS_GetVersion()    >>  8) & 0xFF;
	BYTE BASS_REVISION      = (BASS_GetVersion()         ) & 0xFF;
	QString BASS_VERSION    = QString("%1.%2.%3.%4").arg(BASS_MAJOR_VERSION).arg(BASS_MINOR_VERSION).arg(BASS_PATCH_VERSION).arg(BASS_REVISION);

	QUMessageBox::information(this,
			tr("About BASS"),
			QString(tr("<b>BASS Audio Library</b><br><br>"
					"BASS is an audio library for use in Windows and MacOSX software. Its purpose is to provide the most powerful and efficient (yet easy to use), sample, stream, MOD music, and recording functions. All in a tiny DLL, under 100KB in size.<br><br>"
					"Version: <b>%1</b><br><br>"
					"Copyright (c) 1999-2012<br><a href=\"http://www.un4seen.com/bass.html\">Un4seen Developments Ltd.</a> All rights reserved."))
					.arg(BASS_VERSION),
			BTN << ":/marks/accept.png" << "OK",
			330);
}

void QUMainWindow::checkForUpdate(bool silent) {
	int currentVersion = MAJOR_VERSION*100 + MINOR_VERSION*10 + PATCH_VERSION;

	QUrl url("http://uman.svn.sourceforge.net/viewvc/uman/uman/src/latest_version.xml");
	QNetworkAccessManager *m_NetworkMngr = new QNetworkAccessManager(this);
	QNetworkReply *reply = m_NetworkMngr->get(QNetworkRequest(url));

	QEventLoop loop;
	connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();
	if(reply->error() != QNetworkReply::NoError) {
		if (!silent) {
			QUMessageBox::warning(this,
					tr("Update check failed."),
					QString(tr("Is your internet connection working?")),
					BTN << ":/marks/accept.png" << "OK",
					240);
		}
		logSrv->add(QString(tr("Update check failed. Host unreachable.")), QU::Error);
		return;
	}

	QTemporaryFile tmp;
	if (!tmp.open()) {
		if (!silent) {
			QUMessageBox::warning(this,
					tr("Update check failed."),
					QString(tr("No permission to write file %1.")).arg(tmp.fileName()),
					BTN << ":/marks/accept.png" << "OK",
					240);
		}
		logSrv->add(QString(tr("Update check failed. No permission to write file %1.")).arg(tmp.fileName()), QU::Error);
		return;
	}

	tmp.write(reply->readAll());
	tmp.seek(0);
	QString line =  QString(tmp.readLine());
	QString latestVersionString = line;
	int latestVersion = line.remove('.').toInt();
	delete reply;
	tmp.remove();

	if (currentVersion < latestVersion) {
		QUMessageBox::information(this,
				tr("Update check successful."),
				QString(tr("UltraStar Manager %1.%2.%3 is <b>outdated</b>.<br><br>"
						"Download the most recent UltraStar Manager %4 <a href='http://sourceforge.net/projects/uman/'>here</a>."))
						.arg(MAJOR_VERSION).arg(MINOR_VERSION).arg(PATCH_VERSION)
						.arg(latestVersionString),
				BTN << ":/marks/accept.png" << "OK",
				240);
		logSrv->add(QString(tr("Update check successful. A new version of UltraStar Manager is available.")), QU::Information);
	} else {
		logSrv->add(QString(tr("Update check successful. UltraStar Manager is up to date.")), QU::Information);
		if (!silent) {
			QSettings settings;
			int result = QUMessageBox::information(this,
					tr("Update check successful."),
					QString(tr("UltraStar Manager %1.%2.%3 is <b>up to date</b>!"))
							.arg(MAJOR_VERSION).arg(MINOR_VERSION).arg(PATCH_VERSION),
					BTN << ":/marks/accept.png" << tr("OK. I will check again later.")
						<< ":/marks/accept.png" << tr("OK. Check automatically on startup."),
					240);
			if(result == 0) {
				settings.setValue("allowUpdateCheck", QVariant(false));
				logSrv->add(QString(tr("Automatic check for updates disabled.")), QU::Information);
			} else {
				settings.setValue("allowUpdateCheck", QVariant(true));
				logSrv->add(QString(tr("Automatic check for updates enabled.")), QU::Information);
			}
		}
	}
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
void QUMainWindow::changeSongDir(const QString &path) {
	if(path.isEmpty())
		return;

	if(songTree->hasUnsavedChanges()) {
		int result = QUMessageBox::information(this,
				tr("Change Song Directory"),
				tr("Songs have been modified."),
				BTN << ":/control/save_all.png" << tr("Save all changes.")
					<< ":/control/bin.png"	  << tr("Discard all changes.")
					<< ":/marks/cancel.png"	 << tr("Cancel this action."));
		if(result == 2)
			return;
		else if(result == 0)
			songTree->saveUnsavedChanges();
	}

	// ---------------------------------------------------------

	QSettings settings;

	settings.setValue("songPath", path);
	QU::BaseDir.setPath(path);
	refreshAllSongs(true);

	montyTalk();

	logSrv->add(QString(tr("UltraStar song directory changed to: \"%1\".")).arg(QDir::toNativeSeparators(QU::BaseDir.path())), QU::Information);

	songTree->headerItem()->setText(FOLDER_COLUMN, QString(tr("Folder (%1)")).arg(QDir::toNativeSeparators(QU::BaseDir.path())));
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
					<< ":/control/bin.png"	  << tr("Discard all changes.")
					<< ":/marks/cancel.png"	 << tr("Cancel this action."));
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

		logSrv->add(QString(tr("Custom tags changed to: \"%1\"")).arg(QUSongSupport::availableCustomTags().join(", ")), QU::Information);
	}

	delete dlg;

	montyTalk();
}

void QUMainWindow::montyTalk(bool force) {
	if(!force && !_menu->montyBtn->isChecked())
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

	for(int i = 0; i < songTree->topLevelItemCount(); ++i)
		tmpList.append(dynamic_cast<QUSongItem*>(songTree->topLevelItem(i)));

	foreach(QUSongItem *item, tmpList) {
		dlg.update(item->song()->songFileInfo().dir().dirName());

		item->updateAsDirectory(checked);
	}

	if(checked)
		logSrv->add(tr("Relative song paths are displayed in the song tree now."), QU::Information);
	else
		logSrv->add(tr("Only song directories are displayed in the song tree now."), QU::Information);
}

/*!
 * Toggle whether the completion for detail edition should be case-sensitive.
 */
void QUMainWindow::toggleCompleterChk(bool checked) {
	QSettings settings;
	settings.setValue("caseSensitiveAutoCompletion", QVariant(checked));
}

/*!
 * Toggle whether changes are saved automatically or need to be saved manually.
 */
void QUMainWindow::toggleAutoSaveChk(bool checked) {
	QSettings settings;
	settings.setValue("autoSave", QVariant(checked));

	_menu->saveAllBtn->setEnabled(!checked);
}

/*!
 * Toggle whether alternative song tree icons are used.
 */
void QUMainWindow::toggleAltSongTreeChk(bool checked) {
	QSettings settings;
	if(settings.value("altSongTree", false) == checked)
		return; // no change

	settings.setValue("altSongTree", checked);

	QUProgressDialog dlg(tr("Repainting song tree icons..."), songTree->topLevelItemCount(), this, false);
	dlg.setPixmap(":/types/folder.png");
	dlg.show();

	for(int i = 0; i < songTree->topLevelItemCount(); ++i) {
		QUSongItem *songItem = dynamic_cast<QUSongItem*>(songTree->topLevelItem(i));

		if(songItem) {
			dlg.update(songItem->song()->songFileInfo().dir().dirName());
			songItem->updateSpellFileCheckColumns();
		}
	}

	if(checked)
		logSrv->add(tr("Alternative tree symbols are displayed in the song tree now."), QU::Information);
	else
		logSrv->add(tr("Normal tree symbols are displayed in the song tree now."), QU::Information);
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
		this->setWindowFlags(this->windowFlags() & uint(!Qt::WindowStaysOnTopHint));

	this->show();
}

/*!
 * Maximize window and hide decorations.
 */
void QUMainWindow::toggleFullScreenMode() {
	setWindowState(windowState() ^ Qt::WindowFullScreen);
}

/*!
 * Applies the default action for the current file.
 * Text files (main song file as well as friends) will be displayed in a new window.
 * Image files will be shown in a preview window.
 * Audio and video files will be opened externally.
 */
void QUMainWindow::applyDefaultAction(QTreeWidgetItem *item, int column) {
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
		// swap song with friend
//		songDB->swapSongWithFriend(songItem->song(), item->text(FOLDER_COLUMN));
//		updateDetails();
	} else if(QUSongSupport::allowedImageFiles().contains(fileScheme, Qt::CaseInsensitive)) {
		QUPictureDialog dlg(QFileInfo(songItem->song()->songFileInfo().dir(), songItem->text(FOLDER_COLUMN)).filePath(), this);
		dlg.exec();
	} else if(QUSongSupport::allowedAudioFiles().contains(fileScheme, Qt::CaseInsensitive)
		|| QUSongSupport::allowedVideoFiles().contains(fileScheme, Qt::CaseInsensitive)
		|| QUSongSupport::allowedKaraokeFiles().contains(fileScheme, Qt::CaseInsensitive)
		|| QUSongSupport::allowedMidiFiles().contains(fileScheme, Qt::CaseInsensitive)
		|| QUSongSupport::allowedScoreFiles().contains(fileScheme, Qt::CaseInsensitive)) {
		QFileInfo fi(songItem->song()->path(), songItem->text(FOLDER_COLUMN));

		if( !QDesktopServices::openUrl(QUrl::fromLocalFile(fi.filePath())) )
			logSrv->add(QString(tr("Could NOT open file: \"%1\".")).arg(fi.filePath()), QU::Warning);
		else
			logSrv->add(QString(tr("File was opened successfully: \"%1\".")).arg(fi.filePath()), QU::Information);
	}
}

/*!
 * Opens the current file externally
 */
void QUMainWindow::openExternally(QTreeWidgetItem *item) {
	QUSongItem *songItem = dynamic_cast<QUSongItem*>(item);

	if(!songItem || songItem->isToplevel())
		return;

	QFileInfo fi(songItem->song()->path(), songItem->text(FOLDER_COLUMN));

	if( !QDesktopServices::openUrl(QUrl::fromLocalFile(fi.filePath())) )
		logSrv->add(QString(tr("Could NOT open file: \"%1\".")).arg(fi.filePath()), QU::Warning);
	else
		logSrv->add(QString(tr("File was opened successfully: \"%1\".")).arg(fi.filePath()), QU::Information);
}

/*!
 * Opens the current file internally
 */
void QUMainWindow::openInternally(QTreeWidgetItem *item) {
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
	} else if(QUSongSupport::allowedImageFiles().contains(fileScheme, Qt::CaseInsensitive)) {
		QUPictureDialog dlg(QFileInfo(songItem->song()->songFileInfo().dir(), songItem->text(FOLDER_COLUMN)).filePath(), this);
		dlg.exec();
	}
}

/*!
 * Sets the current text file as the primary song file (if it is a friend)
 */
void QUMainWindow::setPrimary(QTreeWidgetItem *item) {
	QUSongItem *songItem = dynamic_cast<QUSongItem*>(item);

	if(!songItem || songItem->isToplevel())
		return;

	QString fileScheme("*." + QFileInfo(songItem->text(FOLDER_COLUMN)).suffix());

	if(songItem->song()->isFriend(item->text(FOLDER_COLUMN))) {
		songDB->swapSongWithFriend(songItem->song(), item->text(FOLDER_COLUMN));
		updateDetails();
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
	QUReportDialog *dlg = new QUReportDialog(songDB->songs(), songTree->visibleSongs(), playlistDB->playlists(), this);

	dlg->exec();

	delete dlg;
}

/*!
 * Changes the application language to English.
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
 * Changes the application language to German.
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
 * Changes the application language to Polish.
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

/*!
 * Changes the application language to French.
 */
void QUMainWindow::enableFrench() {
	_menu->langFrBtn->setChecked(true);

	QSettings settings;
	settings.setValue("language", QLocale(QLocale::French, QLocale::France).name());

	// ---------------

	int result = QUMessageBox::information(this,
			tr("Change Language"),
			tr("Application language changed to <b>French</b>. You need to restart UltraStar Manager to take effect."),
			BTN << ":/control/quit.png" << tr("Quit UltraStar Manager.")
				<< ":/marks/accept.png" << tr("Continue."));
	if(result == 0)
		this->close();
}

/*!
 * Changes the application language to Spanish.
 */
void QUMainWindow::enableSpanish() {
	_menu->langEsBtn->setChecked(true);

	QSettings settings;
	settings.setValue("language", QLocale(QLocale::Spanish, QLocale::Spain).name());

	// ---------------

	int result = QUMessageBox::information(this,
			tr("Change Language"),
			tr("Application language changed to <b>Spanish</b>. You need to restart UltraStar Manager to take effect."),
			BTN << ":/control/quit.png" << tr("Quit UltraStar Manager.")
				<< ":/marks/accept.png" << tr("Continue."));
	if(result == 0)
		this->close();
}

/*!
 * Changes the application language to Portuguese.
 */
void QUMainWindow::enablePortuguese() {
	_menu->langPtBtn->setChecked(true);

	QSettings settings;
	settings.setValue("language", QLocale(QLocale::Portuguese, QLocale::Portugal).name());

	// ---------------

	int result = QUMessageBox::information(this,
			tr("Change Language"),
			tr("Application language changed to <b>Portuguese</b>. You need to restart UltraStar Manager to take effect."),
			BTN << ":/control/quit.png" << tr("Quit UltraStar Manager.")
				<< ":/marks/accept.png" << tr("Continue."));
	if(result == 0)
		this->close();
}

void QUMainWindow::getCovers(QList<QUSongItem*> items) {
	QURemoteImageDialog *dlg = new QURemoteImageDialog(items, this);

	if(dlg->exec()) {
		updateDetails();
		montyTalk();
	}

	disconnect(dlg, 0, 0, 0);
	delete dlg;
}

void QUMainWindow::getCovers() {
	QURemoteImageDialog(songTree->selectedSongItems(), this).exec();
	updateDetails();
	montyTalk();
}

///*!
// * Do everything to get back to a consistent state with the file system.
// */
//void QUMainWindow::processExternalSongFileChange(QUSongFile *song) {
//	if(song->hasUnsavedChanges()) {
//		logSrv->add(QString(tr("INCONSISTENT STATE! The song \"%1 - %2\" has unsaved changes and its persistent song file \"%3\" was modified externally. Save your changes or rebuild the tree manually.")).arg(song->artist()).arg(song->title()).arg(song->songFileInfo().filePath()), QU::Warning);
//		return;
//	}
//
//	foreach(QUSongItem *songItem, songTree->allSongItems()) {
//		if(songItem->song() == song or songItem->song()->isFriend(song)) {
//			song->updateCache();
//			songItem->update();
//			songTree->setCurrentItem(songItem);
//
//			updateDetails();
//			break;
//		}
//	}
//
//	logSrv->add(QString(tr("Song file changed: \"%1\"")).arg(QDir::toNativeSeparators(song->songFileInfo().filePath())), QU::Information);
//}

/*!
 * Select a path and copy all audio files of all songs to that path. (no public feature)
 */
void QUMainWindow::copyAudioToPath() {
	QString target = QFileDialog::getExistingDirectory(this, tr("Choose backup destination..."));

	if(target.isEmpty())
		return;

	QUProgressDialog dlg(tr("Backup audio files..."), songDB->songCount(), this);
	dlg.setPixmap(":/types/music.png");
	dlg.show();

	foreach(QUSongFile *song, songDB->songs()) {
		if(!song->hasMp3())
			continue;

		dlg.update(song->mp3FileInfo().fileName());
		if(dlg.cancelled()) break;

		// TODO: copy files
		QFile::copy(song->mp3FileInfo().filePath(), QFileInfo(QDir(target), song->mp3FileInfo().fileName()).filePath());
	}

	logSrv->add(tr("Backup for audio files finished."), QU::Information);
}

/*!
 * This is used to necessary information to the media player so that a song can be played.
 */
void QUMainWindow::sendSelectedSongsToMediaPlayer() {
	mediaplayer->stop();
	mediaplayer->setSongs(songTree->selectedSongs(), QUMediaPlayer::SelectedSongs);
	mediaplayer->play();
}

void QUMainWindow::sendAllSongsToMediaPlayer() {
	mediaplayer->setSongs(songDB->songs());
	mediaplayer->play();
}

void QUMainWindow::sendVisibleSongsToMediaPlayer() {
	mediaplayer->setSongs(songTree->visibleSongs());
	mediaplayer->play();
}

void QUMainWindow::sendCurrentPlaylistToMediaPlayer() {
	if(playlistDB->currentPlaylist()) {
		mediaplayer->setSongs(playlistDB->currentPlaylist()->connectedSongs());
		mediaplayer->play();
	}
}

/*!
 * Displays a dialog to manage plugins generally.
 */
void QUMainWindow::showPluginDialog() {
	QUPluginDialog(this).exec();
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
	_menu->updateBaseDirMenu();
}

void QUMainWindow::setDefaultEncodings() {
	QString defaultInputEncoding = QUSongSupport::defaultInputEncoding();
	QString defaultOutputEncoding = QUSongSupport::defaultOutputEncoding();
	QUEncodingsDialog *dlg = new QUEncodingsDialog(this);

	if(dlg->exec()) {
		if (defaultInputEncoding != QUSongSupport::defaultInputEncoding()) {
			this->refreshAllSongs(true);
			detailsTable->reset();

			logSrv->add(QString(tr("Default input encoding changed to \"%1\".")).arg(QUSongSupport::defaultInputEncoding()), QU::Information);
		}
		if (defaultOutputEncoding != QUSongSupport::defaultOutputEncoding()) {
			logSrv->add(QString(tr("Default output encoding changed to \"%1\".")).arg(QUSongSupport::defaultOutputEncoding()), QU::Information);
		}

		delete dlg;
	}

	montyTalk();
}

void QUMainWindow::setMediumMp3Quality(QString quality) {
	QSettings settings;
	settings.setValue("mediumMp3Quality", quality.split(" ").first().toInt());

	// ensure that highMp3Quality is at least one level higher
	if(_menu->highMp3QualityComboBox->currentIndex() <= _menu->highMp3QualityComboBox->findText(quality)) {
		disconnect(_menu->highMp3QualityComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(setHighMp3Quality(QString)));
		_menu->highMp3QualityComboBox->setCurrentIndex(_menu->highMp3QualityComboBox->findText(quality) + 1);
		connect(_menu->highMp3QualityComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(setHighMp3Quality(QString)));
		settings.setValue("highMp3Quality", _menu->highMp3QualityComboBox->currentText().split(" ").first().toInt());
	}

	QUProgressDialog dlg(tr("Updating audio quality icons..."), songTree->topLevelItemCount(), this, false);
	dlg.setPixmap(":/types/folder.png");
	dlg.show();

	for(int i = 0; i < songTree->topLevelItemCount(); ++i) {
		QUSongItem *songItem = dynamic_cast<QUSongItem*>(songTree->topLevelItem(i));

		if(songItem) {
			dlg.update(songItem->song()->songFileInfo().dir().dirName());
			songItem->updateSpellFileCheckColumns();
		}
	}
}

void QUMainWindow::setHighMp3Quality(QString quality) {
	QSettings settings;
	settings.setValue("highMp3Quality", quality.split(" ").first().toInt());

	// ensure that mediumMp3Quality is at least one level lower
	if(_menu->mediumMp3QualityComboBox->findText(quality) != -1) {
		if(_menu->mediumMp3QualityComboBox->currentIndex() >= _menu->mediumMp3QualityComboBox->findText(quality)) {
			disconnect(_menu->mediumMp3QualityComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(setMediumMp3Quality(QString)));
			_menu->mediumMp3QualityComboBox->setCurrentIndex(_menu->mediumMp3QualityComboBox->findText(quality) - 1);
			connect(_menu->mediumMp3QualityComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(setMediumMp3Quality(QString)));
			settings.setValue("mediumMp3Quality", _menu->mediumMp3QualityComboBox->currentText().split(" ").first().toInt());
		}
	}


	QUProgressDialog dlg(tr("Updating audio quality icons..."), songTree->topLevelItemCount(), this, false);
	dlg.setPixmap(":/types/folder.png");
	dlg.show();

	for(int i = 0; i < songTree->topLevelItemCount(); ++i) {
		QUSongItem *songItem = dynamic_cast<QUSongItem*>(songTree->topLevelItem(i));

		if(songItem) {
			dlg.update(songItem->song()->songFileInfo().dir().dirName());
			songItem->updateSpellFileCheckColumns();
		}
	}
}

void QUMainWindow::setMediumCoverQuality(QString quality) {
	QSettings settings;
	settings.setValue("mediumCoverQuality", quality);

	// ensure that highCoverQuality is at least one level higher
	if(_menu->highCoverQualityComboBox->currentIndex() <= _menu->highCoverQualityComboBox->findText(quality)) {
		disconnect(_menu->highCoverQualityComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(setHighCoverQuality(QString)));
		_menu->highCoverQualityComboBox->setCurrentIndex(_menu->highCoverQualityComboBox->findText(quality) + 1);
		connect(_menu->highCoverQualityComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(setHighCoverQuality(QString)));
		settings.setValue("highCoverQuality", _menu->highCoverQualityComboBox->currentText().split(" ").first().toInt());
	}

	QUProgressDialog dlg(tr("Updating cover quality icons..."), songTree->topLevelItemCount(), this, false);
	dlg.setPixmap(":/types/folder.png");
	dlg.show();

	for(int i = 0; i < songTree->topLevelItemCount(); ++i) {
		QUSongItem *songItem = dynamic_cast<QUSongItem*>(songTree->topLevelItem(i));

		if(songItem) {
			dlg.update(songItem->song()->songFileInfo().dir().dirName());
			songItem->updateSpellFileCheckColumns();
		}
	}
}

void QUMainWindow::setHighCoverQuality(QString quality) {
	QSettings settings;
	settings.setValue("highCoverQuality", quality);

	// ensure that mediumCoverQuality is at least one level lower
	if(_menu->mediumCoverQualityComboBox->findText(quality) != -1) {
		if(_menu->mediumCoverQualityComboBox->currentIndex() >= _menu->mediumCoverQualityComboBox->findText(quality)) {
			disconnect(_menu->mediumCoverQualityComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(setMediumCoverQuality(QString)));
			_menu->mediumCoverQualityComboBox->setCurrentIndex(_menu->mediumCoverQualityComboBox->findText(quality) - 1);
			connect(_menu->mediumCoverQualityComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(setMediumCoverQuality(QString)));
			settings.setValue("mediumCoverQuality", _menu->mediumCoverQualityComboBox->currentText().split(" ").first().toInt());
		}
	}

	QUProgressDialog dlg(tr("Updating cover quality icons..."), songTree->topLevelItemCount(), this, false);
	dlg.setPixmap(":/types/folder.png");
	dlg.show();

	for(int i = 0; i < songTree->topLevelItemCount(); ++i) {
		QUSongItem *songItem = dynamic_cast<QUSongItem*>(songTree->topLevelItem(i));

		if(songItem) {
			dlg.update(songItem->song()->songFileInfo().dir().dirName());
			songItem->updateSpellFileCheckColumns();
		}
	}
}

void QUMainWindow::setMediumBackgroundQuality(QString quality) {
	QSettings settings;
	settings.setValue("mediumBackgroundQuality", quality);

	// ensure that highBackgroundQuality is at least one level higher
	if(_menu->highBackgroundQualityComboBox->currentIndex() <= _menu->highBackgroundQualityComboBox->findText(quality)) {
		disconnect(_menu->highBackgroundQualityComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(setHighBackgroundQuality(QString)));
		_menu->highBackgroundQualityComboBox->setCurrentIndex(_menu->highBackgroundQualityComboBox->findText(quality) + 1);
		connect(_menu->highBackgroundQualityComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(setHighBackgroundQuality(QString)));
		settings.setValue("highBackgroundQuality", _menu->highBackgroundQualityComboBox->currentText().split(" ").first().toInt());
	}

	QUProgressDialog dlg(tr("Updating background quality icons..."), songTree->topLevelItemCount(), this, false);
	dlg.setPixmap(":/types/folder.png");
	dlg.show();

	for(int i = 0; i < songTree->topLevelItemCount(); ++i) {
		QUSongItem *songItem = dynamic_cast<QUSongItem*>(songTree->topLevelItem(i));

		if(songItem) {
			dlg.update(songItem->song()->songFileInfo().dir().dirName());
			songItem->updateSpellFileCheckColumns();
		}
	}
}

void QUMainWindow::setHighBackgroundQuality(QString quality) {
	QSettings settings;
	settings.setValue("highBackgroundQuality", quality);

	// ensure that mediumBackgroundQuality is at least one level lower
	if(_menu->mediumBackgroundQualityComboBox->findText(quality) != -1) {
		if(_menu->mediumBackgroundQualityComboBox->currentIndex() >= _menu->mediumBackgroundQualityComboBox->findText(quality)) {
			disconnect(_menu->mediumBackgroundQualityComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(setMediumBackgroundQuality(QString)));
			_menu->mediumBackgroundQualityComboBox->setCurrentIndex(_menu->mediumBackgroundQualityComboBox->findText(quality) - 1);
			connect(_menu->mediumBackgroundQualityComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(setMediumBackgroundQuality(QString)));
			settings.setValue("mediumBackgroundQuality", _menu->mediumBackgroundQualityComboBox->currentText().split(" ").first().toInt());
		}
	}

	QUProgressDialog dlg(tr("Updating background quality icons..."), songTree->topLevelItemCount(), this, false);
	dlg.setPixmap(":/types/folder.png");
	dlg.show();

	for(int i = 0; i < songTree->topLevelItemCount(); ++i) {
		QUSongItem *songItem = dynamic_cast<QUSongItem*>(songTree->topLevelItem(i));

		if(songItem) {
			dlg.update(songItem->song()->songFileInfo().dir().dirName());
			songItem->updateSpellFileCheckColumns();
		}
	}
}

void QUMainWindow::clearStatusMessage() {
	_statusIconLbl->hide();
	_statusMessageLbl->hide();
}
