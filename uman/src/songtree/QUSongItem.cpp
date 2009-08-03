#include "QUSongItem.h"

#include "QUSongTree.h"

#include <QString>
#include <QStringList>
#include <QBrush>
#include <QFileInfo>
#include <QDir>
#include <QIcon>
#include <QRegExp>
#include <QSettings>
#include <QMessageBox>

#include "QUSongSupport.h"
#include "QUStringSupport.h"

#define LOWER_SPEED_BOUND_DEFAULT 2.5
#define UPPER_SPEED_BOUND_DEFAULT 5.5

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
		this->setBackgroundColor(i, QColor(0, 0, 0, 0));
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

		if(QUSongSupport::allowedLicenseFiles().contains(fileNames.at(i), Qt::CaseInsensitive)) {
			child->updateAsLicense();
		} else if(QUSongSupport::allowedSongFiles().contains(fileScheme, Qt::CaseInsensitive)) {
			child->updateAsTxt();
		} else if(QUSongSupport::allowedAudioFiles().contains(fileScheme, Qt::CaseInsensitive)) {
			child->updateAsMp3();
		} else if(QUSongSupport::allowedPictureFiles().contains(fileScheme, Qt::CaseInsensitive)) {
			child->updateAsPicture();
		} else if(QUSongSupport::allowedVideoFiles().contains(fileScheme, Qt::CaseInsensitive)) {
			child->updateAsVideo();
		} else if(QUSongSupport::allowedMidiFiles().contains(fileScheme, Qt::CaseInsensitive)) {
			child->updateAsMidi();
		} else if(QUSongSupport::allowedKaraokeFiles().contains(fileScheme, Qt::CaseInsensitive)) {
			child->updateAsKaraoke();
		} else if(QUSongSupport::allowedScoreFiles().contains(fileScheme, Qt::CaseInsensitive)) {
			child->updateAsScore();
		} else {
			child->updateAsUnknown();
		}
	}

//	this->setSelected(true);
}

/*!
 * Format this item as toplevel directory item.
 */
void QUSongItem::updateAsDirectory(bool showRelativePath) {
	clearContents();

	if(showRelativePath)
		this->setText(FOLDER_COLUMN, QU::BaseDir.relativeFilePath(song()->songFileInfo().path()));
	else
		this->setText(FOLDER_COLUMN, song()->songFileInfo().dir().dirName());

	if(song()->unsupportedTagsFound()) {
		this->setIcon(FOLDER_COLUMN, QIcon(":/types/folder_blue.png"));
		this->setToolTip(FOLDER_COLUMN, QObject::tr("Unsupported Tags:\n#") + song()->unsupportedTags());
	} else {
		this->setIcon(FOLDER_COLUMN, QIcon(":/types/folder.png"));
		this->setToolTip(FOLDER_COLUMN, "");
	}

	// indicate unsaved changes
	// TODO: Do we really need to create a QFont object here? No convenience?!
	QFont f(this->font(FOLDER_COLUMN));
	f.setBold(song()->hasUnsavedChanges());
	this->setFont(FOLDER_COLUMN, f);

	updateSpellCheckColumns();
	updateFileCheckColumns();
	updateTypeColumns();
	updateTimeCheckColumns();
	updateTextColumns();
	updateControlColumns();
	updateBackground();
}

void QUSongItem::updateAsTxt() {
	clearContents();

	this->setIcon(FOLDER_COLUMN, QIcon(":/types/text.png"));

	if(QString::compare(this->text(FOLDER_COLUMN), song()->songFileInfo().fileName(), Qt::CaseInsensitive) == 0) {
		// song itself found
		this->setTextColor(FOLDER_COLUMN, QColor(13, 86, 166, 255));

		if(song()->isDuet()) {
			this->setIcon(MULTIPLE_SONGS_COLUMN, QIcon(":/types/duet.png"));
			this->setToolTip(MULTIPLE_SONGS_COLUMN, QObject::tr("This song is a duet and for two singers."));
		} else if(song()->isKaraoke()) {
			this->setIcon(MULTIPLE_SONGS_COLUMN, QIcon(":/types/karaoke.png"));
			this->setToolTip(MULTIPLE_SONGS_COLUMN, QObject::tr("This is a karaoke song, which means that the audio file contains no singing voice."));
		}

		return;
	}

	if(song()->isFriend(this->text(FOLDER_COLUMN))) {
		// friend found
		this->setTextColor(FOLDER_COLUMN, QColor(13, 86, 166, 255));
		this->setBackgroundColor(FOLDER_COLUMN, QColor(255, 209, 64, 120));
		QUSongFile* f = song()->friendAt(this->text(FOLDER_COLUMN));
		if(f->isDuet()) {
			this->setIcon(MULTIPLE_SONGS_COLUMN, QIcon(":/types/duet.png"));
			this->setToolTip(MULTIPLE_SONGS_COLUMN, QObject::tr("This song is for two or more singers."));
		} else if(f->isKaraoke()) {
			this->setIcon(MULTIPLE_SONGS_COLUMN, QIcon(":/types/karaoke.png"));
			this->setToolTip(MULTIPLE_SONGS_COLUMN, QObject::tr("This is a karaoke song, which means that the audio file contains no singing voice."));
		}

	} else {
		// unnecessary song text file, not used --- SHOULD NEVER HAPPEN!
		this->setTextColor(FOLDER_COLUMN, Qt::gray);
	}

	// Show that there are multiple songs in this folder available.
	(dynamic_cast<QUSongItem*>(this->parent()))->showMultipleSongsIcon(this->text(FOLDER_COLUMN));
}

