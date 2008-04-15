#include "QUSongTree.h"
#include "QUSongItem.h"
#include "QUMainWindow.h"

#include <QMessageBox>
#include <QUrl>
#include <QIcon>
#include <QMenu>

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
	
	if(data->urls().size() > FILE_DROP_LIMIT)
		if(QMessageBox::question(this, tr("Confirm Copy Operation"), QString(tr("You want to copy %1 files. Are you sure?")).arg(data->urls().size()), 
			QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No)
			return false;
	
	bool dataUsed = false;
	
	QUProgressDialog dlg(QString("Copy & Use files for the song: \"%1 - %2\"...").arg(item->song()->artist()).arg(item->song()->title()), data->urls().size(), this);
	dlg.setPixmap(":/marks/disk.png");
	dlg.show();
	
	foreach(QUrl url, data->urls()) {
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

QStringList QUSongTree::mimeTypes() const {
	return QStringList("text/uri-list");
}

void QUSongTree::showContextMenu(const QPoint &point) {
	QUSongItem *item = dynamic_cast<QUSongItem*>(this->itemAt(point));
	
	if(!item)
		return;
	
	QMenu menu(this);
	menu.addAction(tr("Hey!"));
	
	
	menu.exec(this->mapToGlobal(point));
	// TODO: Implement Context Menu
}
