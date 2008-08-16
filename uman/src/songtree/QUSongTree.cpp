#include "QUSongTree.h"
#include "QUSongItem.h"
#include "QUSongFile.h"
#include "QUMainWindow.h"
#include "QUMessageBox.h"

#include <QMessageBox>
#include <QUrl>
#include <QIcon>
#include <QMenu>
#include <QList>
#include <QRegExp>
#include <QByteArray>

#include "QUProgressDialog.h"

#define FILE_DROP_LIMIT 5

QUSongTree::QUSongTree(QWidget *parent): QTreeWidget(parent) {
	this->setAcceptDrops(true);
	this->setContextMenuPolicy(Qt::CustomContextMenu);

	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));
}

void QUSongTree::initHorizontalHeader() {
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
//	header->setText(COMMENT_COLUMN, tr("Comment"));
//	header->setIcon(COMMENT_COLUMN, QIcon(":/types/comment.png"));

	this->setHeaderItem(header);
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

	return songs;
}

/*!
 * Creates a new entry for each given song.
 */
void QUSongTree::fill(QList<QUSongFile*> songs) {
	foreach(QUSongFile* song, songs) {
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
	this->addTopLevelItems(_hiddenItems);
	_hiddenItems.clear();

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

	foreach(QTreeWidgetItem *item, topLevelItems) {
		QUSongItem *songItem = dynamic_cast<QUSongItem*>(item);

		if(songItem) {
			QUSongFile *song = songItem->song();

			progress.update(QString("%1 - %2").arg(song->artist()).arg(song->title()));
			if(progress.cancelled()) break;

			bool filterInfo = true;
			bool filterFile = true;
			bool filterCtrl = true;

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

			if(
				(filterInfo && filterFile && filterCtrl && !(mode.testFlag(QU::negateFilter)))
				||
				(!(filterInfo && filterFile && filterCtrl) && (mode.testFlag(QU::negateFilter)))
			)
				_hiddenItems.append(this->takeTopLevelItem(this->indexOfTopLevelItem(item)));
		}
	}

	emit itemSelectionChanged(); // update details
	emit finished(QString(tr("Filter applied: \"%1\"")).arg(regexp), QU::information);
}

bool QUSongTree::dropMimeData (QTreeWidgetItem *parent, int index, const QMimeData *data, Qt::DropAction action) {
	if(data->urls().isEmpty())
		return false;

	if(!parent)
		return dropSongFiles(data->urls());

	QUSongItem *item = dynamic_cast<QUSongItem*>(parent);

	if(!item)
		return false;

	return this->copyFilesToSong(data->urls(), item);
}

QStringList QUSongTree::mimeTypes() const {
	return QStringList("text/uri-list");
}

/*!
 * Shows a context menu with actions for all selected items/songs.
 */
void QUSongTree::showContextMenu(const QPoint &point) {
	if(!this->itemAt(point))
		return; // no item clicked

	QMenu menu(this);

	menu.addAction(QIcon(":/control/refresh.png"), tr("Refresh"), this, SLOT(refreshSelectedItems()), QKeySequence::fromString("F5"));
	menu.addAction(QIcon(":/control/save.png"), tr("Save"), this, SLOT(saveSelectedSongs()), QKeySequence::fromString("Ctrl+S"));
	menu.addAction(QIcon(":/control/playlist_to.png"), tr("Send To Playlist"), this, SLOT(sendSelectedSongsToPlaylist()), QKeySequence::fromString("Ctrl+P"));

	this->fillContextMenu(menu, point);

	menu.exec(this->mapToGlobal(point));
}

/*!
 * Appends actions to the menu according to the selected item/file.
 */
void QUSongTree::fillContextMenu(QMenu &menu, const QPoint &point) {
	QUSongItem *item = dynamic_cast<QUSongItem*>(this->currentItem());

	if(!item || item->isToplevel())
		return;

	menu.addSeparator();
	menu.addAction(QIcon(":/control/bin.png"), tr("Delete"), this, SLOT(deleteCurrentItem()), QKeySequence::fromString("Del"));
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

		if(!QFileInfo(url.toLocalFile()).isDir()) {
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
bool QUSongTree::dropSongFiles(const QList<QUrl> &urls) {
	foreach(QUrl url, urls) {
		QFileInfo fi(url.toLocalFile());
		QString fileScheme("*." + fi.suffix());

		if(QUSongFile::allowedSongFiles().contains(fileScheme, Qt::CaseInsensitive)) {
			QUSongFile tmp(fi.filePath());

			QString newSongDirName = QString("%1 - %2").arg(tmp.artist()).arg(tmp.title());

			if(QUMainWindow::BaseDir.mkdir(newSongDirName)) {
				QDir newSongDir(QUMainWindow::BaseDir);
				if(newSongDir.cd(newSongDirName)) {
					QFileInfo newFi(newSongDir, fi.fileName());
					if(QFile::copy(fi.filePath(), newFi.filePath())) {
						QUSongFile *newSong = new QUSongFile(newFi.filePath());
						QUSongItem *newItem = new QUSongItem(newSong, true);
						this->addTopLevelItem(newItem);

						newItem->setSelected(true);

						emit finished(QString(tr("New song included to your song collection: \"%1 - %2\".")).arg(newSong->artist()).arg(newSong->title()), QU::information);
						emit songCreated(newSong);
					} else {
						emit finished(QString(tr("Could not copy song file \"%1\" to new song directory \"%2\"!")).arg(fi.fileName()).arg(newSongDirName), QU::warning);
					}
				} else {
					emit finished(QString(tr("Could not change to new song directory \"%1\"!")).arg(newSongDirName), QU::warning);
				}
			} else {
				emit finished(QString(tr("Could not create new song directory \"%1\"!")).arg(newSongDirName), QU::warning);
			}
		}
	}

	return true;
}

void QUSongTree::refreshSelectedItems() {
	QList<QTreeWidgetItem*> items = this->selectedItems();

	if(items.isEmpty())
		items.append(this->currentItem());

	QUProgressDialog dlg(tr("Refreshing selected songs..."), items.size(), this, false);
	dlg.setPixmap(":/types/folder.png");
	dlg.show();

	foreach(QTreeWidgetItem *item, items) {
		QUSongItem *songItem = dynamic_cast<QUSongItem*>(item);

		if(songItem) {
			dlg.update(songItem->song()->songFileInfo().dir().dirName());
			if(dlg.cancelled()) break;

			songItem->update();
		}
	}

	emit itemSelectionChanged(); // update details
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
