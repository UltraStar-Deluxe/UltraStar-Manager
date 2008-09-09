#include "QUSongTree.h"
#include "QUSongItem.h"
#include "QUSongFile.h"
#include "QUColumnAction.h"
#include "QUMainWindow.h"
#include "QUMessageBox.h"

#include "QUMetaphoneString.h"

#include <QMessageBox>
#include <QUrl>
#include <QIcon>
#include <QMenu>
#include <QList>
#include <QRegExp>
#include <QByteArray>
#include <QAction>
#include <QHeaderView>
#include <QCursor>
#include <QTime>

#include "QUProgressDialog.h"

#define FILE_DROP_LIMIT 5

QUSongTree::QUSongTree(QWidget *parent): QTreeWidget(parent) {
	this->setAcceptDrops(true);

	// enable context menu for items
	this->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showItemMenu(const QPoint&)));

	// enable context menu for header
	this->header()->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this->header(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showHeaderMenu(const QPoint&)));
}

QUMainWindow* QUSongTree::parentWindow() const {
	return dynamic_cast<QUMainWindow*>(parentWidget());
}

void QUSongTree::initHorizontalHeader() {
	this->setColumnCount(FIXED_COLUMN_COUNT + QUSongFile::customTags().count());

	QTreeWidgetItem *header = new QTreeWidgetItem();
	header->setText(FOLDER_COLUMN, QString(tr("Folder (%1)")).arg(QUMainWindow::BaseDir.path()));
	header->setIcon(FOLDER_COLUMN, QIcon(":/types/folder.png"));

	header->setIcon(ARTIST_COLUMN, QIcon(":/types/user.png"));
	header->setToolTip(ARTIST_COLUMN, tr("Shows whether your folder includes the artist correctly:<br><i>Artist - Title ...</i>"));
	header->setIcon(TITLE_COLUMN, QIcon(":/types/font.png"));
	header->setToolTip(TITLE_COLUMN, tr("Shows whether your folder includes the title correctly:<br><i>Artist - Title ...</i>"));

	header->setIcon(MP3_COLUMN, QIcon(":/types/music.png"));
	header->setToolTip(MP3_COLUMN, tr("Shows whether the song text file points to an <b>audio file</b> that can be found by UltraStar"));
	header->setIcon(COVER_COLUMN, QIcon(":/types/cover.png"));
	header->setToolTip(COVER_COLUMN, tr("Shows whether the song text file points to a <b>cover picture</b> that can be found by UltraStar"));
	header->setIcon(BACKGROUND_COLUMN, QIcon(":/types/background.png"));
	header->setToolTip(BACKGROUND_COLUMN, tr("Shows whether the song text file points to a <b>background picture</b> that can be found by UltraStar"));
	header->setIcon(VIDEO_COLUMN, QIcon(":/types/film.png"));
	header->setToolTip(VIDEO_COLUMN, tr("Shows whether the song text file points to a <b>video file</b> that can be found by UltraStar"));

	header->setIcon(UNUSED_FILES_COLUMN, QIcon(":/types/unused_files.png"));
	header->setToolTip(UNUSED_FILES_COLUMN, tr("Shows whether your folder contains unused files."));
	header->setIcon(MULTIPLE_SONGS_COLUMN, QIcon(":/types/text_stack.png"));
	header->setToolTip(MULTIPLE_SONGS_COLUMN, tr("Shows whether your folder contains more than one song text file."));

	header->setText(ARTIST_COLUMN_EX, tr("Artist"));
	header->setIcon(ARTIST_COLUMN_EX, QIcon(":/types/user.png"));
	header->setText(TITLE_COLUMN_EX, tr("Title"));
	header->setIcon(TITLE_COLUMN_EX, QIcon(":/types/font.png"));

	header->setText(LANGUAGE_COLUMN, tr("Language"));
	header->setIcon(LANGUAGE_COLUMN, QIcon(":/types/language.png"));
	header->setText(EDITION_COLUMN, tr("Edition"));
	header->setIcon(EDITION_COLUMN, QIcon(":/types/edition.png"));
	header->setText(GENRE_COLUMN, tr("Genre"));
	header->setIcon(GENRE_COLUMN, QIcon(":/types/genre.png"));
	header->setText(YEAR_COLUMN, tr("Year"));
	header->setIcon(YEAR_COLUMN, QIcon(":/types/date.png"));
	header->setText(CREATOR_COLUMN, tr("Creator"));
	header->setIcon(CREATOR_COLUMN, QIcon(":/types/creator.png"));

	header->setText(LENGTH_COLUMN, tr("Song"));
	header->setIcon(LENGTH_COLUMN, QIcon(":/types/time_song.png"));
	header->setToolTip(LENGTH_COLUMN, tr("Song length calculated from BPM and lyrics."));

	header->setIcon(LENGTH_DIFF_COLUMN, QIcon(":/types/time_warning.png"));
	header->setToolTip(LENGTH_DIFF_COLUMN, tr("Indicates a problem with the difference of <i>song length</i> and <i>audio length</i>.<br><br>The tooltip shows the difference."));

	header->setText(LENGTH_MP3_COLUMN, tr("Audio"));
	header->setIcon(LENGTH_MP3_COLUMN, QIcon(":/types/time_mp3.png"));
	header->setToolTip(LENGTH_MP3_COLUMN, tr("Shows length of audio file, if present.<br><br>Can be reset through <b>#END</b> tag."));

	header->setText(LENGTH_EFF_COLUMN, tr("Total"));
	header->setIcon(LENGTH_EFF_COLUMN, QIcon(":/types/time_eff.png"));
	header->setToolTip(LENGTH_EFF_COLUMN, tr("Shows the effective length:<br><b>audio length - #START</b>."));

	header->setText(START_COLUMN, tr("Start"));
	header->setIcon(START_COLUMN, QIcon(":/types/start.png"));
	header->setToolTip(START_COLUMN, tr("Skips the first seconds of the song."));
	header->setText(END_COLUMN, tr("End"));
	header->setIcon(END_COLUMN, QIcon(":/types/end.png"));
	header->setToolTip(END_COLUMN, tr("Resets the length of the audio file.<br><i>(in milliseconds)</i>"));
	header->setText(VIDEOGAP_COLUMN, tr("Videogap"));
	header->setIcon(VIDEOGAP_COLUMN, QIcon(":/types/videogap.png"));
	header->setToolTip(VIDEOGAP_COLUMN, tr("Skips the first seconds of the video.<br><br><i>Use negative values here with positive ones in <b>#START</b> to fix a short video file.</i>"));

//	header->setText(DUPLICATE_ID_COLUMN, "ID");
	header->setToolTip(DUPLICATE_ID_COLUMN, tr("Indicate duplicate songs. <b>You should not see me.</b>"));

	int i = 0;
	foreach(QString customTag, QUSongFile::customTags()) {
		header->setText(FIRST_CUSTOM_TAG_COLUMN + i, customTag);
		header->setIcon(FIRST_CUSTOM_TAG_COLUMN + i, QIcon(":/bullets/bullet_star.png"));
		i++;
	}

	// set up default header
	this->setHeaderItem(header);
	this->header()->setSectionHidden(ARTIST_COLUMN_EX, true);
	this->header()->setSectionHidden(TITLE_COLUMN_EX, true);
	this->header()->setSectionHidden(LENGTH_COLUMN, true);
	this->header()->setSectionHidden(LENGTH_DIFF_COLUMN, true);
	this->header()->setSectionHidden(LENGTH_MP3_COLUMN, true);
	this->header()->setSectionHidden(LENGTH_EFF_COLUMN, true);
	this->header()->setSectionHidden(START_COLUMN, true);
	this->header()->setSectionHidden(END_COLUMN, true);
	this->header()->setSectionHidden(VIDEOGAP_COLUMN, true);

	this->header()->setSectionHidden(DUPLICATE_ID_COLUMN, true);

	// load custom setup
	QSettings settings;
	this->header()->restoreState(settings.value("songTreeState", QVariant()).toByteArray());
}

