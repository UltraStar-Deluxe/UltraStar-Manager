#include "QUSongTree.h"
#include "QUSongItem.h"
#include "QUMainWindow.h"

#include <QMessageBox>
#include <QUrl>
#include <QIcon>
#include <QMenu>
#include <QList>

#include "QUProgressDialog.h"

#define FILE_DROP_LIMIT 10

QUSongTree::QUSongTree(QWidget *parent): QTreeWidget(parent) {
	this->setAcceptDrops(true);
	this->setContextMenuPolicy(Qt::CustomContextMenu);
	
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));
}

void QUSongTree::initHorizontalHeader() {
	QTreeWidgetItem *header = new QTreeWidgetItem();
	header->setText(0, QString("Folder (%1)").arg(QUMainWindow::BaseDir.path()));
	header->setIcon(0, QIcon(":/types/folder.png"));

	header->setIcon(1, QIcon(":/types/user.png"));
	header->setToolTip(1, tr("Shows whether your folder includes the artist correctly:<br><i>Artist - Title ...</i>"));
	header->setIcon(2, QIcon(":/types/font.png"));
	header->setToolTip(2, tr("Shows whether your folder includes the title correctly:<br><i>Artist - Title ...</i>"));
	header->setIcon(3, QIcon(":/types/music.png"));
	header->setToolTip(3, tr("Shows whether the song text file points to an <b>audio file</b> that can be found by UltraStar"));
	header->setIcon(4, QIcon(":/types/picture.png"));
	header->setToolTip(4, tr("Shows whether the song text file points to a <b>cover picture</b> that can be found by UltraStar"));
	header->setIcon(5, QIcon(":/types/picture.png"));
	header->setToolTip(5, tr("Shows whether the song text file points to a <b>background picture</b> that can be found by UltraStar"));
	header->setIcon(6, QIcon(":/types/film.png"));
	header->setToolTip(6, tr("Shows whether the song text file points to a <b>video file</b> that can be found by UltraStar"));

	header->setText(7, tr("Language"));
	header->setIcon(7, QIcon(":/types/language.png"));
	header->setText(8, tr("Edition"));
	header->setIcon(8, QIcon(":/types/edition.png"));
	header->setText(9, tr("Genre"));
	header->setIcon(9, QIcon(":/types/genre.png"));
	header->setText(10, tr("Year"));
	header->setIcon(10, QIcon(":/types/date.png"));
	header->setText(11, tr("Creator"));
	header->setIcon(11, QIcon(":/types/creator.png"));
	header->setText(12, tr("Comment"));
	header->setIcon(12, QIcon(":/types/comment.png"));
	
	this->setHeaderItem(header);	
}

bool QUSongTree::dropMimeData (QTreeWidgetItem *parent, int index, const QMimeData *data, Qt::DropAction action) {
	if(!parent || data->urls().isEmpty())
		return false;
	
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
	
	this->fillContextMenu(menu, point);
	
	menu.exec(this->mapToGlobal(point));
}

/*!
 * Appends actions to the menu according to the selected item.
 */
void QUSongTree::fillContextMenu(QMenu &menu, const QPoint &point) {
	QUSongItem *item = dynamic_cast<QUSongItem*>(this->currentItem());
	
	if(!item || item->isToplevel())
		return;
	
	menu.addSeparator();
	menu.addAction(QIcon(":/control/bin.png"), tr("Delete"), this, SLOT(deleteCurrentItem()), QKeySequence::fromString("Del"));
}

bool QUSongTree::copyFilesToSong(const QList<QUrl> &files, QUSongItem *item) {
	if(files.size() > FILE_DROP_LIMIT)
		if(QMessageBox::question(this, tr("Confirm Copy Operation"), QString(tr("You want to copy %1 files. Are you sure?")).arg(files.size()), 
			QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No)
			return false;
	
	bool dataUsed = false;
	
	QUProgressDialog dlg(QString("Copy & Use files for the song: \"%1 - %2\"...").arg(item->song()->artist()).arg(item->song()->title()), files.size(), this);
	dlg.setPixmap(":/marks/disk.png");
	dlg.show();
	
	foreach(QUrl url, files) {
		dlg.update(url.toLocalFile());
		
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

void QUSongTree::refreshSelectedItems() {
	QList<QTreeWidgetItem*> items = this->selectedItems();
	
	if(items.isEmpty())
		items.append(this->currentItem());
	
	QUProgressDialog dlg("Refreshing selected songs...", items.size(), this);
	dlg.setPixmap(":/types/folder.png");
	dlg.show();
	
	foreach(QTreeWidgetItem *item, items) {
		QUSongItem *songItem = dynamic_cast<QUSongItem*>(item);
		
		if(songItem) {
			dlg.update(songItem->song()->songFileInfo().dir().dirName());
			
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
	
	if( QMessageBox::question(this, tr("Confirm Delete Operation"), QString(tr("Do you really want to delete \"%1\"?")).arg(item->text(0)),
			QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No )
		return;
	
	if(QFile::remove(QFileInfo(item->song()->songFileInfo().dir(), item->text(0)).filePath()))
		emit finished(QString(tr("The file \"%1\" was deleted successfully.")).arg(item->text(0)), QU::information);
	else
		emit finished(QString(tr("The file \"%1\" was NOT deleted.")).arg(item->text(0)), QU::warning);
	
	this->setCurrentItem(item->parent());
	item->update();
	
	emit itemSelectionChanged(); // update details
}
