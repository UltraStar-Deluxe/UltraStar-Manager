#include "QUSongItem.h"
#include "QUMainWindow.h"

#include <QString>
#include <QStringList>
#include <QBrush>
#include <QFileInfo>
#include <QDir>
#include <QIcon>
#include <QRegExp>
#include <QSettings>
#include <QMessageBox>

QUSongItem::QUSongItem(QUSongFile *song, bool isToplevel): 
	QTreeWidgetItem(), 
	_song(song), 
	_isToplevel(isToplevel) {
	if(isToplevel)
		update();
}

/*!
 * Removes all icons from the cells in order to reset them.
 * Resets font color to black.
 */
void QUSongItem::clearContents() {
	for(int i = 0; i < this->columnCount(); i++) {
		this->setIcon(i, QIcon());
		this->setTextColor(i, Qt::black);
	}
}

/*!
 * If this is a toplevel item, update itself as directory and
 * its children as necessary.
 */
void QUSongItem::update() {
	if(!isToplevel()) { // use parent (which should be toplevel) if this is not toplevel
		(dynamic_cast<QUSongItem*>(this->parent()))->update();
		return;
	}
	
	QSettings settings;
	this->updateAsDirectory(settings.value("showRelativeSongPath", QVariant(true)).toBool());
	
	QUSongItem *child;
	QStringList fileNames = song()->songFileInfo().dir().entryList(
			QStringList("*.txt") + QUSongFile::allowedAudioFiles() + QUSongFile::allowedPictureFiles() + QUSongFile::allowedVideoFiles(), 
			QDir::Files);
	
	qDeleteAll(this->takeChildren());
	
	for(int i = 0; i < fileNames.size(); i++) {
		child = new QUSongItem(song());
		this->addChild(child);
				
		child->setText(0, fileNames[i]); // set filename for first column
		child->setFlags(Qt::ItemIsEnabled); // do not allow selection
		
		QString fileScheme("*." + QFileInfo(fileNames[i]).suffix());
		
		if(QString::compare(fileScheme, "*.txt", Qt::CaseInsensitive) == 0) {
			child->updateAsTxt();
		} else if(QUSongFile::allowedAudioFiles().contains(fileScheme, Qt::CaseInsensitive)) {
			child->updateAsMp3();
		} else if(QUSongFile::allowedPictureFiles().contains(fileScheme, Qt::CaseInsensitive)) {
			child->updateAsPicture();
		} else if(QUSongFile::allowedVideoFiles().contains(fileScheme, Qt::CaseInsensitive)) {
			child->updateAsVideo();
		} else {
			child->updateAsUnknown();
		}
	}
	
	if(this->treeWidget())
		this->treeWidget()->setCurrentItem(this);
}

/*!
 * Format this item as toplevel directory item.
 */
void QUSongItem::updateAsDirectory(bool showRelativePath) {
	clearContents();
	
	if(showRelativePath)
		this->setText(0, QUMainWindow::_baseDir.relativeFilePath(song()->songFileInfo().path()));
	else
		this->setText(0, song()->songFileInfo().dir().dirName());
	
	if(song()->unsupportedTagsFound()) {
		this->setIcon(0, QIcon(":/types/folder_blue.png"));
		this->setToolTip(0, "Unsupported Tags:\n#" + song()->unsupportedTags());
	} else {
		this->setIcon(0, QIcon(":/types/folder.png"));
		this->setToolTip(0, "");
	}
	
	QRegExp r("\\[.*\\]");
	r.setMinimal(true);
	
	QString pattern(song()->songFileInfo().dir().dirName().remove(r).trimmed());
	
	if(QString::compare(song()->artist(), pattern.section(" - ", 0, 0), Qt::CaseSensitive) == 0)
		this->setIcon(1, QIcon(":/marks/tick.png"));
	else if(QString::compare(song()->artist(), pattern.section(" - ", 0, 0), Qt::CaseInsensitive) == 0)
		this->setIcon(1, QIcon(":/marks/tick_blue.png"));
	else
		this->setIcon(1, QIcon(":/marks/cross.png"));

	if(QString::compare(song()->title(), pattern.section(" - ", 1, 1), Qt::CaseSensitive) == 0)
		this->setIcon(2, QIcon(":/marks/tick.png"));
	else if(QString::compare(song()->title(), pattern.section(" - ", 1, 1), Qt::CaseInsensitive) == 0)
		this->setIcon(2, QIcon(":/marks/tick_blue.png"));
	else
		this->setIcon(2, QIcon(":/marks/cross.png"));

	if(song()->hasMp3()) this->setIcon(3, QIcon(":/marks/tick.png")); else this->setIcon(3, QIcon(":/marks/cross.png")); 
	if(song()->hasCover()) this->setIcon(4, QIcon(":/marks/tick.png")); else this->setIcon(4, QIcon(":/marks/cross.png"));
	if(song()->hasBackground()) this->setIcon(5, QIcon(":/marks/tick.png")); else this->setIcon(5, QIcon(":/marks/cross.png"));
	if(song()->hasVideo()) this->setIcon(6, QIcon(":/marks/tick.png")); else this->setIcon(6, QIcon(":/marks/cross.png"));
	
	this->setText(7, song()->language());
	this->setText(8, song()->edition());
	this->setText(9, song()->genre());
	this->setText(10, song()->year());
	this->setText(11, song()->creator());
}