bool QUSongTree::hasUnsavedChanges() const {
	bool result = false;

	for(int i = 0; i < this->topLevelItemCount(); i++) {
		QUSongItem *songItem = dynamic_cast<QUSongItem*>(this->topLevelItem(i));

		if(songItem)
			result |= songItem->song()->hasUnsavedChanges();
	}

	return result;
}

/*!
 * Overloaded to clear hidden items.
 */
void QUSongTree::clear() {
	qDeleteAll(_hiddenItems);
	_hiddenItems.clear();
	QTreeWidget::clear();
}

/*!
 * \returns All selected songs in the song tree.
 */
QList<QUSongFile*> QUSongTree::selectedSongs() {
	QList<QUSongFile*> songs;

	if(this->selectedItems().isEmpty()) { // nothing selected? Try to use the current item...
		QUSongItem *songItem = dynamic_cast<QUSongItem*>(this->currentItem());
		if(songItem)
			songs.append(songItem->song());
	} else { // look for all songs in the selection
		foreach(QTreeWidgetItem *item, this->selectedItems()) {
			QUSongItem *songItem = dynamic_cast<QUSongItem*>(item);
			if(songItem)
				songs.append(songItem->song());
		}
	}
	// TODO: Refactor - melt with selectedSongItems()

	return songs;
}

/*!
 * \returns All selected song items or the current toplevel item if nothing was
 * selected.
 */
QList<QUSongItem*> QUSongTree::selectedSongItems() {
	QList<QUSongItem*> items;

	if(this->selectedItems().isEmpty()) { // nothing selected? Try to use the current item...
		QUSongItem *cItem = dynamic_cast<QUSongItem*>(this->currentItem());
		if(cItem) {
			if(cItem->isToplevel())
				items.append(cItem);
			else {
				QUSongItem *pItem = dynamic_cast<QUSongItem*>(cItem->parent());
				if(pItem) items.append(pItem); // just add toplevel items
			}
		}
	} else { // look for all songs in the selection
		foreach(QTreeWidgetItem *item, this->selectedItems()) {
			QUSongItem *songItem = dynamic_cast<QUSongItem*>(item);
			if(songItem)
				items.append(songItem);
		}
	}

	return items;
}

QList<QUSongFile*> QUSongTree::visibleSongs() {
	QList<QUSongFile*> result;

	foreach(QUSongItem *songItem, visibleSongItems())
		result.append(songItem->song());

	return result;
}

/*!
 * \returns a list of all visible song items
 */
QList<QUSongItem*> QUSongTree::visibleSongItems() {
	QList<QUSongItem*> items;

	QList<QTreeWidgetItem*> topLevelItems;
	for(int i = 0; i < this->topLevelItemCount(); i++)
		topLevelItems.append(this->topLevelItem(i));

	foreach(QTreeWidgetItem *item, topLevelItems) {
		QUSongItem *songItem = dynamic_cast<QUSongItem*>(item);

		if(songItem)
			items.append(songItem);
	}

	return items;
}

/*!
 * \returns a list of all available song items in the tree. That items
 * can be visible or hidden.
 */
QList<QUSongItem*> QUSongTree::allSongItems() {
	return (visibleSongItems() << _hiddenItems);
}

/*!
 * Creates a new entry for each given song.
 */
void QUSongTree::fill(QList<QUSongFile*> songs) {
	this->initHorizontalHeader();

	QUProgressDialog dlg(tr("Filling song tree..."), songs.size(), this, false);
	dlg.setPixmap(":/types/folder.png");
	dlg.show();

	foreach(QUSongFile* song, songs) {
		dlg.update(song->songFileInfo().dir().dirName());
		this->addTopLevelItem(new QUSongItem(song, true));
	}

	this->resizeToContents();
	this->sortItems(FOLDER_COLUMN, Qt::AscendingOrder);
}

