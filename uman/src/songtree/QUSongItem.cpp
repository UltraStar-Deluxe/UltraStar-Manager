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
		this->setToolTip(i, "");
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

		if(QU::allowedSongFiles().contains(fileScheme, Qt::CaseInsensitive)) {
			child->updateAsTxt();
		} else if(QU::allowedAudioFiles().contains(fileScheme, Qt::CaseInsensitive)) {
			child->updateAsMp3();
		} else if(QU::allowedPictureFiles().contains(fileScheme, Qt::CaseInsensitive)) {
			child->updateAsPicture();
		} else if(QU::allowedVideoFiles().contains(fileScheme, Qt::CaseInsensitive)) {
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

	QString pattern = QU::withoutFolderTags(song()->songFileInfo().dir().dirName());
	QString toolTip = "\"%2\" %3 \"%1\"";
	QString part1, part2;

	/* artist column */
	part1 = QU::withoutUnsupportedCharacters(song()->artist());
	part1 = QU::withoutFolderTags(part1);
	part2 = QU::withoutUnsupportedCharacters(pattern.section(" - ", 0, 0));

	if(QString::compare(part1, part2, Qt::CaseSensitive) == 0)
		this->setTick(ARTIST_COLUMN);
	else if(QString::compare(part1, part2, Qt::CaseInsensitive) == 0)
		this->setTick(ARTIST_COLUMN, true, toolTip.arg(part1).arg(part2).arg(QObject::trUtf8(CHAR_UTF8_APPROX)));
	else
		this->setCross(ARTIST_COLUMN, false, toolTip.arg(part1).arg(part2).arg(QObject::trUtf8(CHAR_UTF8_NEQUAL)));

	/* title column */
	part1 = QU::withoutUnsupportedCharacters(song()->title());
	part1 = QU::withoutFolderTags(part1);
	part2 = QU::withoutUnsupportedCharacters(pattern.section(" - ", 1));

	if(QString::compare(part1, part2, Qt::CaseSensitive) == 0)
		this->setTick(TITLE_COLUMN);
	else if(QString::compare(part1, part2, Qt::CaseInsensitive) == 0)
		this->setTick(TITLE_COLUMN, true, toolTip.arg(part1).arg(part2).arg(QObject::trUtf8(CHAR_UTF8_APPROX)));
	else
		this->setCross(TITLE_COLUMN, false, toolTip.arg(part1).arg(part2).arg(QObject::trUtf8(CHAR_UTF8_NEQUAL)));

	/* file-related columns */

	     if(song()->hasMp3())     this->setTick(MP3_COLUMN);
	else if(song()->mp3() != N_A) this->setCross(MP3_COLUMN, true, QString(QObject::tr("File not found: \"%1\"")).arg(song()->mp3()));
	else                          this->setCross(MP3_COLUMN);

	     if(song()->hasCover())     this->setTick(COVER_COLUMN);
	else if(song()->cover() != N_A) this->setCross(COVER_COLUMN, true, QString(QObject::tr("File not found: \"%1\"")).arg(song()->cover()));
	else                            this->setCross(COVER_COLUMN);

	     if(song()->hasBackground())     this->setTick(BACKGROUND_COLUMN);
	else if(song()->background() != N_A) this->setCross(BACKGROUND_COLUMN, true, QString(QObject::tr("File not found: \"%1\"")).arg(song()->background()));
	else                                 this->setCross(BACKGROUND_COLUMN);

	     if(song()->hasVideo())     this->setTick(VIDEO_COLUMN);
	else if(song()->video() != N_A) this->setCross(VIDEO_COLUMN, true, QString(QObject::tr("File not found: \"%1\"")).arg(song()->video()));
	else                            this->setCross(VIDEO_COLUMN);

	/* other tag columns */

	this->setText(ARTIST_COLUMN_EX, song()->artist());
	this->setText(TITLE_COLUMN_EX,  song()->title());

	this->setText(LANGUAGE_COLUMN, song()->language());
	this->setText(EDITION_COLUMN,  song()->edition());
	this->setText(GENRE_COLUMN,    song()->genre());
	this->setText(YEAR_COLUMN,     song()->year());
	this->setText(CREATOR_COLUMN,  song()->creator());

	this->setText(LENGTH_COLUMN, QString("%1:%2").arg(song()->length() / 60).arg(song()->length() % 60, 2, 10, QChar('0')));

	// show custom tags
	int i = 0;
	foreach(QString customTag, QUSongFile::customTags()) {
		this->setText(FIRST_CUSTOM_TAG_COLUMN + (i++), song()->customTag(customTag));
	}

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
		// unnecessary song text file, not used
		this->setTextColor(FOLDER_COLUMN, Qt::gray);

		// Show that there are multiple songs in this folder available.
		(dynamic_cast<QUSongItem*>(this->parent()))->showMultipleSongsIcon(this->text(FOLDER_COLUMN));
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
		(dynamic_cast<QUSongItem*>(this->parent()))->showUnusedFilesIcon(this->text(FOLDER_COLUMN));
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
		(dynamic_cast<QUSongItem*>(this->parent()))->showUnusedFilesIcon(this->text(FOLDER_COLUMN));
	}
}