void QUSongItem::updateAsTxt() {	
	clearContents();
	
	this->setIcon(0, QIcon(":/types/text.png"));
	
	if(QString::compare(this->text(0), song()->songFileInfo().fileName(), Qt::CaseInsensitive) != 0)
		this->setTextColor(0, Qt::gray); // unnecessary song text file, not used
	else
		this->setTextColor(0, Qt::blue);
}

void QUSongItem::updateAsMp3() {
	clearContents();
	
	this->setIcon(0, QIcon(":/types/music.png"));
	
	if(QString::compare(song()->mp3(), this->text(0), Qt::CaseInsensitive) == 0)
		this->setIcon(3, QIcon(":/marks/link.png"));
	else
		this->setTextColor(0, Qt::gray); // unused mp3
}

void QUSongItem::updateAsPicture() {
	clearContents();
	
	this->setIcon(0, QIcon(":/types/picture.png"));
	
	bool used = false;
		
	if(QString::compare(song()->cover(), this->text(0), Qt::CaseInsensitive) == 0) {
		this->setIcon(4, QIcon(":/marks/link.png"));
		used = true; 
	}
	
	if(QString::compare(song()->background(), this->text(0), Qt::CaseInsensitive) == 0) {
		this->setIcon(5, QIcon(":/marks/link.png"));
		used = true;
	}
		
	if(!used)
		this->setTextColor(0, Qt::gray);
}

void QUSongItem::updateAsVideo() {
	clearContents();
	
	this->setIcon(0, QIcon(":/types/film.png"));
	
	if(QString::compare(song()->video(), this->text(0), Qt::CaseInsensitive) == 0)
		this->setIcon(6, QIcon(":/marks/link.png"));
	else
		this->setTextColor(0, Qt::gray);				
}

void QUSongItem::updateAsUnknown() {
	this->setTextColor(0, Qt::gray);
}

/*!
 * Uses all child items (files) to guess missing files according to some
 * common patterns: "cover", "[CO]" -> Cover; "back", "[BG]", -> Background, a.s.o.
 */
void QUSongItem::autoSetFiles() {
	if(!isToplevel()) { // use parent (which should be toplevel) if this is not toplevel
		(dynamic_cast<QUSongItem*>(this->parent()))->autoSetFiles();
		return;
	}
	
	for(int i = 0; i < this->childCount(); i++) {
		QString fileName(this->child(i)->text(0));
		QString fileScheme("*." + QFileInfo(fileName).suffix());
		
		if(QUSongFile::allowedAudioFiles().contains(fileScheme, Qt::CaseInsensitive)) {
			if(!song()->hasMp3())
				song()->setInfo(MP3_TAG, fileName);
		} else if(QUSongFile::allowedVideoFiles().contains(fileScheme, Qt::CaseInsensitive)) {
			if(!song()->hasVideo())
				song()->setInfo(VIDEO_TAG, fileName);
		} else if(QUSongFile::allowedPictureFiles().contains(fileScheme, Qt::CaseInsensitive)) {
			QRegExp reCover("\\[CO\\]|cover", Qt::CaseInsensitive);
			QRegExp reBackground("\\[BG\\]|back", Qt::CaseInsensitive);
			
			if(fileName.contains(reCover) and !song()->hasCover())
				song()->setInfo(COVER_TAG, fileName);
			else if(fileName.contains(reBackground) and !song()->hasBackground())
				song()->setInfo(BACKGROUND_TAG, fileName);
		}
	}
}