void QUSongTree::saveUnsavedChanges() {
	QUProgressDialog dlg(tr("Saving unsaved changes..."), this->topLevelItemCount(), this);
	dlg.show();

	for(int i = 0; i < this->topLevelItemCount(); i++) {
		QUSongItem *songItem = dynamic_cast<QUSongItem*>(this->topLevelItem(i));

		if(songItem) {
			dlg.update(songItem->song()->songFileInfo().fileName());
			if(dlg.cancelled()) break;

			if(songItem->song()->hasUnsavedChanges()) {
				songItem->song()->save(true);
				songItem->update();
			}
		}
	}

	emit itemSelectionChanged(); // update details
}

/*!
 * Hides all items from the tree which do not match the filter. An empty string
 * will remove the filter and show all items again. Selected items will be
 * unselected if hidden.
 */
void QUSongTree::filterItems(const QString &regexp, QU::FilterModes mode) {
	this->removeFilter();

	if(regexp.isEmpty()) {
		emit finished(tr("Filter removed. All songs are visible now."), QU::information);
		emit itemSelectionChanged(); // update details
		return;
	}

	QRegExp rx(regexp, Qt::CaseInsensitive);

	// collect all toplevel items
	QList<QTreeWidgetItem*> topLevelItems;
	for(int i = 0; i < this->topLevelItemCount(); i++)
		topLevelItems.append(this->topLevelItem(i));

	QUProgressDialog progress(tr("Applying filter..."), topLevelItems.size(), this, false);
	progress.setPixmap(":/control/filter.png");
	progress.show();

	QList<QUSongItem*> selectedItems = this->selectedSongItems();
	clearSelection();

	foreach(QTreeWidgetItem *item, topLevelItems) {
		QUSongItem *songItem = dynamic_cast<QUSongItem*>(item);

		if(songItem) {
			QUSongFile *song = songItem->song();

			progress.update(QString("%1 - %2").arg(song->artist()).arg(song->title()));
			if(progress.cancelled()) break;

			bool filterInfo = true;
			bool filterFile = true;
			bool filterCtrl = true;
			bool filterCstm = true;

			if(mode.testFlag(QU::informationTags)) {
				bool matchesInfoTag =
					song->artist().contains(rx) ||
					song->title().contains(rx) ||
					song->genre().contains(rx) ||
					song->year().contains(rx) ||
					song->edition().contains(rx) ||
					song->creator().contains(rx) ||
					song->language().contains(rx);

				if(!matchesInfoTag)
					filterInfo = true;
				else
					filterInfo = false;
			}

			if(mode.testFlag(QU::fileTags)) {
				bool matchesFileTag =
					song->mp3().contains(rx) ||
					song->cover().contains(rx) ||
					song->background().contains(rx) ||
					song->video().contains(rx);

				if(!matchesFileTag)
					filterFile = true;
				else
					filterFile = false;
			}

			if(mode.testFlag(QU::controlTags)) {
				bool matchesControlTag =
					song->videogap().contains(rx) ||
					song->start().contains(rx) ||
					song->end().contains(rx) ||
					song->relative().contains(rx) ||
					song->bpm().contains(rx) ||
					song->gap().contains(rx);

				if(!matchesControlTag)
					filterCtrl = true;
				else
					filterCtrl = false;
			}

			if(mode.testFlag(QU::customTags)) {
				bool matchesCustomTag = false;

				foreach(QString customTag, QUSongFile::customTags()) {
					matchesCustomTag = matchesCustomTag || song->customTag(customTag).contains(rx);
				}

				if(!matchesCustomTag)
					filterCstm = true;
				else
					filterCstm = false;
			}

			if(
				(filterInfo && filterFile && filterCtrl && filterCstm && !(mode.testFlag(QU::negateFilter)))
				||
				(!(filterInfo && filterFile && filterCtrl && filterCstm) && (mode.testFlag(QU::negateFilter)))
			) {
				this->takeTopLevelItem(this->indexOfTopLevelItem(item));
				_hiddenItems.append(songItem);
			}
		}
	}

	restoreSelection(selectedItems);

//	emit itemSelectionChanged(); // update details
	emit finished(QString(tr("Filter applied: \"%1\"%2")).arg(regexp).arg(mode.testFlag(QU::negateFilter) ? tr(", negated") : ""), QU::information);
}

/*!
 * Show only songs which seem to be NOT unique. ^_^
 */
