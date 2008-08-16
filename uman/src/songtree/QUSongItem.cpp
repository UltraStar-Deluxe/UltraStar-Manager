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
			QStringList("*"), QDir::Files);

	qDeleteAll(this->takeChildren());

	for(int i = 0; i < fileNames.size(); i++) {
		child = new QUSongItem(song());
		this->addChild(child);

		child->setText(FOLDER_COLUMN, fileNames[i]); // set filename for first column
		child->setFlags(Qt::ItemIsEnabled); // do not allow selection

		QString fileScheme("*." + QFileInfo(fileNames[i]).suffix());

		if(QUSongFile::allowedSongFiles().contains(fileScheme, Qt::CaseInsensitive)) {
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

	this->setSelected(true);
}

/*!
 * Format this item as toplevel directory item.
 */
void QUSongItem::updateAsDirectory(bool showRelativePath) {
	clearContents();

	if(showRelativePath)
		this->setText(FOLDER_COLUMN, QUMainWindow::BaseDir.relativeFilePath(song()->songFileInfo().path()));
	else
		this->setText(FOLDER_COLUMN, song()->songFileInfo().dir().dirName());

	if(song()->unsupportedTagsFound()) {
		this->setIcon(FOLDER_COLUMN, QIcon(":/types/folder_blue.png"));
		this->setToolTip(FOLDER_COLUMN, QObject::tr("Unsupported Tags:\n#") + song()->unsupportedTags());
	} else {
		this->setIcon(FOLDER_COLUMN, QIcon(":/types/folder.png"));
		this->setToolTip(FOLDER_COLUMN, "");
	}

	QRegExp r("\\[.*\\]");
	r.setMinimal(true);

	// create a pattern without any folder tags: [SC], [VIDEO], a.s.o.
	// TOFIX: Fix bug for "Die Ärzte - Der - Titel"
	QString pattern(song()->songFileInfo().dir().dirName().remove(r).trimmed());

	if(QString::compare(QUSongFile::withoutUnsupportedCharacters(song()->artist()), pattern.section(" - ", 0, 0), Qt::CaseSensitive) == 0)
		this->setTick(ARTIST_COLUMN);
	else if(QString::compare(QUSongFile::withoutUnsupportedCharacters(song()->artist()), pattern.section(" - ", 0, 0), Qt::CaseInsensitive) == 0)
		this->setTick(ARTIST_COLUMN, true);
	else
		this->setCross(ARTIST_COLUMN);

	if(QString::compare(QUSongFile::withoutUnsupportedCharacters(song()->title()), pattern.section(" - ", 1, 1), Qt::CaseSensitive) == 0)
		this->setTick(TITLE_COLUMN);
	else if(QString::compare(QUSongFile::withoutUnsupportedCharacters(song()->title()), pattern.section(" - ", 1, 1), Qt::CaseInsensitive) == 0)
		this->setTick(TITLE_COLUMN, true);
	else
		this->setCross(TITLE_COLUMN);

	if(song()->hasMp3())        this->setTick(MP3_COLUMN);        else if(song()->mp3() != N_A) this->setCross(MP3_COLUMN, true);               else this->setCross(MP3_COLUMN);
	if(song()->hasCover())      this->setTick(COVER_COLUMN);      else if(song()->cover() != N_A) this->setCross(COVER_COLUMN, true);           else this->setCross(COVER_COLUMN);
	if(song()->hasBackground()) this->setTick(BACKGROUND_COLUMN); else if(song()->background() != N_A) this->setCross(BACKGROUND_COLUMN, true); else this->setCross(BACKGROUND_COLUMN);
	if(song()->hasVideo())      this->setTick(VIDEO_COLUMN);      else if(song()->video() != N_A) this->setCross(VIDEO_COLUMN, true);           else this->setCross(VIDEO_COLUMN);

	this->setText(LANGUAGE_COLUMN, song()->language());
	this->setText(EDITION_COLUMN,  song()->edition());
	this->setText(GENRE_COLUMN,    song()->genre());
	this->setText(YEAR_COLUMN,     song()->year());
	this->setText(CREATOR_COLUMN,  song()->creator());
	this->setText(COMMENT_COLUMN,  song()->comment());

	// indicate unsaved changes
	// TODO: Do we really need to create a QFont object here? No convenience?!
	QFont f(this->font(FOLDER_COLUMN));

	if(song()->hasUnsavedChanges())
		f.setBold(true);
	else
		f.setBold(false);

	this->setFont(FOLDER_COLUMN, f);
}

void QUSongItem::updateAsTxt() {
	clearContents();

	this->setIcon(FOLDER_COLUMN, QIcon(":/types/text.png"));

	if(QString::compare(this->text(FOLDER_COLUMN), song()->songFileInfo().fileName(), Qt::CaseInsensitive) != 0) {
		this->setTextColor(FOLDER_COLUMN, Qt::gray); // unnecessary song text file, not used
//		QFont f(this->font(FOLDER_COLUMN));
//		f.setStrikeOut(true);
//		this->setFont(FOLDER_COLUMN, f);

		// Show that there are multiple songs in this folder available.
		(dynamic_cast<QUSongItem*>(this->parent()))->showMultipleSongsIcon();
	} else {
		this->setTextColor(FOLDER_COLUMN, Qt::blue);
	}
}

void QUSongItem::updateAsMp3() {
	clearContents();

	this->setIcon(FOLDER_COLUMN, QIcon(":/types/music.png"));

	if(QString::compare(song()->mp3(), this->text(FOLDER_COLUMN), Qt::CaseInsensitive) == 0)
		this->setIcon(MP3_COLUMN, QIcon(":/marks/link.png"));
	else {
		this->setTextColor(FOLDER_COLUMN, Qt::gray); // unused mp3
		(dynamic_cast<QUSongItem*>(this->parent()))->showUnusedFilesIcon();
	}
}

void QUSongItem::updateAsPicture() {
	clearContents();

	this->setIcon(FOLDER_COLUMN, QIcon(":/types/picture.png"));

	bool used = false;

	if(QString::compare(song()->cover(), this->text(FOLDER_COLUMN), Qt::CaseInsensitive) == 0) {
		this->setIcon(COVER_COLUMN, QIcon(":/marks/link.png"));
		used = true;
	}

	if(QString::compare(song()->background(), this->text(FOLDER_COLUMN), Qt::CaseInsensitive) == 0) {
		this->setIcon(BACKGROUND_COLUMN, QIcon(":/marks/link.png"));
		used = true;
	}

	if(!used) {
		this->setTextColor(FOLDER_COLUMN, Qt::gray);
		(dynamic_cast<QUSongItem*>(this->parent()))->showUnusedFilesIcon();
	}
}

void QUSongItem::updateAsVideo() {
	clearContents();

	this->setIcon(FOLDER_COLUMN, QIcon(":/types/film.png"));

	if(QString::compare(song()->video(), this->text(FOLDER_COLUMN), Qt::CaseInsensitive) == 0)
		this->setIcon(VIDEO_COLUMN, QIcon(":/marks/link.png"));
	else {
		this->setTextColor(FOLDER_COLUMN, Qt::gray);
		(dynamic_cast<QUSongItem*>(this->parent()))->showUnusedFilesIcon();
	}
}

void QUSongItem::updateAsUnknown() {
	this->setTextColor(FOLDER_COLUMN, Qt::gray);
	QFont f(this->font(FOLDER_COLUMN));
	f.setStrikeOut(true);
	this->setFont(FOLDER_COLUMN, f);

	(dynamic_cast<QUSongItem*>(this->parent()))->showUnusedFilesIcon();
}

void QUSongItem::showUnusedFilesIcon() {
	this->setIcon(UNUSED_FILES_COLUMN, QIcon(":/types/unused_files.png"));

	// used for sorting, should be smaller than zero
	this->setData(UNUSED_FILES_COLUMN, Qt::UserRole, QVariant(-1));
}

void QUSongItem::showMultipleSongsIcon() {
	this->setIcon(MULTIPLE_SONGS_COLUMN, QIcon(":/types/text_stack.png"));

	// used for sorting, should be smaller than zero
	this->setData(MULTIPLE_SONGS_COLUMN, Qt::UserRole, QVariant(-1));
}

void QUSongItem::setTick(int column, bool isBlue) {
	if(isBlue) {
		this->setIcon(column, QIcon(":/marks/tick_blue.png"));
		// used for sorting, should be greater than a "cross" icon
		this->setData(column, Qt::UserRole, QVariant(1));
	} else {
		this->setIcon(column, QIcon(":/marks/tick.png"));
		// used for sorting, should be greater than a "cross" icon
		this->setData(column, Qt::UserRole, QVariant(2));
	}
}

void QUSongItem::setCross(int column, bool isBlue) {
	if(isBlue) {
		this->setIcon(column, QIcon(":/marks/cross_blue.png"));
		// used for sorting, should be smaller than a "tick" icon
		this->setData(column, Qt::UserRole, QVariant(-1));
	} else {
		this->setIcon(column, QIcon(":/marks/cross.png"));
		// used for sorting, should be smaller than a "tick" icon
		this->setData(column, Qt::UserRole, QVariant(0));
	}
}

/*!
 * Replace base function completely. See Qt source code.
 *
 * Used to sort "icon only" columns.
 */
bool QUSongItem::operator< (const QTreeWidgetItem &other) const {
	int column = treeWidget() ? treeWidget()->sortColumn() : 0;

	switch(column) {
	case ARTIST_COLUMN:
	case TITLE_COLUMN:
	case MP3_COLUMN:
	case COVER_COLUMN:
	case BACKGROUND_COLUMN:
	case VIDEO_COLUMN:
	case UNUSED_FILES_COLUMN:
	case MULTIPLE_SONGS_COLUMN:
		return this->data(column, Qt::UserRole).toInt() < other.data(column, Qt::UserRole).toInt(); break;
	default:
		return text(column) < other.text(column);
	}
}