void QUSongItem::updateAsMp3() {
	clearContents();

	this->setIcon(FOLDER_COLUMN, QIcon(":/types/music.png"));

	if(QString::compare(song()->mp3(), this->text(FOLDER_COLUMN), Qt::CaseInsensitive) == 0) {
		this->setIcon(MP3_COLUMN, QIcon(":/marks/link.png"));
	} else if(song()->friendHasTag(MP3_TAG, this->text(FOLDER_COLUMN))) { // audio file used by friend
		this->setTextColor(FOLDER_COLUMN, QColor(13, 86, 166, 255));
		this->setBackgroundColor(FOLDER_COLUMN, QColor(255, 209, 64, 120));
	} else {
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

void QUSongItem::updateAsLicense() {
	clearContents();

	this->setIcon(FOLDER_COLUMN, QIcon(":/types/license.png"));

	// special files, special color ^_^
	this->setTextColor(FOLDER_COLUMN, Qt::darkGreen);
}

void QUSongItem::updateAsMidi() {
	clearContents();

	this->setIcon(FOLDER_COLUMN, QIcon(":/types/midi.png"));

	// special files, special color ^_^
//	this->setTextColor(FOLDER_COLUMN, Qt::darkGreen);
}

void QUSongItem::updateAsKaraoke() {
	clearContents();

	this->setIcon(FOLDER_COLUMN, QIcon(":/types/midi_kar.png"));

	// special files, special color ^_^
//	this->setTextColor(FOLDER_COLUMN, Qt::darkGreen);
}

void QUSongItem::updateAsScore() {
	clearContents();

	this->setIcon(FOLDER_COLUMN, QIcon(":/types/score.png"));

	if(song()->score() && QFileInfo(song()->score()->filePath()).fileName() == text(FOLDER_COLUMN))
		return;

	foreach(QUSongFile *friendSong, song()->friends()) {
		if(friendSong->score() && QFileInfo(friendSong->score()->filePath()).fileName() == text(FOLDER_COLUMN)) {
			// song friend uses this file!
			this->setTextColor(FOLDER_COLUMN, QColor(13, 86, 166, 255));
			this->setBackgroundColor(FOLDER_COLUMN, QColor(255, 209, 64, 120));
			return;
		}
	}

	// last option: no song uses this score file
	this->setTextColor(FOLDER_COLUMN, Qt::gray);
	(dynamic_cast<QUSongItem*>(this->parent()))->showUnusedFilesIcon(this->text(FOLDER_COLUMN));
}

void QUSongItem::updateAsUnknown() {
	this->setTextColor(FOLDER_COLUMN, Qt::gray);
	QFont f(this->font(FOLDER_COLUMN));
	f.setStrikeOut(true);
	this->setFont(FOLDER_COLUMN, f);

	(dynamic_cast<QUSongItem*>(this->parent()))->showUnusedFilesIcon(this->text(FOLDER_COLUMN));
}

/*!
 * Used to switch to an "alternative" view mode.
 * \sa altViewEnabled()
 */
void QUSongItem::updateSpellFileCheckColumns() {
	if(!isToplevel()) { // use parent (which should be toplevel) if this is not toplevel
		(dynamic_cast<QUSongItem*>(this->parent()))->updateSpellFileCheckColumns();
		return;
	}

	updateSpellCheckColumns();
	updateFileCheckColumns();
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
		this->setToolTip(MULTIPLE_SONGS_COLUMN, QString(QObject::tr("Multiple songs found:\n* %1 (primary)")).arg(song()->songFileInfo().fileName())); // headline for the tooltip

	if(!fileName.isEmpty())
		this->setToolTip(MULTIPLE_SONGS_COLUMN, QString("%1\n* %2").arg(this->toolTip(MULTIPLE_SONGS_COLUMN)).arg(fileName));

	// used for sorting, should be smaller than zero
	this->setData(MULTIPLE_SONGS_COLUMN, Qt::UserRole, QVariant(-1));
}

void QUSongItem::setTick(int column) {
	if(!altViewEnabled())
		this->setIcon(column, QIcon(":/marks/tick.png"));
	else {
		     if(column == MP3_COLUMN)        this->setIcon(column, QIcon(":/types/music.png"));
		else if(column == COVER_COLUMN)      this->setIcon(column, QIcon(":/types/cover.png"));
		else if(column == BACKGROUND_COLUMN) this->setIcon(column, QIcon(":/types/background.png"));
		else if(column == VIDEO_COLUMN)      this->setIcon(column, QIcon(":/types/film.png"));
	}

	// used for sorting, should be greater than a "cross" icon
	this->setData(column, Qt::UserRole, QVariant(1));
}

void QUSongItem::setCross(int column, bool isWarning, QString toolTip) {

	if(isWarning) {
		if(!altViewEnabled())
			this->setIcon(column, QIcon(":/marks/cross_error.png"));
		else {
				 if(column == MP3_COLUMN)        this->setIcon(column, QIcon(":/types/music_warn.png"));
			else if(column == COVER_COLUMN)      this->setIcon(column, QIcon(":/types/picture_warn.png"));
			else if(column == BACKGROUND_COLUMN) this->setIcon(column, QIcon(":/types/picture_warn.png"));
			else if(column == VIDEO_COLUMN)      this->setIcon(column, QIcon(":/types/film_warn.png"));
		}
	} else {
		if(!altViewEnabled())
			this->setIcon(column, QIcon(":/marks/cross.png"));
		else if(column == MP3_COLUMN)
			this->setIcon(column, QIcon(":/types/music_error.png"));
		else
			this->setIcon(column, QIcon());
	}

	if(isWarning)
		this->setData(column, Qt::UserRole, QVariant(-1)); // used for sorting, should be smaller than a "tick" icon
	else
		this->setData(column, Qt::UserRole, QVariant(0)); // used for sorting, should be smaller than a "tick" icon

	this->setToolTip(column, toolTip);
}

void QUSongItem::setSmiley(int column, QU::SpellStates state, QString toolTip) {
	switch(state) {
	case QU::spellingOk:
		if(!altViewEnabled())
			this->setIcon(column, QIcon(":/marks/spell_ok.png"));
		else
			this->setIcon(column, QIcon());
		this->setData(column, Qt::UserRole, QVariant(0));
		break;
	case QU::spellingWarning:
		this->setIcon(column, QIcon(":/marks/spell_warn.png"));
		this->setData(column, Qt::UserRole, QVariant(-1));
		break;
	case QU::spellingError:
		this->setIcon(column, QIcon(":/marks/spell_error.png"));
		this->setData(column, Qt::UserRole, QVariant(-2));
		break;
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
	case TYPE_DUET_COLUMN:
	case TYPE_KARAOKE_COLUMN:
	case UNUSED_FILES_COLUMN:
	case MULTIPLE_SONGS_COLUMN:
	case LENGTH_COLUMN:
	case LENGTH_DIFF_COLUMN:
	case LENGTH_MP3_COLUMN:
	case LENGTH_EFF_COLUMN:
	case SPEED_COLUMN:
		return this->data(column, Qt::UserRole).toInt() < other.data(column, Qt::UserRole).toInt(); break;
	case START_COLUMN:
	case END_COLUMN:
	case VIDEOGAP_COLUMN:
	case BPM_COLUMN:
	case GAP_COLUMN:
		return this->data(column, Qt::UserRole).toDouble() < other.data(column, Qt::UserRole).toDouble(); break;
	default:
		return text(column) < other.text(column);
	}
}

/*!
 * Checks whether an alternative version of the song tree will be shown.
 */
bool QUSongItem::altViewEnabled() {
	QSettings settings;
	return settings.value("altSongTree", false).toBool();
}

void QUSongItem::setAltViewEnabled(bool enabled) {
	QSettings settings;
	settings.setValue("altSongTree", enabled);
}

void QUSongItem::updateSpellCheckColumns() {
	QString pattern = QUStringSupport::withoutFolderTags(song()->songFileInfo().dir().dirName());
	QString toolTip = "\"%2\" %3 \"%1\"";
	QString part1, part2;

	/* artist column */
	part1 = QUStringSupport::withoutUnsupportedCharacters(song()->artist());
	part1 = QUStringSupport::withoutFolderTags(part1);
	part1 = QUStringSupport::withoutPathDelimiters(part1);

	part2 = QUStringSupport::withoutUnsupportedCharacters(pattern.section(" - ", 0, 0));
	part2 = QUStringSupport::withoutPathDelimiters(part2);

	if(QString::compare(part1, part2, Qt::CaseSensitive) == 0)
		this->setSmiley(ARTIST_COLUMN);
	else if(QString::compare(part1, part2, Qt::CaseInsensitive) == 0)
		this->setSmiley(ARTIST_COLUMN, QU::spellingWarning, toolTip.arg(part1).arg(part2).arg(QObject::trUtf8(CHAR_UTF8_APPROX)));
	else
		this->setSmiley(ARTIST_COLUMN, QU::spellingError, toolTip.arg(part1).arg(part2).arg(QObject::trUtf8(CHAR_UTF8_NEQUAL)));

	/* title column */
	part1 = QUStringSupport::withoutUnsupportedCharacters(song()->title());
	part1 = QUStringSupport::withoutFolderTags(part1);
	part1 = QUStringSupport::withoutPathDelimiters(part1);

	part2 = QUStringSupport::withoutUnsupportedCharacters(pattern.section(" - ", 1));
	part2 = QUStringSupport::withoutPathDelimiters(part2);

	if(QString::compare(part1, part2, Qt::CaseSensitive) == 0)
		this->setSmiley(TITLE_COLUMN);
	else if(QString::compare(part1, part2, Qt::CaseInsensitive) == 0)
		this->setSmiley(TITLE_COLUMN, QU::spellingWarning, toolTip.arg(part1).arg(part2).arg(QObject::trUtf8(CHAR_UTF8_APPROX)));
	else
		this->setSmiley(TITLE_COLUMN, QU::spellingError, toolTip.arg(part1).arg(part2).arg(QObject::trUtf8(CHAR_UTF8_NEQUAL)));
}

void QUSongItem::updateFileCheckColumns() {
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

	// score files
	if(song()->score())
		this->setIcon(SCORE_COLUMN, QIcon(":/types/score.png"));
}

void QUSongItem::updateTypeColumns() {
	if(song()->isDuet()) {
		this->setIcon(TYPE_DUET_COLUMN, QIcon(":/types/duet.png"));
		this->setData(TYPE_DUET_COLUMN, Qt::UserRole, -1);
	}

	if(song()->isKaraoke()) {
		this->setIcon(TYPE_KARAOKE_COLUMN, QIcon(":/types/karaoke.png"));
		this->setData(TYPE_KARAOKE_COLUMN, Qt::UserRole, -1);
	}

}

void QUSongItem::updateTimeCheckColumns() {
	int length = song()->length();
	this->setTextAlignment(LENGTH_COLUMN, Qt::AlignRight);
	this->setText(LENGTH_COLUMN, length == 0 ? N_A : QString("%1:%2").arg(length / 60).arg(length % 60, 2, 10, QChar('0')));
	this->setData(LENGTH_COLUMN, Qt::UserRole, QVariant(length));
	int lengthMp3 = song()->lengthMp3();
	this->setText(LENGTH_MP3_COLUMN, lengthMp3 == 0 ? N_A : QString("%1:%2").arg(lengthMp3 / 60).arg(lengthMp3 % 60, 2, 10, QChar('0')));
	this->setData(LENGTH_MP3_COLUMN, Qt::UserRole, QVariant(lengthMp3));
	int lengthEffective = song()->lengthEffective();
	this->setText(LENGTH_EFF_COLUMN, lengthEffective == 0 ? N_A : QString("%1:%2").arg(lengthEffective / 60).arg(lengthEffective % 60, 2, 10, QChar('0')));
	this->setData(LENGTH_EFF_COLUMN, Qt::UserRole, QVariant(lengthEffective));

	QSettings settings;
	int timeDiff = lengthMp3 - length;
	int timeDiffLower = settings.value("timeDiffLower", LOWER_TIME_BOUND_DEFAULT).toInt();
	int timeDiffUpper = settings.value("timeDiffUpper", UPPER_TIME_BOUND_DEFAULT).toInt();

	if(lengthMp3 >= length and timeDiff <= timeDiffLower)
		; // well done ^_^
	else if(lengthMp3 > length and timeDiff <= timeDiffUpper)
		this->setIcon(LENGTH_DIFF_COLUMN, QIcon(":/bullets/bullet_warning.png"));
	else
		this->setIcon(LENGTH_DIFF_COLUMN, QIcon(":/bullets/bullet_cancel.png"));

	this->setData(LENGTH_DIFF_COLUMN, Qt::UserRole, QVariant(timeDiff * (-1))); // for sorting issues
	this->setToolTip(LENGTH_DIFF_COLUMN, QString(QObject::tr("%1 seconds")).arg(timeDiff));

	// show song speed
	double speed = song()->syllablesPerSecond();

	this->setData(SPEED_COLUMN, Qt::UserRole, (speed) * 1000); // for sorting issues

	if(speed == -1.0) {
		this->setText(SPEED_COLUMN, QObject::tr("Not calculated."));
		this->setToolTip(SPEED_COLUMN, QObject::tr("Calculate the speed first."));
	} else {
		this->setText(SPEED_COLUMN, QString("%1").arg(speed, 2, 'f', 1, QChar('0')));

		double speedLower = settings.value("speedLower", LOWER_SPEED_BOUND_DEFAULT).toDouble();
		double speedUpper = settings.value("speedUpper", UPPER_SPEED_BOUND_DEFAULT).toDouble();

		if(speed < speedLower)
			this->setIcon(SPEED_COLUMN, QIcon(":/marks/speed_slow_turtle.png"));
		else if(speed < speedUpper)
			this->setIcon(SPEED_COLUMN, QIcon(":/marks/speed_med_cat.png"));
		else
			this->setIcon(SPEED_COLUMN, QIcon(":/marks/speed_fast_hamster.png"));

		this->setToolTip(SPEED_COLUMN, QString("%1").arg(speed, 0, 'f'));
	}

	// show some time control tags
	this->setText(START_COLUMN, song()->start());
	this->setData(START_COLUMN, Qt::UserRole, song()->start().replace(",", "."));
	this->setText(END_COLUMN, song()->end());
	this->setData(END_COLUMN, Qt::UserRole, song()->end().replace(",", "."));
	this->setText(VIDEOGAP_COLUMN, song()->videogap());
	this->setData(VIDEOGAP_COLUMN, Qt::UserRole, song()->videogap().replace(",", "."));
}

void QUSongItem::updateTextColumns() {
	/* other tag columns */

	this->setText(ARTIST_COLUMN_EX, song()->artist());  if(song()->artist() != N_A) this->setToolTip(ARTIST_COLUMN_EX, song()->artist());
	this->setText(TITLE_COLUMN_EX,  song()->title());   if(song()->title()  != N_A) this->setToolTip(TITLE_COLUMN_EX,  song()->title());

	this->setText(LANGUAGE_COLUMN, song()->language()); if(song()->language() != N_A) this->setToolTip(LANGUAGE_COLUMN,  song()->language());
	this->setText(EDITION_COLUMN,  song()->edition());  if(song()->edition()  != N_A) this->setToolTip(EDITION_COLUMN,   song()->edition());
	this->setText(GENRE_COLUMN,    song()->genre());    if(song()->genre()    != N_A) this->setToolTip(GENRE_COLUMN,     song()->genre());
	this->setText(YEAR_COLUMN,     song()->year());     if(song()->year()     != N_A) this->setToolTip(YEAR_COLUMN,      song()->year());
	this->setText(CREATOR_COLUMN,  song()->creator());  if(song()->creator()  != N_A) this->setToolTip(CREATOR_COLUMN,   song()->creator());

	// show custom tags
	int i = 0;
	foreach(QString customTag, QUSongSupport::availableCustomTags()) {
		this->setText(FIRST_CUSTOM_TAG_COLUMN + (i),      song()->customTag(customTag));
		this->setToolTip(FIRST_CUSTOM_TAG_COLUMN + (i++), song()->customTag(customTag));
	}
}

void QUSongItem::updateControlColumns() {
	setText(RELATIVE_COLUMN, song()->relative());
	setIcon(RELATIVE_COLUMN, QIcon(song()->isRelative() ? ":/marks/tick.png": ":/marks/cross.png"));

	setText(BPM_COLUMN, song()->bpm());
	setData(BPM_COLUMN, Qt::UserRole, song()->bpm().replace(",", "."));

	setText(GAP_COLUMN, song()->gap());
	setData(GAP_COLUMN, Qt::UserRole, song()->gap().replace(",", "."));
}

void QUSongItem::updateBackground() {
	if(!song()->isValid())
		for(int i = 0; i < this->columnCount(); i++)
			this->setBackgroundColor(i, QColor(255, 48, 48, 50));
	else
		for(int i = 0; i < this->columnCount(); i++)
			this->setBackgroundColor(i, QColor(0, 0, 0, 0));

}