void QUSongTree::filterDuplicates() {
	this->hideAll();

	QList<QUSongItem*> allItems = _hiddenItems;
	QList<QUSongItem*> workingList = allItems;
	QList<QUSongItem*> uniqueList; // store unique items;

	int nextID = 0;
	QMap<QUSongItem*, int> id;

	// sort after an invisible column to combine duplicate items
	this->sortItems(FOLDER_COLUMN, Qt::AscendingOrder);
	this->sortItems(DUPLICATE_ID_COLUMN, Qt::AscendingOrder);

	QUProgressDialog dlg(tr("Looking for duplicate songs..."), allItems.size(), this);
//	dlg.setPixmap(":/control/eye.png"); --> inserte a duplicate picture? show which songs we combine?
	dlg.show();

	foreach(QUSongItem *item, allItems) {
		dlg.update(QString("%1 - %2").arg(item->song()->artist()).arg(item->song()->title()));
		if(dlg.cancelled()) break;

		bool isItemUnique = true;

		foreach(QUSongItem *workingItem, workingList) {
			if(item == workingItem)
				continue; // ignore the same item

			if(QUSongFile::equal(item->song(), workingItem->song())) {
				if(id.contains(item)) {
					id.insert(workingItem, id.value(item));
					workingItem->setText(DUPLICATE_ID_COLUMN, QString("%1").arg(id.value(item), 4, 10, QChar('0')));
				} else if(id.contains(workingItem)) {
					id.insert(item, id.value(workingItem));
					item->setText(DUPLICATE_ID_COLUMN, QString("%1").arg(id.value(workingItem), 4, 10, QChar('0')));
				} else {
					id.insert(item, nextID);
					id.insert(workingItem, nextID);

					item->setText(DUPLICATE_ID_COLUMN, QString("%1").arg(nextID, 4, 10, QChar('0')));
					workingItem->setText(DUPLICATE_ID_COLUMN, QString("%1").arg(nextID, 4, 10, QChar('0')));

					nextID++;
				}

				this->addTopLevelItem(item);
				_hiddenItems.removeAll(item);

				this->addTopLevelItem(workingItem);
				_hiddenItems.removeAll(workingItem);

				isItemUnique = false;
				break;
			}
		}

		if(isItemUnique) {
			workingList.removeAll(item);
			item->setText(DUPLICATE_ID_COLUMN, N_A);
		}
	}

	emit itemSelectionChanged();

	if(nextID > 0)
		emit finished(QString(tr("Filter applied. Duplicates for %1 songs found.")).arg(nextID), QU::information);
	else
		emit finished(QString(tr("No duplicate songs found.")), QU::information);
}

bool QUSongTree::dropMimeData (QTreeWidgetItem *parent, int index, const QMimeData *data, Qt::DropAction action) {
	if(data->urls().isEmpty())
		return false;

	if(!parent) {
		dropSongFiles(data->urls());
		return true;
	}

	QUSongItem *item = dynamic_cast<QUSongItem*>(parent);

	if(!item)
		return false;

	return this->copyFilesToSong(data->urls(), item);
}

QStringList QUSongTree::mimeTypes() const {
	return QStringList("text/uri-list");
}

/*!
 * Appends actions to the menu according to the selected item/file.
 */
void QUSongTree::showItemMenu(const QPoint &point) {
	if(!this->itemAt(point))
		return; // no item clicked

	QMenu menu(this);

	QUSongItem *item = dynamic_cast<QUSongItem*>(this->currentItem());

	if(item && !item->isToplevel()) {
		// file menu
		menu.addAction(tr("Open"), this, SLOT(openCurrentFile()));
		menu.addAction(QIcon(":/control/bin.png"), tr("Delete"), this, SLOT(deleteCurrentItem()), Qt::Key_Delete);
	} else {
		// song/folder menu
		menu.addAction(QIcon(":/control/refresh.png"), tr("Refresh"), this, SLOT(refreshSelectedItems()),       Qt::Key_F5);
		menu.addAction(QIcon(":/control/save.png"),    tr("Save"),    this, SLOT(saveSelectedSongs()),          Qt::CTRL  + Qt::Key_S);
		menu.addAction(QIcon(":/control/bin.png"),     tr("Delete"),  this, SLOT(requestDeleteSelectedSongs()), Qt::SHIFT + Qt::Key_Delete);
		menu.addAction(                                tr("Merge"),   this, SLOT(mergeSelectedSongs()),         Qt::CTRL  + Qt::Key_M);

		menu.addSeparator();
		menu.addAction(QIcon(":/control/playlist_to.png"), tr("Send To Playlist"), this, SLOT(sendSelectedSongsToPlaylist()), QKeySequence::fromString("Ctrl+P"));
		menu.addAction(tr("Get Covers From Amazon..."), this, SLOT(requestCoversFromAmazon()));

		QMenu *filterMenu = menu.addMenu(tr("Hide"));
		filterMenu->setIcon(QIcon(":/control/eye.png"));
		filterMenu->addAction(tr("Selected Songs"), this, SLOT(hideSelected()));
		filterMenu->addAction(tr("Selected Songs Only"), this, SLOT(hideSelectedOnly()));
		filterMenu->addAction(tr("Unselected Songs"), this, SLOT(hideAllButSelected()));
		filterMenu->addSeparator();
		filterMenu->addAction(tr("All"), this, SLOT(hideAll()));

		menu.addSeparator();
		menu.addAction(tr("Show Lyrics..."), this, SLOT(requestLyrics()), Qt::CTRL + Qt::Key_L);
	}

	menu.exec(QCursor::pos());
}

/*!
 * Enables the user to show/hide various columns.
 */
void QUSongTree::showHeaderMenu(const QPoint &point) {
	QMenu menu(this);

	QMenu presetsMenu(tr("Presets")); presetsMenu.setIcon(QIcon(":/control/tree_presets.png"));
	presetsMenu.addAction(tr("All"), this, SLOT(showAllColumns()));
	presetsMenu.addAction(tr("Default"), this, SLOT(showDefaultColumns()));
	presetsMenu.addAction(tr("Minimum"), this, SLOT(showMinimalColumns()));
	presetsMenu.addSeparator();
	presetsMenu.addAction(tr("Spell && File Checking"), this, SLOT(showCheckColumns()));
	presetsMenu.addAction(tr("Time Comparison"), this, SLOT(showTimeColumns()));
	menu.addMenu(&presetsMenu);
	menu.addSeparator();

	QMenu customTagsMenu(tr("Custom Tags"), this); customTagsMenu.setIcon(QIcon(":/bullets/bullet_star.png"));
	QMenu lengthsMenu(tr("Lengths"), this); lengthsMenu.setIcon(QIcon(":/types/time.png"));

	for(int i = 0; i < headerItem()->columnCount(); i++) {
		if(i != LENGTH_DIFF_COLUMN and (headerItem()->text(i).isEmpty() or i == FOLDER_COLUMN))
			continue;

		QUColumnAction *a = new QUColumnAction(headerItem()->text(i), i); // save the logical index of this column
		a->setChecked(!header()->isSectionHidden(i));

		connect(a, SIGNAL(columnToggled(bool, int)), this, SLOT(toggleColumn(bool, int)));

		switch(i) {
		case START_COLUMN: lengthsMenu.addSeparator();
		case LENGTH_COLUMN:
		case LENGTH_DIFF_COLUMN:
		case LENGTH_MP3_COLUMN:
		case LENGTH_EFF_COLUMN:
		case END_COLUMN:
		case VIDEOGAP_COLUMN: lengthsMenu.addAction(a); break;
		default:
			if(QUSongFile::customTags().contains(a->text(), Qt::CaseInsensitive))
				customTagsMenu.addAction(a);
			else
				menu.addAction(a);
		}
	}

	menu.addSeparator();
	menu.addMenu(&lengthsMenu);
	menu.addMenu(&customTagsMenu);

	menu.exec(QCursor::pos());
}