void QUSongItem::updateAsVideo() {
	clearContents();

	this->setIcon(FOLDER_COLUMN, QIcon(":/types/film.png"));

	if(QString::compare(song()->video(), this->text(FOLDER_COLUMN), Qt::CaseInsensitive) == 0)
		this->setIcon(VIDEO_COLUMN, QIcon(":/marks/link.png"));
	else {
		this->setTextColor(FOLDER_COLUMN, Qt::gray);
		(dynamic_cast<QUSongItem*>(this->parent()))->showUnusedFilesIcon(this->text(FOLDER_COLUMN));
	}
}

void QUSongItem::updateAsUnknown() {
	this->setTextColor(FOLDER_COLUMN, Qt::gray);
	QFont f(this->font(FOLDER_COLUMN));
	f.setStrikeOut(true);
	this->setFont(FOLDER_COLUMN, f);

	(dynamic_cast<QUSongItem*>(this->parent()))->showUnusedFilesIcon(this->text(FOLDER_COLUMN));
}

void QUSongItem::showUnusedFilesIcon(QString fileName) {
	this->setIcon(UNUSED_FILES_COLUMN, QIcon(":/types/unused_files.png"));

	if(this->toolTip(UNUSED_FILES_COLUMN).isEmpty())
		this->setToolTip(UNUSED_FILES_COLUMN, QObject::tr("Unused files found:")); // headline for the tooltip

	if(!fileName.isEmpty())
		this->setToolTip(UNUSED_FILES_COLUMN, QString("%1\n* %2").arg(this->toolTip(UNUSED_FILES_COLUMN)).arg(fileName));

	// used for sorting, should be smaller than zero
	this->setData(UNUSED_FILES_COLUMN, Qt::UserRole, QVariant(-1));
}

void QUSongItem::showMultipleSongsIcon(QString fileName) {
	this->setIcon(MULTIPLE_SONGS_COLUMN, QIcon(":/types/text_stack.png"));

	if(this->toolTip(MULTIPLE_SONGS_COLUMN).isEmpty())
		this->setToolTip(MULTIPLE_SONGS_COLUMN, QString(QObject::tr("Multiple songs found:\n* %1 (active)")).arg(song()->songFileInfo().fileName())); // headline for the tooltip

	if(!fileName.isEmpty())
		this->setToolTip(MULTIPLE_SONGS_COLUMN, QString("%1\n* %2").arg(this->toolTip(MULTIPLE_SONGS_COLUMN)).arg(fileName));

	// used for sorting, should be smaller than zero
	this->setData(MULTIPLE_SONGS_COLUMN, Qt::UserRole, QVariant(-1));
}

void QUSongItem::setTick(int column, bool isBlue, QString toolTip) {
	if(isBlue) {
		this->setIcon(column, QIcon(":/marks/tick_blue.png"));
		// used for sorting, should be greater than a "cross" icon
		this->setData(column, Qt::UserRole, QVariant(1));
	} else {
		this->setIcon(column, QIcon(":/marks/tick.png"));
		// used for sorting, should be greater than a "cross" icon
		this->setData(column, Qt::UserRole, QVariant(2));
	}

	this->setToolTip(column, toolTip);
}

void QUSongItem::setCross(int column, bool isBlue, QString toolTip) {
	if(isBlue) {
		this->setIcon(column, QIcon(":/marks/cross_blue.png"));
		// used for sorting, should be smaller than a "tick" icon
		this->setData(column, Qt::UserRole, QVariant(-1));
	} else {
		this->setIcon(column, QIcon(":/marks/cross.png"));
		// used for sorting, should be smaller than a "tick" icon
		this->setData(column, Qt::UserRole, QVariant(0));
	}

	this->setToolTip(column, toolTip);
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
