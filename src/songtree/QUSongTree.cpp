#include "QUSongTree.h"
#include "QUSongItem.h"
#include "QUSongFile.h"
#include "QUColumnAction.h"
#include "QUMainWindow.h"
#include "QUMessageBox.h"
#include "QULogService.h"
#include "QUSongDatabase.h"

#include "QUMetaphoneString.h"

#include <QMessageBox>
#include <QUrl>
#include <QUrlQuery>
#include <QIcon>
#include <QMenu>
#include <QList>
#include <QRegularExpression>
#include <QByteArray>
#include <QAction>
#include <QHeaderView>
#include <QCursor>
#include <QTime>
#include <QDesktopServices>
#include <QShortcut>
#include <QUrl>
#include <QTextStream>
#include <QMimeData>

#include "QUProgressDialog.h"

#include "QUSongSupport.h"
#include "QUStringSupport.h"

#define FILE_DROP_LIMIT 5

QUSongTree::QUSongTree(QWidget *parent): QTreeWidget(parent) {
	setAcceptDrops(true);
	setDropIndicatorShown(true);

	this->setEditTriggers(QAbstractItemView::NoEditTriggers);

	// enable context menu for items
	this->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showItemMenu(const QPoint&)));

	// enable context menu for header
	this->header()->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this->header(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showHeaderMenu(const QPoint&)));

	// be aware of database changes
	connect(songDB, SIGNAL(databaseCleared()), this, SLOT(clear()));
	connect(songDB, SIGNAL(songAdded(QUSongFile*)), this, SLOT(addItem(QUSongFile*)));
	connect(songDB, SIGNAL(databaseReloaded()), this, SLOT(resizeAndSort()));
	connect(songDB, SIGNAL(songChanged(QUSongFile*)), this, SLOT(updateItem(QUSongFile*)));
	connect(songDB, SIGNAL(songWithFriendSwapped(QUSongFile*,QUSongFile*)), this, SLOT(updateItemWithNewSong(QUSongFile*,QUSongFile*)));

	new QShortcut(QKeySequence("F5"), this, SLOT(refreshSelectedItems()));
}

QUMainWindow* QUSongTree::parentWindow() const {
	return dynamic_cast<QUMainWindow*>(parentWidget());
}

void QUSongTree::initHorizontalHeader() {
	this->setColumnCount(FIXED_COLUMN_COUNT + QUSongSupport::availableCustomTags().count());

	QTreeWidgetItem *header = new QTreeWidgetItem();
	header->setText(FOLDER_COLUMN, QString(tr("Folder (%1)")).arg(QDir::toNativeSeparators(QU::BaseDir.path())));
	header->setIcon(FOLDER_COLUMN, QIcon(":/types/folder.png"));

	header->setIcon(ARTIST_COLUMN, QIcon(":/types/user.png"));
	header->setToolTip(ARTIST_COLUMN, tr("Shows whether your folder includes the artist correctly:<br><i>Artist - Title ...</i>"));
	header->setSizeHint(ARTIST_COLUMN, QSize(25,0));
	header->setIcon(TITLE_COLUMN, QIcon(":/types/font.png"));
	header->setToolTip(TITLE_COLUMN, tr("Shows whether your folder includes the title correctly:<br><i>Artist - Title ...</i>"));
	header->setSizeHint(TITLE_COLUMN, QSize(25,0));

	header->setIcon(MP3_COLUMN, QIcon(":/types/music.png"));
	header->setToolTip(MP3_COLUMN, tr("Shows whether the song text file points to an <b>audio file</b> that can be found by UltraStar"));
	header->setSizeHint(MP3_COLUMN, QSize(25,0));
	header->setIcon(COVER_COLUMN, QIcon(":/types/cover.png"));
	header->setToolTip(COVER_COLUMN, tr("Shows whether the song text file points to a <b>cover picture</b> that can be found by UltraStar"));
	header->setSizeHint(COVER_COLUMN, QSize(25,0));
	header->setIcon(BACKGROUND_COLUMN, QIcon(":/types/background.png"));
	header->setToolTip(BACKGROUND_COLUMN, tr("Shows whether the song text file points to a <b>background picture</b> that can be found by UltraStar"));
	header->setSizeHint(BACKGROUND_COLUMN, QSize(25,0));
	header->setIcon(VIDEO_COLUMN, QIcon(":/types/video.png"));
	header->setToolTip(VIDEO_COLUMN, tr("Shows whether the song text file points to a <b>video file</b> that can be found by UltraStar"));
	header->setSizeHint(VIDEO_COLUMN, QSize(25,0));

	header->setIcon(TYPE_KARAOKE_COLUMN, QIcon(":/types/karaoke.png"));
	header->setToolTip(TYPE_KARAOKE_COLUMN, tr("Shows whether the song is a karaoke song."));
	header->setSizeHint(TYPE_KARAOKE_COLUMN, QSize(25,0));
	header->setIcon(TYPE_DUET_COLUMN, QIcon(":/types/duet.png"));
	header->setToolTip(TYPE_DUET_COLUMN, tr("Shows whether the song is a duet."));
	header->setSizeHint(TYPE_DUET_COLUMN, QSize(25,0));
	header->setIcon(MEDLEY_COLUMN, QIcon(":/types/medley.png"));
	header->setToolTip(MEDLEY_COLUMN, tr("Shows whether the song has a medley section."));
	header->setSizeHint(MEDLEY_COLUMN, QSize(25,0));
	header->setIcon(GOLDEN_NOTES_COLUMN, QIcon(":/types/golden_notes.png"));
	header->setToolTip(GOLDEN_NOTES_COLUMN, tr("Shows whether the song has golden notes."));
	header->setSizeHint(GOLDEN_NOTES_COLUMN, QSize(25,0));
	header->setIcon(RAP_NOTES_COLUMN, QIcon(":/types/rap_notes.png"));
	header->setToolTip(RAP_NOTES_COLUMN, tr("Shows whether the song has rap notes."));
	header->setSizeHint(RAP_NOTES_COLUMN, QSize(25,0));

	header->setIcon(UNUSED_FILES_COLUMN, QIcon(":/types/unused_files.png"));
	header->setToolTip(UNUSED_FILES_COLUMN, tr("Shows whether your folder contains unused files."));
	header->setSizeHint(UNUSED_FILES_COLUMN, QSize(25,0));
	header->setIcon(MULTIPLE_SONGS_COLUMN, QIcon(":/types/text_stack.png"));
	header->setToolTip(MULTIPLE_SONGS_COLUMN, tr("Shows whether your folder contains more than one song text file."));
	header->setSizeHint(MULTIPLE_SONGS_COLUMN, QSize(25,0));

	header->setIcon(SCORE_COLUMN, QIcon(":/types/score.png"));
	header->setToolTip(SCORE_COLUMN, tr("Shows whether a score file was found."));
	header->setSizeHint(SCORE_COLUMN, QSize(25,0));

	header->setIcon(SYNC_COLUMN, QIcon(":/types/sync.png"));
	header->setToolTip(SYNC_COLUMN, tr("Shows whether a sync file was found."));
	header->setSizeHint(SYNC_COLUMN, QSize(25,0));

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
	header->setToolTip(LENGTH_COLUMN, tr("Shows the song length calculated from BPM and lyrics."));

	header->setIcon(LENGTH_DIFF_COLUMN, QIcon(":/types/time_warning.png"));
	header->setToolTip(LENGTH_DIFF_COLUMN, tr("Indicates a problem with the difference of <i>song length</i> and <i>audio length</i>.<br><br>The tooltip shows the difference."));
	header->setSizeHint(LENGTH_DIFF_COLUMN, QSize(25,0));

	header->setText(LENGTH_MP3_COLUMN, tr("Audio"));
	header->setIcon(LENGTH_MP3_COLUMN, QIcon(":/types/time_mp3.png"));
	header->setToolTip(LENGTH_MP3_COLUMN, tr("Shows the length of audio file, if present.<br><br>Can be reset through <b>#END</b> tag."));

	header->setText(LENGTH_EFF_COLUMN, tr("Effective"));
	header->setIcon(LENGTH_EFF_COLUMN, QIcon(":/types/time_eff.png"));
	header->setToolTip(LENGTH_EFF_COLUMN, tr("Shows the effective song length:<br><b>audio length - #START</b>."));

	header->setText(SPEED_COLUMN, tr("Speed"));
//	header->setIcon(SPEED_COLUMN, QIcon(":/types/time_eff.png"));
	header->setToolTip(SPEED_COLUMN, tr("Shows the number of singable syllables per second. No freestyle or pauses included."));

	header->setText(START_COLUMN, tr("Start"));
	header->setIcon(START_COLUMN, QIcon(":/types/start.png"));
	header->setToolTip(START_COLUMN, tr("Shows how much time is skipped from the beginning of the audio file.<br><i>(in seconds)</i>"));
	header->setText(END_COLUMN, tr("End"));
	header->setIcon(END_COLUMN, QIcon(":/types/end.png"));
	header->setToolTip(END_COLUMN, tr("Shows when the song playback stops.<br><i>(in <b>milli</b>seconds)</i>"));
	header->setText(VIDEOGAP_COLUMN, tr("Videogap"));
	header->setIcon(VIDEOGAP_COLUMN, QIcon(":/types/videogap.png"));
	header->setToolTip(VIDEOGAP_COLUMN, tr("Shows how much time is skipped from the video file.<br><i>(in seconds)</i>"));

	header->setText(RELATIVE_COLUMN, tr("Relative?"));
	header->setToolTip(RELATIVE_COLUMN, tr("Shows whether the song has relative timestamps."));
	header->setText(BPM_COLUMN, tr("BPM"));
	header->setIcon(BPM_COLUMN, QIcon(":/types/bpm.png"));
	header->setToolTip(BPM_COLUMN, tr("Shows the song tempo.<br><i>(in beats per 15 seconds)"));
	header->setText(GAP_COLUMN, tr("GAP"));
	header->setIcon(GAP_COLUMN, QIcon(":/types/gap.png"));
	header->setToolTip(GAP_COLUMN, tr("Shows the gap between the beginning of the song and the first syllable.<br><i>(in milliseconds)</i>"));

	//header->setText(DUPLICATE_ID_COLUMN, "ID");
	header->setToolTip(DUPLICATE_ID_COLUMN, tr("Indicate duplicate songs. <b>You should not see me.</b>"));

	int i = 0;
	foreach(QString customTag, QUSongSupport::availableCustomTags()) {
		header->setText(FIRST_CUSTOM_TAG_COLUMN + i, customTag);
		header->setIcon(FIRST_CUSTOM_TAG_COLUMN + i, QIcon(":/bullets/bullet_star.png"));
		i++;
	}

	// set up default header
	this->setHeaderItem(header);
	this->showDefaultColumns(false);

	// load custom setup
	QSettings settings;
	this->header()->restoreState(settings.value("songTreeState", QVariant()).toByteArray());
}