void QUSongTree::toggleColumn(bool show, int index) {
	this->header()->setSectionHidden(index, !show);
	this->resizeToContents();

	QSettings settings;
	settings.setValue("songTreeState", QVariant(header()->saveState()));
}

void QUSongTree::showAllColumns() {
	for(int i = 0; i < headerItem()->columnCount(); i++)
		header()->showSection(i);

	this->header()->setSectionHidden(DUPLICATE_ID_COLUMN, true);

	this->resizeToContents();

	QSettings settings;
	settings.setValue("songTreeState", QVariant(header()->saveState()));
}

void QUSongTree::showDefaultColumns() {
	for(int i = 0; i < headerItem()->columnCount(); i++)
		header()->showSection(i);

	this->header()->setSectionHidden(ARTIST_COLUMN_EX, true);
	this->header()->setSectionHidden(TITLE_COLUMN_EX, true);
	this->header()->setSectionHidden(LENGTH_COLUMN, true);
	this->header()->setSectionHidden(LENGTH_DIFF_COLUMN, true);
	this->header()->setSectionHidden(LENGTH_MP3_COLUMN, true);
	this->header()->setSectionHidden(LENGTH_EFF_COLUMN, true);
	this->header()->setSectionHidden(START_COLUMN, true);
	this->header()->setSectionHidden(END_COLUMN, true);
	this->header()->setSectionHidden(VIDEOGAP_COLUMN, true);

	this->header()->setSectionHidden(DUPLICATE_ID_COLUMN, true);

	this->resizeToContents();

	QSettings settings;
	settings.setValue("songTreeState", QVariant(header()->saveState()));
}

/*!
 * Hide all but the folder column.
 */
void QUSongTree::showMinimalColumns() {
	for(int i = 0; i < headerItem()->columnCount(); i++)
		header()->hideSection(i);

	this->header()->showSection(FOLDER_COLUMN);

	this->resizeToContents();

	QSettings settings;
	settings.setValue("songTreeState", QVariant(header()->saveState()));
}

void QUSongTree::showTimeColumns() {
	for(int i = 0; i < headerItem()->columnCount(); i++)
		header()->hideSection(i);

	this->header()->showSection(FOLDER_COLUMN);
	this->header()->showSection(LENGTH_COLUMN);
	this->header()->showSection(LENGTH_DIFF_COLUMN);
	this->header()->showSection(LENGTH_MP3_COLUMN);
	this->header()->showSection(LENGTH_EFF_COLUMN);
	this->header()->showSection(START_COLUMN);
	this->header()->showSection(END_COLUMN);
	this->header()->showSection(VIDEOGAP_COLUMN);

	this->resizeToContents();

	QSettings settings;
	settings.setValue("songTreeState", QVariant(header()->saveState()));
}

void QUSongTree::showCheckColumns() {
	for(int i = 0; i < headerItem()->columnCount(); i++)
		header()->hideSection(i);

	this->header()->showSection(FOLDER_COLUMN);
	this->header()->showSection(ARTIST_COLUMN);
	this->header()->showSection(TITLE_COLUMN);
	this->header()->showSection(MP3_COLUMN);
	this->header()->showSection(COVER_COLUMN);
	this->header()->showSection(BACKGROUND_COLUMN);
	this->header()->showSection(VIDEO_COLUMN);
	this->header()->showSection(UNUSED_FILES_COLUMN);
	this->header()->showSection(MULTIPLE_SONGS_COLUMN);

	this->resizeToContents();

	QSettings settings;
	settings.setValue("songTreeState", QVariant(header()->saveState()));
}

/*!
 * Emits the proper signal to open the current file. (See QUMainWindow)
 */
void QUSongTree::openCurrentFile() {
	emit itemActivated(this->currentItem(), FOLDER_COLUMN);
}

/*!
 * Add all items to the invisible list.
 */
void QUSongTree::hideAll() {
	QList<QUSongItem*> topLevelItems = visibleSongItems();

	QUProgressDialog dlg(tr("Hiding all visible songs..."), topLevelItems.size(), this, false);
	dlg.setPixmap(":/control/eye.png");
	dlg.show();

	clearSelection();

	foreach(QUSongItem *item, topLevelItems) {
		dlg.update(item->song()->songFileInfo().dir().dirName());

		this->takeTopLevelItem(this->indexOfTopLevelItem(item));
		_hiddenItems.append(item);
	}

	emit itemSelectionChanged();
}

/*!
 * Add the selected Items to the hidden-list.
 */
void QUSongTree::hideSelected() {
	QList<QUSongItem*> selectedItems = this->selectedSongItems();

	if(selectedItems.isEmpty()) {
		emit finished(tr("Could not hide any item."), QU::warning);
		return;
	}

	QUProgressDialog dlg(tr("Hiding selected songs..."), selectedItems.size(), this, true);
	dlg.setPixmap(":/types/folder.png");
	dlg.show();

	clearSelection();

	foreach(QUSongItem *item, selectedItems) {
		dlg.update(item->song()->songFileInfo().dir().dirName());
		if(dlg.cancelled()) break;

		this->takeTopLevelItem(this->indexOfTopLevelItem(item));
		_hiddenItems.append(item);
	}

	emit itemSelectionChanged(); // update details
	emit finished(QString(tr("%1 songs added to invisible list.")).arg(selectedItems.count()), QU::information);
}

/*!
 * Show all hidden items. Then hide the selected ones.
 */
void QUSongTree::hideSelectedOnly() {
	this->removeFilter();
	this->hideSelected();
}

/*!
 * Hide all items but show the selected ones.
 */
void QUSongTree::hideAllButSelected() {
	QList<QUSongItem*> selectedItems = this->selectedSongItems();

	this->filterItems(".*", (QU::FilterModes) QU::informationTags | QU::negateFilter);

	foreach(QUSongItem *item, selectedItems) {
		this->addTopLevelItem(item);
		_hiddenItems.removeAll(item);
	}

	emit itemSelectionChanged(); // update details
	emit finished(QString(tr("%1 songs are visible now.")).arg(selectedItems.count()), QU::information);
}

/*!
 * Request the deletion for all selected songs.
 */
void QUSongTree::requestDeleteSelectedSongs() {
	QList<QUSongItem*> selectedItems = selectedSongItems();

	if(selectedItems.isEmpty())
		return;

	// ---------------------------------

	QString infoText = QString(tr("All files in the song directory will be deleted including the directory itself if it is not empty.<br><br>You want to delete the following <b>%1</b> songs:")).arg(selectedItems.size());

	int i = 0;
	foreach(QUSongItem *songItem, selectedItems) {
		if(++i >= 5) {
			infoText.append("<br>&nbsp;&nbsp;...");
			break;
		}

		infoText.append(QString("<br>&nbsp;&nbsp;<b>%1 - %2</b>").arg(songItem->song()->artist()).arg(songItem->song()->title()));
	}

	QUMessageBox::Results result = QUMessageBox::ask(this,
			tr("Delete Songs"),
			infoText,
			":/control/bin.png", tr("Delete these songs."),
			":/marks/cancel.png", tr("Cancel delete operation."),
			"", "",
			50);
	if(result == QUMessageBox::second)
		return;

	// ----------------------------------

	QUProgressDialog dlg(tr("Deleting selected songs..."), selectedItems.size(), this, true);
	dlg.setPixmap(":/control/bin.png");
	dlg.show();

	clearSelection();

	foreach(QUSongItem *songItem, selectedItems) {
		dlg.update(songItem->song()->songFileInfo().dir().dirName());
		if(dlg.cancelled()) break;

		QUSongFile *song = songItem->song();
		delete takeTopLevelItem(indexOfTopLevelItem(songItem));

		emit deleteSongRequested(song);
	}

	emit itemSelectionChanged();
}

/*!
 * Merge all selected songs with the first one in the selection.
 *
 * Copy all files of the source song into the directory of the target song. The sources
 * song will then be deleted.
 *
 */
void QUSongTree::mergeSelectedSongs() {
	QList<QUSongItem*> selectedItems = selectedSongItems();

	if(selectedItems.size() < 2) {
		emit finished(tr("Too few songs selected. You have to merge at least 2 songs."), QU::warning);
		return;
	}

	QUSongItem *currentSongItem = dynamic_cast<QUSongItem*>(currentItem());

	if(!currentSongItem)
		return;

	selectedItems.removeAll(currentSongItem);

	// ---------------------------------

	QString infoText = QString(tr("You want to merge songs with <b>\"%1 - %2\"</b>. All files of the other songs will be moved to that song's path.<br><br>The following <b>%3</b> songs will disappear:"))
		.arg(currentSongItem->song()->artist())
		.arg(currentSongItem->song()->title())
		.arg(selectedItems.size());

	int i = 0;
	foreach(QUSongItem *songItem, selectedItems) {
		if(++i >= 5) {
			infoText.append("<br>&nbsp;&nbsp;...");
			break;
		}

		infoText.append(QString("<br>&nbsp;&nbsp;<b>%1 - %2</b>").arg(songItem->song()->artist()).arg(songItem->song()->title()));
	}

	QUMessageBox::Results result = QUMessageBox::ask(this,
			tr("Merge Songs"),
			infoText,
			":/control/bin.png", tr("Merge these songs."),
			":/marks/cancel.png", tr("Cancel merge operation."),
			"", "",
			100);
	if(result == QUMessageBox::second)
		return;

	// ----------------------------------

	QUProgressDialog dlg(QString(tr("Merging selected songs with \"%1 - %2\"...")).arg(currentSongItem->song()->artist()).arg(currentSongItem->song()->title()), selectedItems.size(), this);
//	dlg.setPixmap(":/control/bin.png");
	dlg.show();

	clearSelection();

	i = 0;
	foreach(QUSongItem *songItem, selectedItems) {
		dlg.update(songItem->song()->songFileInfo().dir().dirName());
		if(dlg.cancelled()) break;

		QDir dir(songItem->song()->songFileInfo().dir());

		QFileInfoList fiList = dir.entryInfoList(QStringList("*.*"), QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot, QDir::Name);

		bool allFilesCopied = true;
		foreach(QFileInfo fi, fiList) {
			QString target = QFileInfo(currentSongItem->song()->songFileInfo().dir(), QString("%1%2_%3").arg(i++, 3, 10, QChar('0')).arg(QTime::currentTime().toString("sszzz")).arg(fi.fileName())).filePath();

			if(!QFile::copy(fi.filePath(), target)) {
				allFilesCopied = false;
				emit finished(QString(tr("Could NOT copy file \"%1\" to \"%2\".")).arg(fi.filePath()).arg(target), QU::warning);
			} else
				emit finished(QString(tr("File was copied successfully from \"%1\" to \"%2\".")).arg(fi.filePath()).arg(target), QU::information);
		}

		if(allFilesCopied) {
			QUSongFile *song = songItem->song();
			delete takeTopLevelItem(indexOfTopLevelItem(songItem));

			emit deleteSongRequested(song);
		} else {
			emit finished(QString(tr("Not all files of \"%1 - %2\" were copied. Song will not be deleted. Merging failed.")).arg(songItem->song()->artist()).arg(songItem->song()->title()), QU::warning);
		}
	}

	currentSongItem->update();
	currentSongItem->setSelected(true);
}