bool QUSongTree::hasUnsavedChanges() const {
	foreach(QUSongItem *visibleItem, visibleSongItems()) {
		if(visibleItem->song()->hasUnsavedChanges())
			return true;
	}

	foreach(QUSongItem *hiddenItem, _hiddenItems) {
		if(hiddenItem->song()->hasUnsavedChanges())
			return true;
	}

	return false;
}

/*!
 * \returns All selected songs in the song tree.
 */
QList<QUSongFile*> QUSongTree::selectedSongs() const {
	QList<QUSongFile*> songs;

	foreach(QUSongItem *songItem, selectedSongItems())
		songs.append(songItem->song());

	return songs;
}

/*!
 * \returns All selected song items or the current toplevel item if nothing was
 * selected.
 */
QList<QUSongItem*> QUSongTree::selectedSongItems() const {
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

QList<QUSongFile*> QUSongTree::visibleSongs() const {
	QList<QUSongFile*> result;

	foreach(QUSongItem *songItem, visibleSongItems())
		result.append(songItem->song());

	return result;
}

/*!
 * \returns a list of all visible song items
 */
QList<QUSongItem*> QUSongTree::visibleSongItems() const {
	QList<QUSongItem*> items;

	QList<QTreeWidgetItem*> topLevelItems;
	for(int i = 0; i < this->topLevelItemCount(); ++i)
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
QList<QUSongItem*> QUSongTree::allSongItems() const {
	return (visibleSongItems() << _hiddenItems);
}

int QUSongTree::selectedSongFriendsCount() const {
	int result = 0;
	foreach(QUSongFile *song, selectedSongs())
		result += song->friends().size();
	return result;
}

int QUSongTree::visibleSongFriendsCount() const {
	int result = 0;
	foreach(QUSongFile *song, visibleSongs())
		result += song->friends().size();
	return result;
}

int QUSongTree::hiddenSongFriendsCount() const {
	int result = 0;
	foreach(QUSongItem *songItem, _hiddenItems)
		result += songItem->song()->friends().size();
	return result;
}

void QUSongTree::saveUnsavedChanges() {
	this->removeFilter();

	QList<QUSongItem*> songItems = visibleSongItems(); // should be all

	if(songItems.isEmpty())
		return;

	QUProgressDialog dlg(tr("Saving unsaved changes..."), songItems.size(), this);
	dlg.show();

	clearSelection();

	foreach(QUSongItem *songItem, songItems) {
		dlg.update(songItem->song()->songFileInfo().fileName());
		if(dlg.cancelled()) break;

		if(songItem->song()->hasUnsavedChanges()) {
			songItem->song()->save(true);
			songItem->update();
		}
	}

//	restoreSelection(songItems);
	emit itemSelectionChanged();
}

/*!
 * Overloaded to clear hidden items.
 */
void QUSongTree::clear() {
	qDeleteAll(_hiddenItems);
	_hiddenItems.clear();
	QTreeWidget::clear();
}

void QUSongTree::addItem(QUSongFile *song) {
	addTopLevelItem(new QUSongItem(song, true));
}

void QUSongTree::updateItem(QUSongFile *song) {
	foreach(QUSongItem *songItem, allSongItems()) {
		if(songItem->song() == song || songItem->song()->isFriend(song)) {
			songItem->update();
			setCurrentItem(songItem);
			emit itemSelectionChanged();
			break;
		}
	}
}

void QUSongTree::resizeAndSort() {
	resizeToContents();
	sortItems(FOLDER_COLUMN, Qt::AscendingOrder);
}

void QUSongTree::updateItemWithNewSong(QUSongFile *oldSong, QUSongFile *newSong) {
	foreach(QUSongItem *songItem, allSongItems()) {
		if(songItem->song() == oldSong) {
			songItem->setSong(newSong);
			songItem->update();
			setCurrentItem(songItem);
			emit itemSelectionChanged();
			break;
		}
	}
}

/*!
 * Hides all items from the tree which do not match the filter. An empty string
 * will remove the filter and show all items again. Selected items will be
 * unselected if hidden.
 */
void QUSongTree::filterItems(const QString &regexp, QU::FilterModes mode) {
	this->removeFilter();

	if(regexp.isEmpty()) {
		logSrv->add(tr("Filter removed. All songs are visible now."), QU::Information);
		emit itemSelectionChanged(); // update details
		return;
	}

	QRegularExpression rx(regexp, QRegularExpression::CaseInsensitiveOption);

	// collect all toplevel items
	QList<QTreeWidgetItem*> topLevelItems;
	for(int i = 0; i < this->topLevelItemCount(); ++i)
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

			progress.update(QString("%1 - %2").arg(song->artist(), song->title()));
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

				foreach(QString customTag, QUSongSupport::availableCustomTags()) {
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

	emit itemSelectionChanged(); // update details
	logSrv->add(QString(tr("Filter applied: \"%1\"%2")).arg(regexp, mode.testFlag(QU::negateFilter) ? tr(", negated") : ""), QU::Information);
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
	QHash<QUSongItem*, int> id;

	// sort after an invisible column to combine duplicate items
	this->sortItems(FOLDER_COLUMN, Qt::AscendingOrder);
	this->sortItems(DUPLICATE_ID_COLUMN, Qt::AscendingOrder);

	QUProgressDialog dlg(tr("Looking for duplicate songs..."), allItems.size(), this);
//	dlg.setPixmap(":/control/eye.png"); --> inserte a duplicate picture? show which songs we combine?
	dlg.show();

	foreach(QUSongItem *item, allItems) {
		dlg.update(QString("%1 - %2").arg(item->song()->artist(), item->song()->title()));
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
		logSrv->add(QString(tr("Filter applied. Duplicates for %1 songs found.")).arg(nextID), QU::Information);
	else
		logSrv->add(QString(tr("No duplicate songs found.")), QU::Information);
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

Qt::DropActions QUSongTree::supportedDropActions() const {
	return Qt::CopyAction | Qt::MoveAction;
}

QStringList QUSongTree::mimeTypes() const {
	return QStringList() << "text/uri-list";
}

/*!
 * Appends actions to the menu according to the selected item/file.
 */
void QUSongTree::showItemMenu(const QPoint &point) {
	QMenu *menu;

	if(!this->itemAt(point))
		menu = generalMenu();
	else {
		QUSongItem *item = dynamic_cast<QUSongItem*>(this->currentItem());
		if(item)
			menu = itemMenu(item);
		else
			return;
	}

	menu->exec(QCursor::pos());
	delete menu;
}

QMenu* QUSongTree::hideMenu() const {
	QMenu *filterMenu = new QMenu(tr("Hide"));
	filterMenu->setIcon(QIcon(":/control/eye.png"));
	filterMenu->addAction(tr("Selected Songs"), this, SLOT(hideSelected()));
	filterMenu->addAction(tr("Selected Songs Only"), this, SLOT(hideSelectedOnly()));
	filterMenu->addAction(tr("Unselected Songs"), this, SLOT(hideAllButSelected()));
	filterMenu->addSeparator();
	filterMenu->addAction(tr("All"), this, SLOT(hideAll()));
	filterMenu->addAction(tr("None"), this, SLOT(removeFilter()));
	return filterMenu;
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
	QMenu lengthsMenu(tr("Time && Speed"), this); lengthsMenu.setIcon(QIcon(":/types/time.png"));
	QMenu typesMenu(tr("Types"), this);

	for(int i = 0; i < headerItem()->columnCount(); ++i) {
		if(i != LENGTH_DIFF_COLUMN
		   && i != TYPE_DUET_COLUMN
		   && i != TYPE_KARAOKE_COLUMN
		   && i != MEDLEY_COLUMN
		   && i != GOLDEN_NOTES_COLUMN
		   && i != RAP_NOTES_COLUMN
		   && (headerItem()->text(i).isEmpty() || i == FOLDER_COLUMN))
			continue;

		QUColumnAction *a = new QUColumnAction(headerItem()->text(i), i); // save the logical index of this column
		a->setChecked(!header()->isSectionHidden(i));

		connect(a, SIGNAL(columnToggled(bool, int)), this, SLOT(toggleColumn(bool, int)));

		switch(i) {
		case TYPE_KARAOKE_COLUMN:
		case TYPE_DUET_COLUMN:
		case MEDLEY_COLUMN:
		case GOLDEN_NOTES_COLUMN:
		case RAP_NOTES_COLUMN: typesMenu.addAction(a); break;
		case SPEED_COLUMN: lengthsMenu.addSeparator();
		case START_COLUMN: lengthsMenu.addSeparator();
		case LENGTH_COLUMN:
		case LENGTH_DIFF_COLUMN:
		case LENGTH_MP3_COLUMN:
		case LENGTH_EFF_COLUMN:
		case END_COLUMN:
		case VIDEOGAP_COLUMN: lengthsMenu.addAction(a); break;
		case RELATIVE_COLUMN: menu.addSeparator();
		default:
			if(QUSongSupport::availableCustomTags().contains(a->text(), Qt::CaseInsensitive))
				customTagsMenu.addAction(a);
			else
				menu.addAction(a);
		}
	}

	menu.addSeparator();
	menu.addMenu(&typesMenu);
	menu.addMenu(&lengthsMenu);
	menu.addMenu(&customTagsMenu);

	menu.exec(QCursor::pos());
}

void QUSongTree::toggleColumn(bool show, int index) {
	this->header()->setSectionHidden(index, !show);
//	this->resizeToContents();
	this->resizeColumnToContents(index);

	QSettings settings;
	settings.setValue("songTreeState", QVariant(header()->saveState()));
}

void QUSongTree::showAllColumns() {
	for(int i = 0; i < headerItem()->columnCount(); ++i)
		header()->showSection(i);

	this->header()->setSectionHidden(DUPLICATE_ID_COLUMN, true);

	this->resizeToContents();

	QSettings settings;
	settings.setValue("songTreeState", QVariant(header()->saveState()));
}

void QUSongTree::showDefaultColumns(bool save) {
	for(int i = 0; i < headerItem()->columnCount(); ++i)
		header()->hideSection(i);

	this->header()->showSection(FOLDER_COLUMN);
	this->header()->showSection(ARTIST_COLUMN);
	this->header()->showSection(TITLE_COLUMN);
	this->header()->showSection(MP3_COLUMN);
	this->header()->showSection(COVER_COLUMN);
	this->header()->showSection(BACKGROUND_COLUMN);
	this->header()->showSection(VIDEO_COLUMN);
	this->header()->showSection(TYPE_KARAOKE_COLUMN);
	this->header()->showSection(TYPE_DUET_COLUMN);
	this->header()->showSection(MEDLEY_COLUMN);
	this->header()->showSection(GOLDEN_NOTES_COLUMN);
	this->header()->showSection(RAP_NOTES_COLUMN);
	this->header()->showSection(UNUSED_FILES_COLUMN);
	this->header()->showSection(MULTIPLE_SONGS_COLUMN);
	this->header()->showSection(SCORE_COLUMN);
	this->header()->showSection(SYNC_COLUMN);
	this->header()->showSection(LANGUAGE_COLUMN);
	this->header()->showSection(EDITION_COLUMN);
	this->header()->showSection(GENRE_COLUMN);
	this->header()->showSection(YEAR_COLUMN);
	this->header()->showSection(CREATOR_COLUMN);

	int customTagsCount = QUSongSupport::availableCustomTags().size();
	for(int i = 0; i < customTagsCount; ++i)
		header()->showSection(FIRST_CUSTOM_TAG_COLUMN + i);

	this->resizeToContents();

	if(save) {
		QSettings settings;
		settings.setValue("songTreeState", QVariant(header()->saveState()));
	}
}

/*!
 * Hide all but the folder column.
 */
void QUSongTree::showMinimalColumns() {
	for(int i = 0; i < headerItem()->columnCount(); ++i)
		header()->hideSection(i);

	this->header()->showSection(FOLDER_COLUMN);

	this->resizeToContents();

	QSettings settings;
	settings.setValue("songTreeState", QVariant(header()->saveState()));
}

void QUSongTree::showTimeColumns() {
	for(int i = 0; i < headerItem()->columnCount(); ++i)
		header()->hideSection(i);

	this->header()->showSection(FOLDER_COLUMN);
	this->header()->showSection(LENGTH_COLUMN);
	this->header()->showSection(LENGTH_DIFF_COLUMN);
	this->header()->showSection(LENGTH_MP3_COLUMN);
	this->header()->showSection(LENGTH_EFF_COLUMN);
	this->header()->showSection(SPEED_COLUMN);
	this->header()->showSection(START_COLUMN);
	this->header()->showSection(END_COLUMN);
	this->header()->showSection(VIDEOGAP_COLUMN);

	this->resizeToContents();

	QSettings settings;
	settings.setValue("songTreeState", QVariant(header()->saveState()));
}

void QUSongTree::showCheckColumns() {
	for(int i = 0; i < headerItem()->columnCount(); ++i)
		header()->hideSection(i);

	this->header()->showSection(FOLDER_COLUMN);
	this->header()->showSection(ARTIST_COLUMN);
	this->header()->showSection(TITLE_COLUMN);
	this->header()->showSection(MP3_COLUMN);
	this->header()->showSection(COVER_COLUMN);
	this->header()->showSection(BACKGROUND_COLUMN);
	this->header()->showSection(VIDEO_COLUMN);
	this->header()->showSection(MEDLEY_COLUMN);
	this->header()->showSection(GOLDEN_NOTES_COLUMN);
	this->header()->showSection(RAP_NOTES_COLUMN);
	this->header()->showSection(UNUSED_FILES_COLUMN);
	this->header()->showSection(MULTIPLE_SONGS_COLUMN);
	this->header()->showSection(SCORE_COLUMN);
	this->header()->showSection(SYNC_COLUMN);

	this->resizeToContents();

	QSettings settings;
	settings.setValue("songTreeState", QVariant(header()->saveState()));
}

/*!
 * Emits the proper signal to open the current file externally. (See QUMainWindow)
 */
void QUSongTree::openCurrentFileExternal() {
	emit openCurrentFileExternalRequested(this->currentItem());
}

/*!
 * Emits the proper signal to open the current file internally (image and text files only). (See QUMainWindow)
 */
void QUSongTree::openCurrentFileInternal() {
	emit openCurrentFileInternalRequested(this->currentItem());
}

/*!
 * Emits the proper signal to set the current file as primary song file. (See QUMainWindow)
 */
void QUSongTree::setCurrentFilePrimary() {
	emit setCurrentFilePrimaryRequested(this->currentItem());
}

/*!
 * Open the current folder in the file explorer of the operating system.
 */
void QUSongTree::openCurrentFolder() {
	QUSongItem *currentSongItem = dynamic_cast<QUSongItem*>(this->currentItem());

	if(!currentSongItem)
		return;

	QDesktopServices::openUrl(QUrl::fromLocalFile(currentSongItem->song()->songFileInfo().path()));
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
		logSrv->add(tr("Could not hide any item."), QU::Warning);
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
	logSrv->add(QString(tr("%1 songs added to list of invisible songs.")).arg(selectedItems.count()), QU::Information);
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
	logSrv->add(QString(tr("%1 songs are visible now.")).arg(selectedItems.count()), QU::Information);
}

/*!
 * Shows all songs from the current artist.
 */
void QUSongTree::showMoreCurrentArtist() {
	QUSongItem *currentSongItem = dynamic_cast<QUSongItem*>(this->currentItem());

	if(!currentSongItem)
		return;

	// TODO: look for more characters that could be in artist but are regexp things...
	this->filterItems(currentSongItem->song()->artist().replace("+", "\\+"), (QU::FilterModes) QU::informationTags);
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

		infoText.append(QString("<br>&nbsp;&nbsp;<b>%1 - %2</b>").arg(songItem->song()->artist(), songItem->song()->title()));
	}

	int result = QUMessageBox::information(this,
			tr("Delete Songs"),
			infoText,
			BTN << ":/control/bin.png"  << tr("Delete these songs.")
				<< ":/marks/cancel.png" << tr("Cancel delete operation."),
			270);
	if(result == 1)
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

		songDB->deleteSong(song);
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
		logSrv->add(tr("Too few songs selected. You have to merge at least 2 songs."), QU::Warning);
		return;
	}

	QUSongItem *currentSongItem = dynamic_cast<QUSongItem*>(currentItem());

	if(!currentSongItem)
		return;

	selectedItems.removeAll(currentSongItem);

	// ---------------------------------

	QString infoText = QString(tr("You want to merge songs with <b>\"%1 - %2\"</b>. All files of the other songs will be moved to that song's path.<br><br>The following <b>%3</b> songs will disappear:"))
			.arg(currentSongItem->song()->artist(), currentSongItem->song()->title(), QString::number(selectedItems.size()));

	int i = 0;
	foreach(QUSongItem *songItem, selectedItems) {
		if(++i >= 5) {
			infoText.append("<br>&nbsp;&nbsp;...");
			break;
		}

		infoText.append(QString("<br>&nbsp;&nbsp;<b>%1 - %2</b>").arg(songItem->song()->artist(), songItem->song()->title()));
	}

	int result = QUMessageBox::information(this,
			tr("Merge Songs"),
			infoText,
			BTN << ":/control/bin.png"  << tr("Merge these songs.")
				<< ":/marks/cancel.png" << tr("Cancel merge operation."),
			320);
	if(result == 1)
		return;

	// ----------------------------------

	QUProgressDialog dlg(QString(tr("Merging selected songs with \"%1 - %2\"...")).arg(currentSongItem->song()->artist(), currentSongItem->song()->title()), selectedItems.size(), this);
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
			QString target = QFileInfo(currentSongItem->song()->songFileInfo().dir(), QString("%1%2_%3").arg(i++, 3, 10, QChar('0')).arg(QTime::currentTime().toString("sszzz"), fi.fileName())).filePath();

			if(!QFile::copy(fi.filePath(), target)) {
				allFilesCopied = false;
				logSrv->add(QString(tr("Could NOT copy file \"%1\" to \"%2\".")).arg(fi.filePath(), target), QU::Warning);
			} else
				logSrv->add(QString(tr("File was copied successfully from \"%1\" to \"%2\".")).arg(fi.filePath(), target), QU::Information);
		}

		if(allFilesCopied) {
			QUSongFile *song = songItem->song();
			delete takeTopLevelItem(indexOfTopLevelItem(songItem));

			songDB->deleteSong(song);
		} else {
			logSrv->add(QString(tr("Not all files of \"%1 - %2\" were copied. Song will not be deleted. Merging failed.")).arg(songItem->song()->artist(), songItem->song()->title()), QU::Warning);
		}
	}

	currentSongItem->update();
	currentSongItem->setSelected(true);
}

/*!
 * Calculate and show the speed (singable syllables per minute) of the selected songs.
 */
void QUSongTree::calculateSpeed() {
	QList<QUSongItem*> selectedItems = selectedSongItems();

	QUProgressDialog dlg(tr("Calculate selected song speeds..."), selectedItems.size(), this);
	dlg.show();

	clearSelection();

	foreach(QUSongItem *item, selectedItems) {
		dlg.update(QString("%1 - %2").arg(item->song()->artist(), item->song()->title()));
		if(dlg.cancelled()) break;

		item->song()->syllablesPerSecond(false);
		item->update();
	}

	restoreSelection(selectedItems);
//	emit itemSelectionChanged(); // update details
}

bool QUSongTree::copyFilesToSong(const QList<QUrl> &files, QUSongItem *item) {
	if(files.size() > FILE_DROP_LIMIT) {
		int result = QUMessageBox::information(this,
				tr("Copy Files"),
				QString(tr("You want to copy <b>%1</b> files to <b>\"%2\"</b>.")).arg(files.size()).arg(QDir::toNativeSeparators(item->song()->songFileInfo().dir().path())),
				BTN << ":/control/save_all.png" << tr("Copy these files.")
					<< ":/marks/cancel.png"	 << tr("Cancel copy operation."));
		if(result == 1)
			return false;
	}

	bool dataUsed = false;

	QUProgressDialog dlg(QString(tr("Copy & Use files for the song: \"%1 - %2\"...")).arg(item->song()->artist(), item->song()->title()), files.size(), this);
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
		this->clearSelection();
		item->setSelected(true);
		this->scrollToItem(item, QAbstractItemView::EnsureVisible);
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

	QList<QUSongFile*> newSongs;

	foreach(QUrl url, urls) {
		QFileInfo fi(url.toLocalFile());

		dlg.update(fi.fileName());
		if(dlg.cancelled()) break;

		QString fileScheme("*." + fi.suffix());

		if(!QUSongSupport::allowedSongFiles().contains(fileScheme, Qt::CaseInsensitive)) {
			logSrv->add(QString(tr("Invalid song file found: \"%1\". Cannot include those.")).arg(fi.filePath()), QU::Warning);
			continue;
		}

		if(QUSongSupport::allowedLicenseFiles().contains(fi.fileName(), Qt::CaseInsensitive)) {
			logSrv->add(QString(tr("Cannot include license files as songs: \"%1\"")).arg(fi.filePath()), QU::Warning);
			continue;
		}

		QUSongFile *newSong = new QUSongFile(fi.filePath());
		QFileInfo newFileInfo = createSongFolder(newSong);

		if(!newFileInfo.dir().exists())
			continue; // dir creation did not work

		if(!QFile::copy(fi.filePath(), newFileInfo.filePath())) {
			logSrv->add(QString(tr("Could not copy song file \"%1\" to new song directory \"%2\"!")).arg(fi.fileName(), QDir::toNativeSeparators(newFileInfo.path())), QU::Warning);
			continue;
		}

		delete newSong;
		newSong = new QUSongFile(newFileInfo.filePath());
		newSongs.append(newSong);

		logSrv->add(QString(tr("New song included to your song collection: \"%1 - %2\".")).arg(newSong->artist(), newSong->title()), QU::Information);
		songDB->addSong(newSong);
	}

	// select new song items
	QUMainWindow *parentWindow = this->parentWindow();
	if(parentWindow) {
		disconnect(this, SIGNAL(itemSelectionChanged()), parentWindow, SLOT(updateDetails()));
		disconnect(this, SIGNAL(itemSelectionChanged()), parentWindow, SLOT(updatePreviewTree()));
		disconnect(this, SIGNAL(itemSelectionChanged()), parentWindow, SLOT(updateWebInfoTree()));
	}

	foreach(QUSongItem *songItem, visibleSongItems())
		if(newSongs.contains(songItem->song()))
			songItem->setSelected(true);

	if(parentWindow) {
		connect(this, SIGNAL(itemSelectionChanged()), parentWindow, SLOT(updateDetails()));
		connect(this, SIGNAL(itemSelectionChanged()), parentWindow, SLOT(updatePreviewTree()));
		connect(this, SIGNAL(itemSelectionChanged()), parentWindow, SLOT(updateWebInfoTree()));
	}

	emit itemSelectionChanged();
}

/*!
 * Try to create a basic folder for the new song according to the schema:
 * "Artist - Title"
 *
 * Set the given song to that folder.
 */
QFileInfo QUSongTree::createSongFolder(QUSongFile *song) {
	QString newDirName = QUStringSupport::withoutUnsupportedCharacters(QString("%1 - %2").arg(song->artist(), song->title())).trimmed();

	int i = 0;
	QString tmp = newDirName;
	QFileInfo fi(QU::BaseDir, newDirName);
	while(!QU::BaseDir.mkdir(tmp)) {
		if(!fi.exists()) {
			logSrv->add(QString(tr("Could not create directory: \"%1\". Disk full?")).arg(fi.filePath()), QU::Warning);
			return QFileInfo();
		} else {
			tmp = QString("%1_%2").arg(newDirName).arg(i, 3, 10, QChar('0'));
			i++;
			fi.setFile(QU::BaseDir, tmp);
		}
	}

	return QFileInfo(QDir(fi.filePath()), song->songFileInfo().fileName());
//	song->setFile( QDir(fi.filePath()).filePath(song->songFileInfo().fileName()) );
}

void QUSongTree::refreshSelectedItems() {
	QList<QUSongItem*> songItems = this->selectedSongItems();
	QList<bool>		itemExpandedStates;

	if(songItems.isEmpty())
		return;

	QUProgressDialog dlg(tr("Refreshing selected songs..."), songItems.size(), this, false);
	dlg.setPixmap(":/types/folder.png");
	dlg.show();

	clearSelection();

	foreach(QUSongItem *songItem, songItems) {
		dlg.update(songItem->song()->songFileInfo().dir().dirName());
		if(dlg.cancelled()) break;

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
		if(!itemExpandedStates.isEmpty()) {
			songItem->setExpanded(itemExpandedStates.first());
			itemExpandedStates.pop_front();
		} else
			break;
	}
	scrollToItem(currentItem());
	restoreSelection(songItems);

	emit itemSelectionChanged(); // update details
}

void QUSongTree::resizeToContents() {
	for(int i = 0; i < this->columnCount(); ++i)
		this->resizeColumnToContents(i);
}

void QUSongTree::saveSelectedSongs() {
	QList<QUSongItem*> items = this->selectedSongItems();

	if(items.isEmpty())
		return;

	QUProgressDialog dlg(tr("Saving selected songs..."), items.size(), this);
	dlg.show();

	this->clearSelection();

	foreach(QUSongItem *songItem, items) {
		dlg.update(songItem->song()->songFileInfo().fileName());
		if(dlg.cancelled()) break;

		songItem->song()->save(true);
		songItem->update();
	}

	restoreSelection(items);
}

void QUSongTree::keyPressEvent(QKeyEvent *event) {
	if(event->key() == Qt::Key_Delete) {
		this->deleteCurrentItem();
	} else {
		QTreeWidget::keyPressEvent(event);
	}
}

QMenu* QUSongTree::itemMenu(QUSongItem *item) {
	QMenu *menu = new QMenu(this);
	QFont *font = new QFont();
	font->setBold(true);

	if(item && !item->isToplevel()) {
		// file menu
		QString extension = "*" + item->text(0).remove(0,item->text(0).lastIndexOf('.')).toLower();
		if(QUSongSupport::allowedImageFiles().contains(extension)) {
			menu->addAction(QIcon(":/types/image_show.png"), tr("Show preview"), QKeySequence(Qt::Key_Enter), this, SLOT(openCurrentFileInternal()))->setFont(*font);
			menu->addAction(QIcon(":/types/image_openexternal.png"), tr("Open image file externally"), this, SLOT(openCurrentFileExternal()));
		} else if(QUSongSupport::allowedVideoFiles().contains(extension)) {
			menu->addAction(QIcon(":/types/video_openexternal.png"), tr("Open video file externally"), QKeySequence(Qt::Key_Enter), this, SLOT(openCurrentFileExternal()))->setFont(*font);
		} else if(QUSongSupport::allowedAudioFiles().contains(extension)) {
			menu->addAction(QIcon(":/types/music_openexternal.png"), tr("Open audio file externally"), QKeySequence(Qt::Key_Enter), this, SLOT(openCurrentFileExternal()))->setFont(*font);
		} else if(QUSongSupport::allowedSongFiles().contains(extension)) {
			menu->addAction(QIcon(":/types/text.png"), tr("Show file contents"), QKeySequence(Qt::Key_Enter), this, SLOT(openCurrentFileInternal()))->setFont(*font);
			menu->addAction(QIcon(":/types/text_openexternal.png"), tr("Open text file externally"), QKeySequence(Qt::SHIFT | Qt::Key_Enter), this, SLOT(openCurrentFileExternal()));
			if(item->song()->isFriend(item->text(0))) {
				menu->addAction(QIcon(":/types/text_setprimary.png"), tr("Set as primary song"), this, SLOT(setCurrentFilePrimary()));
			}
		} else {
			menu->addAction(QIcon(":/types/file.png"), tr("Open externally"), QKeySequence(Qt::Key_Enter), this, SLOT(openCurrentFileExternal()))->setFont(*font);;
		}
		menu->addAction(QIcon(":/control/bin.png"), tr("Delete"), QKeySequence(Qt::Key_Delete), this, SLOT(deleteCurrentItem()));
	} else {
		// song/folder menu
		menu->addAction(QIcon(":/control/refresh.png"), tr("Refresh"),	QKeySequence(Qt::Key_F5), this, SLOT(refreshSelectedItems()));
		menu->addAction(QIcon(":/control/save.png"),	tr("Save"),		QKeySequence(Qt::CTRL  | Qt::Key_S), this, SLOT(saveSelectedSongs()));
		menu->addAction(QIcon(":/control/bin.png"),     tr("Delete"),	QKeySequence(Qt::SHIFT | Qt::Key_Delete), this, SLOT(requestDeleteSelectedSongs()));
		QAction *a = menu->addAction(QIcon(":/control/merge.png"), tr("Merge"), QKeySequence(Qt::CTRL | Qt::Key_M), this, SLOT(mergeSelectedSongs()));
		if(selectedItems().size() < 2) a->setEnabled(false);
		menu->addAction(QIcon(":/player/play.png"), tr("Play"), this, SIGNAL(playSelectedSongsRequested()));

		menu->addSeparator();
		menu->addAction(QIcon(":/control/playlist_to.png"), tr("Send To Playlist"), QKeySequence(Qt::CTRL | Qt::Key_P), this, SLOT(sendSelectedSongsToPlaylist()));
		menu->addAction(QIcon(":/control/image_go.png"), tr("Get Covers..."), this, SLOT(requestCovers()));

		QMenu *pictureFlowMenu = menu->addMenu(QIcon(":/control/images.png"), tr("Review pictures"));
		pictureFlowMenu->addAction(QIcon(":/types/cover.png"),	  tr("Covers..."),	  this, SLOT(requestCoverFlow()));
		pictureFlowMenu->addAction(QIcon(":/types/background.png"), tr("Backgrounds..."), this, SLOT(requestBackgroundFlow()));

		menu->addMenu(hideMenu());

		menu->addAction(QIcon(":/marks/speed_slow_turtle.png"), tr("Calculate Song Speed"), this, SLOT(calculateSpeed()));

		menu->addSeparator();
#ifdef Q_OS_WIN
		menu->addAction(QIcon(":/types/folder.png"),tr("Open With Explorer..."), QKeySequence(Qt::CTRL | Qt::Key_Return), this, SLOT(openCurrentFolder()));
#endif
#ifdef Q_OS_MAC
		menu->addAction(QIcon(":/types/folder.png"),tr("Open With Finder..."), QKeySequence(Qt::CTRL | Qt::Key_Return), this, SLOT(openCurrentFolder()));
#endif
#ifdef Q_OS_LINUX
		menu->addAction(QIcon(":/types/folder.png"),tr("Open With File Manager..."), QKeySequence(Qt::CTRL | Qt::Key_Return), this, SLOT(openCurrentFolder()));
#endif
		menu->addAction(QIcon(":/types/user.png"), tr("Find More From Artist"), this, SLOT(showMoreCurrentArtist()));
		menu->addAction(QIcon(":/types/text.png"), tr("Show Lyrics..."), QKeySequence(Qt::CTRL | Qt::Key_L), this, SLOT(requestLyrics()));
		menu->addAction(QIcon(":/control/text_edit.png"), tr("Edit Lyrics..."), QKeySequence(Qt::CTRL | Qt::Key_E), this, SLOT(requestEditLyrics()));

		menu->addSeparator();
		menu->addAction(QIcon(":/faviconAAE.ico"),tr("Search for cover on AlbumArtExchange..."), this, SLOT(searchForCoverOnAAE()));
		menu->addAction(QIcon(":/faviconGoogle.ico"),tr("Search for cover on Google Images..."), this, SLOT(searchForCoverOnGoogleImages()));
		menu->addAction(QIcon(":/faviconFanart.ico"),tr("Search for artwork on Fanart..."), this, SLOT(searchForArtworkOnFanart()));
		menu->addAction(QIcon(":/faviconGoogle.ico"),tr("Search for background on Google Images..."), this, SLOT(searchForBackgroundOnGoogleImages()));
		menu->addAction(QIcon(":/faviconGoogle.ico"),tr("Search for video on Google Video..."), this, SLOT(searchForVideoOnGoogleVideo()));
		menu->addAction(QIcon(":/faviconYoutube.png"),tr("Search for video on Youtube..."), this, SLOT(searchForVideoOnYoutube()));
		menu->addAction(QIcon(":/faviconHitparade.png"),tr("Look up on Hitparade.ch..."), this, SLOT(lookUpOnHitparade()));
		menu->addAction(QIcon(":/faviconUSDB.ico"),tr("Look up on USDB..."), this, SLOT(lookUpOnUSDB()));

		if ( item->song()->songFileInfo().dir().entryList(QStringList("*.txt"), QDir::Files).size() == 2 ) {
			menu->addSeparator();
			menu->addAction(QIcon(":/types/duet.png"),tr("Create Duet From Song Files"), this, SLOT(createDuetFromSongFiles()));
		}

	}

	return menu;
}

QMenu* QUSongTree::generalMenu() {
	QMenu *menu = new QMenu(this);

	menu->addMenu(hideMenu());

	return menu;
}

/*!
 * Delete the current item, which should represent a file and not a folder
 * because folder items can be selected and are top-level.
 */
void QUSongTree::deleteCurrentItem() {
	QUSongItem *item = dynamic_cast<QUSongItem*>(this->currentItem());

	if(!item || item->isToplevel())
		return; // only allow to delete files which cannot be selected - no directories

	int result = QUMessageBox::information(this,
			tr("Delete File"),
			QString(tr("<b>\"%1\"</b> will be deleted permanently. You cannot undo a delete operation.")).arg(item->text(FOLDER_COLUMN)),
			BTN << ":/control/bin.png"  << tr("Delete this file.")
				<< ":/marks/cancel.png" << tr("Cancel delete operation."));
	if(result == 1)
		return;

	if(QFile::remove(QFileInfo(item->song()->songFileInfo().dir(), item->text(FOLDER_COLUMN)).filePath()))
		logSrv->add(QString(tr("The file \"%1\" was deleted successfully.")).arg(item->text(FOLDER_COLUMN)), QU::Information);
	else
		logSrv->add(QString(tr("The file \"%1\" was NOT deleted.")).arg(item->text(FOLDER_COLUMN)), QU::Warning);

	this->setCurrentItem(item->parent());
	item->update();
	this->scrollToItem(this->currentItem(), QAbstractItemView::EnsureVisible);

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
		logSrv->add(tr("You can only display the lyrics of one song at a time."), QU::Information);
}

void QUSongTree::requestEditLyrics() {
	QUSongItem *cItem = dynamic_cast<QUSongItem*>(this->currentItem());

	if(cItem)
		emit editLyricsRequested(cItem->song());

	if(selectedSongItems().size() > 1)
		logSrv->add(tr("You can only edit the lyrics of one song at a time."), QU::Information);
}

/*!
 * Request covers for all selected songs (song items). Send the items so that they can be updated,
 * if songs got new covers.
 */
void QUSongTree::requestCovers() {
	emit coversRequested(this->selectedSongItems());
}

void QUSongTree::requestCoverFlow() {
	emit coverFlowRequested(this->selectedSongItems());
}

void QUSongTree::requestBackgroundFlow() {
	emit backgroundFlowRequested(this->selectedSongItems());
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

/*!
 * Look up song at hitparade.ch, e.g. for correct spelling and release year
 */
void QUSongTree::lookUpOnHitparade() {
	QUSongItem *songItem = dynamic_cast<QUSongItem*>(this->currentItem());

	if(songItem) {
		QUrl url("http://hitparade.ch/search.asp");
		QUrlQuery urlQuery;
		urlQuery.addQueryItem("cat", "s");
		QString queryString = songItem->song()->artist() + " " + songItem->song()->title();

		// hitparade.ch only allows search queries up to 50 characters
		while(queryString.length() > 50) {
			queryString = queryString.left(queryString.lastIndexOf(' '));
		}

		urlQuery.addQueryItem("search", queryString.replace("’", "'"));
		url.setQuery(urlQuery);
		QDesktopServices::openUrl(url);
	}

	if(selectedSongItems().size() > 1) {
		logSrv->add(tr("You can only look up one song at a time."), QU::Information);
	}
}

/*!
 * Look up song at www.albumartexchange.com to find a high quality cover
 */
void QUSongTree::searchForCoverOnAAE() {
	QUSongItem *songItem = dynamic_cast<QUSongItem*>(this->currentItem());

	if(songItem) {
		QUrl url("https://www.albumartexchange.com/covers");
		QUrlQuery urlQuery;
		static const QRegularExpression regex("(\\s+)");
		QString queryString = QString(songItem->song()->artist() + " " + songItem->song()->title()).replace(regex, "+").replace("’", "'");
		urlQuery.addQueryItem("q", queryString);
		urlQuery.addQueryItem("fltr", "ALL");
		urlQuery.addQueryItem("sort", "TITLE");
		urlQuery.addQueryItem("status", "");
		urlQuery.addQueryItem("size", "any");
		url.setQuery(urlQuery);
		QDesktopServices::openUrl(url);
	}

	if(selectedSongItems().size() > 1) {
		logSrv->add(tr("You can only look up one song at a time."), QU::Information);
	}
}

/*!
 * Look up song at images.google.com to find a cover
 */
void QUSongTree::searchForCoverOnGoogleImages() {
	QUSongItem *songItem = dynamic_cast<QUSongItem*>(this->currentItem());

	if(songItem) {
		QUrl url("https://www.google.com/search");
		QUrlQuery urlQuery;
		urlQuery.addQueryItem("safe", "off");
		urlQuery.addQueryItem("tbm", "isch");
		urlQuery.addQueryItem("tbs", "imgo:1");
		urlQuery.addQueryItem("q", QString(songItem->song()->artist() + " " + songItem->song()->title()).replace("’", "'"));
		url.setQuery(urlQuery);

		QDesktopServices::openUrl(url);
	}

	if(selectedSongItems().size() > 1) {
		logSrv->add(tr("You can only look up one song at a time."), QU::Information);
	}
}

/*!
 * Look up song at images.google.com to find a background
 */
void QUSongTree::searchForBackgroundOnGoogleImages() {
	QUSongItem *songItem = dynamic_cast<QUSongItem*>(this->currentItem());

	if(songItem) {
		QUrl url("https://www.google.com/search");
		QUrlQuery urlQuery;
		urlQuery.addQueryItem("safe", "off");
		urlQuery.addQueryItem("tbm", "isch");
		urlQuery.addQueryItem("tbs", "imgo:1,isz:lt,islt:2mp");
		urlQuery.addQueryItem("q", songItem->song()->artist().replace("’", "'"));
		url.setQuery(urlQuery);

		QDesktopServices::openUrl(url);
	}

	if(selectedSongItems().size() > 1) {
		logSrv->add(tr("You can only look up one song at a time."), QU::Information);
	}
}

/*!
 * Look up song at fanart.tv to find cover/background artwork
 */
void QUSongTree::searchForArtworkOnFanart() {
	QUSongItem *songItem = dynamic_cast<QUSongItem*>(this->currentItem());

	if(songItem) {
		QUrl url("https://fanart.tv/");
		QUrlQuery urlQuery;
		urlQuery.addQueryItem("sect", "2");
		// There seems to be a bug on fanart.tv that does not return any results if the artist contains an apostrophe
		static const QRegularExpression regex("[’|']\\w+");
		urlQuery.addQueryItem("s", songItem->song()->artist().remove(regex));
		url.setQuery(urlQuery);

		QDesktopServices::openUrl(url);
	}

	if(selectedSongItems().size() > 1) {
		logSrv->add(tr("You can only look up one song at a time."), QU::Information);
	}
}

/*!
 * Look up song at video.google.com to find a video
 */
void QUSongTree::searchForVideoOnGoogleVideo() {
	QUSongItem *songItem = dynamic_cast<QUSongItem*>(this->currentItem());

	if(songItem) {
		QUrl url("https://www.google.com/search");
		QUrlQuery urlQuery;
		urlQuery.addQueryItem("safe", "off");
		urlQuery.addQueryItem("tbm", "vid");
		urlQuery.addQueryItem("q", songItem->song()->artist() + " " + songItem->song()->title());
		url.setQuery(urlQuery);

		QDesktopServices::openUrl(url);
	}

	if(selectedSongItems().size() > 1) {
		logSrv->add(tr("You can only look up one song at a time."), QU::Information);
	}
}

/*!
 * Look up song at youtube.com to find a video
 */
void QUSongTree::searchForVideoOnYoutube() {
	QUSongItem *songItem = dynamic_cast<QUSongItem*>(this->currentItem());

	if(songItem) {
		QUrl url("https://www.youtube.com/results");
		QUrlQuery urlQuery;
		urlQuery.addQueryItem("search_query", songItem->song()->artist() + " " + songItem->song()->title());
		url.setQuery(urlQuery);

		QDesktopServices::openUrl(url);
	}

	if(selectedSongItems().size() > 1) {
		logSrv->add(tr("You can only look up one song at a time."), QU::Information);
	}
}

/*!
 * Look up song at usdb.animux.de, e.g. to see if it is already available there or has been updated
 */
void QUSongTree::lookUpOnUSDB() {
	QUSongItem *songItem = dynamic_cast<QUSongItem*>(this->currentItem());

	if(songItem) {
		QUrl url("http://usdb.animux.de/");
		QUrlQuery urlQuery;
		urlQuery.addQueryItem("link", "list");
		urlQuery.addQueryItem("interpret", songItem->song()->artist().replace("’", "'"));
		urlQuery.addQueryItem("title", songItem->song()->title().replace("’", "'"));
		url.setQuery(urlQuery);

		QDesktopServices::openUrl(url);
	}

	if(selectedSongItems().size() > 1) {
		logSrv->add(tr("You can only look up one song at a time."), QU::Information);
	}
}

/*!
 * Create Duet From Song Files
 */
void QUSongTree::createDuetFromSongFiles() {
	QUSongItem *songItem = dynamic_cast<QUSongItem*>(this->currentItem());

	if(songItem) {
		QString duetFilename = songItem->song()->path() + QDir::separator() + songItem->song()->artist() + " - " + songItem->song()->title() + " (duet).txt";
		QUSongFile *duet = new QUSongFile(duetFilename);

		duet->setInfo(ARTIST_TAG, songItem->song()->artist());
		duet->setInfo(TITLE_TAG, songItem->song()->title());
		duet->setInfo(MP3_TAG, songItem->song()->mp3());
		duet->setInfo(BPM_TAG, songItem->song()->bpm());
		duet->setInfo(P1_TAG, "Singer 1");
		duet->setInfo(P2_TAG, "Singer 2");

		double gapMain = QVariant(songItem->song()->gap().replace(",", ".")).toDouble();
		double gapFriend = QVariant(songItem->song()->friends().first()->gap().replace(",", ".")).toDouble();
		//logSrv->add("gapMain: " + QString::number(int(gapMain)), QU::Information);
		if(gapMain < gapFriend) {
			duet->setInfo(GAP_TAG, songItem->song()->gap());
		} else {
			duet->setInfo(GAP_TAG, songItem->song()->friends().first()->gap());
		}

		duet->saveMelody();
		// TODO: adjust gap, prepend P1, append P2 and corresponding lyrics
		duet->save();
	}
}