bool QUSongTree::copyFilesToSong(const QList<QUrl> &files, QUSongItem *item) {
	if(files.size() > FILE_DROP_LIMIT) {
		QUMessageBox::Results result = QUMessageBox::ask(this,
				tr("Copy Files"),
				QString(tr("You want to copy <b>%1</b> files to <b>\"%2\"</b>.")).arg(files.size()).arg(item->song()->songFileInfo().dir().path()),
				":/control/save_all.png", tr("Copy these files."),
				":/marks/cancel.png", tr("Cancel copy operation."));
		if(result == QUMessageBox::second)
			return false;
	}

	bool dataUsed = false;

	QUProgressDialog dlg(QString(tr("Copy & Use files for the song: \"%1 - %2\"...")).arg(item->song()->artist()).arg(item->song()->title()), files.size(), this);
	dlg.setPixmap(":/marks/disk.png");
	dlg.show();

	foreach(QUrl url, files) {
		dlg.update(url.toLocalFile());
		if(dlg.cancelled()) break;

		if(!QFileInfo(url.toLocalFile()).isDir()) { // do not copy directories
			item->song()->useExternalFile(url.toLocalFile());
			dataUsed = true;
		}
	}

	if(dataUsed) {
		item->song()->save();
		item->update();
		emit itemSelectionChanged(); // update details
	}

	return true;
}

/*!
 * Create new song directories for dropped song text files and copy that file to
 * the new created directory.
 */
void QUSongTree::dropSongFiles(const QList<QUrl> &urls) {
	QUProgressDialog dlg(tr("Including new songs to database..."), urls.size(), this);
	dlg.show();

	QList<QUSongItem*> newItems;

	foreach(QUrl url, urls) {
		QFileInfo fi(url.toLocalFile());

		dlg.update(fi.fileName());
		if(dlg.cancelled()) break;

		QString fileScheme("*." + fi.suffix());

		if(!QU::allowedSongFiles().contains(fileScheme, Qt::CaseInsensitive)) {
			emit finished(QString(tr("Invalid song file found: \"%1\". Cannot include those.")).arg(fi.filePath()), QU::warning);
			continue;
		}

		if(QU::allowedLicenseFiles().contains(fi.fileName(), Qt::CaseInsensitive)) {
			emit finished(QString(tr("Cannot include license files as songs: \"%1\"")).arg(fi.filePath()), QU::warning);
			continue;
		}

		QUSongFile *newSong = new QUSongFile(fi.filePath());

		createSongFolder(newSong);

		if(!QFile::copy(fi.filePath(), newSong->songFileInfo().filePath())) {
			emit finished(QString(tr("Could not copy song file \"%1\" to new song directory \"%2\"!")).arg(fi.fileName()).arg(newSong->songFileInfo().path()), QU::warning);
			continue;
		}

		QUSongItem *newItem = new QUSongItem(newSong, true);
		this->addTopLevelItem(newItem);
		newItems.append(newItem);

		emit finished(QString(tr("New song included to your song collection: \"%1 - %2\".")).arg(newSong->artist()).arg(newSong->title()), QU::information);
		emit songCreated(newSong);
	}

	// select new song items
	QUMainWindow *parentWindow = this->parentWindow();
	if(parentWindow) {
		disconnect(this, SIGNAL(itemSelectionChanged()), parentWindow, SLOT(updateDetails()));
		disconnect(this, SIGNAL(itemSelectionChanged()), parentWindow, SLOT(updatePreviewTree()));
	}

	foreach(QUSongItem *songItem, newItems)
		songItem->setSelected(true);

	if(parentWindow) {
		connect(this, SIGNAL(itemSelectionChanged()), parentWindow, SLOT(updateDetails()));
		connect(this, SIGNAL(itemSelectionChanged()), parentWindow, SLOT(updatePreviewTree()));
	}

	emit itemSelectionChanged();
}

/*!
 * Try to create a basic folder for the new song according to the schema:
 * "Artist - Title"
 *
 * Set the given song to that folder.
 */
void QUSongTree::createSongFolder(QUSongFile *song) {
	QString newDirName = QU::withoutUnsupportedCharacters(QString("%1 - %2").arg(song->artist()).arg(song->title())).trimmed();

	int i = 0;
	QString tmp = newDirName;
	QFileInfo fi(QUMainWindow::BaseDir, newDirName);
	while(!QUMainWindow::BaseDir.mkdir(tmp)) {
		if(!fi.exists()) {
			emit finished(QString(tr("Could not create directory: \"%1\". Disk full?")).arg(fi.filePath()), QU::warning);
			return;
		} else {
			tmp = QString("%1_%2").arg(newDirName).arg(i, 3, 10, QChar('0'));
			i++;
			fi.setFile(QUMainWindow::BaseDir, tmp);
		}
	}

	song->setFile( QFileInfo(QDir(fi.filePath()), song->songFileInfo().fileName()).filePath() );
}

void QUSongTree::refreshSelectedItems() {
	QList<QUSongItem*> songItems = this->selectedSongItems();
	QList<bool>        itemExpandedStates;

	if(songItems.isEmpty())
		return;

	QUProgressDialog dlg(tr("Refreshing selected songs..."), songItems.size(), this, false);
	dlg.setPixmap(":/types/folder.png");
	dlg.show();

	clearSelection();

	foreach(QUSongItem *songItem, songItems) {
		dlg.update(songItem->song()->songFileInfo().dir().dirName());

		itemExpandedStates.append(songItem->isExpanded());

		if(songItem->isToplevel()) { // for performance reasons...
			takeTopLevelItem(indexOfTopLevelItem(songItem));
			songItem->update();
			addTopLevelItem(songItem);
		} else
			songItem->update();
	}

	setCurrentItem(songItems.first());
	foreach(QUSongItem *songItem, songItems) {
		songItem->setExpanded(itemExpandedStates.first());
		itemExpandedStates.pop_front();
	}

	scrollToItem(currentItem());
	restoreSelection(songItems);

//	emit itemSelectionChanged(); // update details
}

void QUSongTree::resizeToContents() {
	for(int i = 0; i < this->columnCount(); i++)
		this->resizeColumnToContents(i);
}

void QUSongTree::saveSelectedSongs() {
	QList<QTreeWidgetItem*> items = this->selectedItems();

	if(items.isEmpty())
		items.append(this->currentItem());

	QUProgressDialog dlg(tr("Saving selected songs..."), items.size(), this);
	dlg.show();

	foreach(QTreeWidgetItem *item, items) {
		QUSongItem *songItem = dynamic_cast<QUSongItem*>(item);

		if(songItem) {
			dlg.update(songItem->song()->songFileInfo().fileName());
			if(dlg.cancelled()) break;

			songItem->song()->save(true);
			songItem->update();
		}
	}

	emit itemSelectionChanged(); // update details
}

void QUSongTree::keyPressEvent(QKeyEvent *event) {
	if(event->key() == Qt::Key_Delete) {
		this->deleteCurrentItem();
	} else {
		QTreeWidget::keyPressEvent(event);
	}
}

/*!
 * Delete the current item, which should represent a file and not a folder
 * because folder items can be selected and are top-level.
 */
void QUSongTree::deleteCurrentItem() {
	QUSongItem *item = dynamic_cast<QUSongItem*>(this->currentItem());

	if(!item || item->isToplevel())
		return; // only allow to delete files which cannot be selected - no directories

	QUMessageBox::Results result = QUMessageBox::ask(this,
			tr("Delete File"),
			QString(tr("<b>\"%1\"</b> will be deleted permanently. You cannot undo a delete operation.")).arg(item->text(FOLDER_COLUMN)),
			":/control/bin.png", tr("Delete this file."),
			":/marks/cancel.png", tr("Cancel delete operation."));
	if(result == QUMessageBox::second)
		return;

	if(QFile::remove(QFileInfo(item->song()->songFileInfo().dir(), item->text(FOLDER_COLUMN)).filePath()))
		emit finished(QString(tr("The file \"%1\" was deleted successfully.")).arg(item->text(FOLDER_COLUMN)), QU::information);
	else
		emit finished(QString(tr("The file \"%1\" was NOT deleted.")).arg(item->text(FOLDER_COLUMN)), QU::warning);

	this->setCurrentItem(item->parent());
	item->update();

	emit itemSelectionChanged(); // update details
}

/*!
 * Send all selected songs to the current playlist.
 */
void QUSongTree::sendSelectedSongsToPlaylist() {
	QList<QTreeWidgetItem*> items = this->selectedItems();

	if(items.isEmpty())
		items.append(this->currentItem());

	foreach(QTreeWidgetItem *item, items) {
		QUSongItem *songItem = dynamic_cast<QUSongItem*>(item);

		if(songItem)
			emit songToPlaylistRequested(songItem->song());
	}
}

/*!
 * Request to show the lyrics of the current song. It is only possible to show the lyrics of one
 * song. More songs could be selected.
 */
void QUSongTree::requestLyrics() {
	QUSongItem *cItem = dynamic_cast<QUSongItem*>(this->currentItem());

	if(cItem)
		emit showLyricsRequested(cItem->song());

	if(selectedSongItems().size() > 1)
		emit finished(tr("You can only display the lyrics of one song at a time."), QU::information);
}

/*!
 * Request covers for all selected songs (song items). Send the items so that they can be updates,
 * if songs got new covers.
 */
void QUSongTree::requestCoversFromAmazon() {
	emit coversFromAmazonRequested(this->selectedSongItems());
}

void QUSongTree::removeFilter() {
	QList<QUSongItem*> selectedItems = this->selectedSongItems();

	QUProgressDialog dlg(tr("Removing current filter..."), _hiddenItems.size(), this, false);
	dlg.setPixmap(":/types/folder.png");
	dlg.show();

	clearSelection();

	foreach(QTreeWidgetItem *hiddenItem, _hiddenItems) {
		QUSongItem *songItem = dynamic_cast<QUSongItem*>(hiddenItem);

		if(songItem)
			dlg.update(songItem->song()->songFileInfo().dir().dirName());
		else
			dlg.update(N_A);

		this->addTopLevelItem(hiddenItem);
	}

	restoreSelection(selectedItems);

	_hiddenItems.clear();
}

/*!
 * Restore a selection of items that was cleared before for performance reasons.
 */
void QUSongTree::restoreSelection(const QList<QUSongItem*> &selectedItems) {
	QItemSelection selection;

	foreach(QUSongItem *item, selectedItems) {
		int row = indexOfTopLevelItem(item->isToplevel() ? item : item->parent());

		if(row == -1)
			continue;

		QModelIndex start = model()->index(row, 0);
		selection.append(QItemSelectionRange(start));
	}

	selectionModel()->select(selection, QItemSelectionModel::Select | QItemSelectionModel::Rows);
}
