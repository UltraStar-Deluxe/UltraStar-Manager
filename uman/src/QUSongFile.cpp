#include "QUSongFile.h"
#include "QUMonty.h"
#include "QUMainWindow.h"
#include "QUMessageBox.h"

#include <QByteArray>
#include <QVariant>
#include <QDir>
#include <QSettings>
#include <QMessageBox>
#include <QFileInfoList>
#include <QApplication>
#include <QFile>

#include "audioproperties.h"
#include "fileref.h"
#include "tag.h"
#include "tstring.h"

/*!
 * Creates a new song file object.
 * \param file an existing US song file (normally a *.txt)
 * \param parent parent for Qt object tree
 *
 * \sa allowedSongFiles()
 */
QUSongFile::QUSongFile(const QString &filePath, QObject *parent): QObject(parent), _hasUnsavedChanges(false) {
	this->setFile(filePath);
}

QUSongFile::~QUSongFile() {
	disconnect(this, 0, 0, 0);
}

/*!
 * Here you can set up a new file that will be used for this song.
 */
void QUSongFile::setFile(const QString &file) {
	_fi.setFile(file);
	updateCache();
}

/* SORTING FUNCTIONS BEGIN */

bool QUSongFile::artistLessThan (QUSongFile *s1, QUSongFile *s2)   { return QString::compare(s1->artist(), s2->artist(), Qt::CaseInsensitive) < 0; }
bool QUSongFile::titleLessThan (QUSongFile *s1, QUSongFile *s2)    { return QString::compare(s1->title(), s2->title(), Qt::CaseInsensitive) < 0; }
bool QUSongFile::languageLessThan (QUSongFile *s1, QUSongFile *s2) { return QString::compare(s1->language(), s2->language(), Qt::CaseInsensitive) < 0; }
bool QUSongFile::editionLessThan (QUSongFile *s1, QUSongFile *s2)  { return QString::compare(s1->edition(), s2->edition(), Qt::CaseInsensitive) < 0; }
bool QUSongFile::genreLessThan (QUSongFile *s1, QUSongFile *s2)    { return QString::compare(s1->genre(), s2->genre(), Qt::CaseInsensitive) < 0; }
bool QUSongFile::yearLessThan (QUSongFile *s1, QUSongFile *s2)     { return QString::compare(s1->year(), s2->year(), Qt::CaseInsensitive) < 0; }
bool QUSongFile::creatorLessThan (QUSongFile *s1, QUSongFile *s2)  { return QString::compare(s1->creator(), s2->creator(), Qt::CaseInsensitive) < 0; }

bool QUSongFile::pathLessThan (QUSongFile *s1, QUSongFile *s2)             { return QString::compare(s1->path(), s2->path(), Qt::CaseInsensitive) < 0; }
bool QUSongFile::filePathLessThan (QUSongFile *s1, QUSongFile *s2)         { return QString::compare(s1->filePath(), s2->filePath(), Qt::CaseInsensitive) < 0; }
bool QUSongFile::relativeFilePathLessThan (QUSongFile *s1, QUSongFile *s2) { return QString::compare(s1->relativeFilePath(), s2->relativeFilePath(), Qt::CaseInsensitive) < 0; }

bool QUSongFile::hasMp3LessThan (QUSongFile *s1, QUSongFile *s2)         { return !s1->hasMp3() && s2->hasMp3(); }
bool QUSongFile::hasCoverLessThan (QUSongFile *s1, QUSongFile *s2)       { return !s1->hasCover() && s2->hasCover(); }
bool QUSongFile::hasBackgroundLessThan (QUSongFile *s1, QUSongFile *s2)  { return !s1->hasBackground() && s2->hasBackground(); }
bool QUSongFile::hasVideoLessThan (QUSongFile *s1, QUSongFile *s2)       { return !s1->hasVideo() && s2->hasVideo(); }

/* SORTING FUNCTIONS END */

/* COMPARING FUNCTINOS BEGIN */

/*!
 * Used to check whether two songs are nearly equal -> duplicate detection!
 *
 * Artist and title of the songs are being normalized (e.g. lower case, remove
 * special characters and fill-words, ...)
 *
 * soundex, metaphone
 *
 * remove any vocals
 *
 * QChar::decompositionTag()
 * QChar::decomposition()
 */
bool QUSongFile::equal(QUSongFile *s1, QUSongFile *s2) {
	return ( QU::equal(s1->artist(), s2->artist()) and QU::equal(s1->title(), s2->title(), true) );
}

/* COMPARING FUNCTINOS END */

/*!
 * \returns The relative file path of this song to the base dir.
 */
QString QUSongFile::relativeFilePath() const {
	return QUMainWindow::BaseDir.relativeFilePath(_fi.filePath());
}

/*!
 * Reads the US data file and loads all data into memory. This is needed to be done
 * before any changes can be made. You cannot send event messages here because a song
 * is connected _after_ the first update of the internal cache.
 * \returns True on success, otherwise false.
 */
bool QUSongFile::updateCache() {
	QFile _file;
	_file.setFileName(_fi.filePath());

	if(!_file.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;

	// clear contents
	_footer.clear();
	_lyrics.clear();
	_info.clear();
	_foundUnsupportedTags.clear();

	/*
	 * Read the header and write all tags into memory. The headers end is assumed
	 * when a line starts with ':', '*', 'F', 'E' or '-'. See the UltraStar documentation
	 * about information to these lines.
	 */
	QString line;
	while( !(QRegExp("[:\\*FE\\-].*").exactMatch(line) || _file.atEnd()) ) {
		line = QU::withoutLeadingBlanks(QString::fromLocal8Bit(_file.readLine()));

		// read supported tags
		bool isSupported = false;
		foreach(QString tag, QUSongFile::tags()) {
			if(line.startsWith("#" + tag + ":", Qt::CaseInsensitive)) {
				setInfo(tag, line.section("#" + tag + ":", 0, 0, QString::SectionSkipEmpty | QString::SectionCaseInsensitiveSeps).trimmed());
				isSupported = true;
			}
		}

		// read unsupported tags
		if(!isSupported and line.startsWith("#")) {
			QString uTag(line.section(":", 0, 0).remove("#").trimmed());
			QString uValue(line.section("#" + uTag + ":", 0, 0, QString::SectionSkipEmpty).trimmed());

			if(!uTag.isEmpty() and !uValue.isEmpty()) {
				setInfo(uTag, uValue);
				_foundUnsupportedTags << uTag;
			}
		}
	}

	// read lyrics + other stuff (distinct them)
	while( !_file.atEnd() ) {
		if(QRegExp("[:\\*F\\-].*").exactMatch(line))
			_lyrics << line;
		else if(QString::compare(line.trimmed(), "E", Qt::CaseInsensitive) != 0 && !line.isEmpty())
			_footer << line;

		line = QU::withoutLeadingBlanks(QString::fromLocal8Bit(_file.readLine()));
	}

	// use last line buffer
	// TODO: a little bit dirty here (duplicate code)
	if(QRegExp("[:\\*F\\-].*").exactMatch(line))
		_lyrics << line;
	else if(QString::compare(line.trimmed(), "E", Qt::CaseInsensitive) != 0 && !line.isEmpty())
		_footer << line;

	_file.close();
	return true;
}

/*!
 * \param tag The tag you want to set/overwrite.
 * \param value The desired value for the tag.
 * \sa tags()
 */
void QUSongFile::setInfo(const QString &tag, const QString &value) {
	if(value == "") {
		_info.take(tag.toUpper());
	} else {
		_info[tag.toUpper()] = value.trimmed();
	}

	emit dataChanged();
}

/*!
 * \returns A list of strings with all available tags. Custom tags are included.
 */
QStringList QUSongFile::tags() {
	QStringList result;

	result << TITLE_TAG;
	result << ARTIST_TAG;
	result << LANGUAGE_TAG;
	result << EDITION_TAG;
	result << GENRE_TAG;
	result << YEAR_TAG;
	result << CREATOR_TAG;
	result << MP3_TAG;
	result << COVER_TAG;
	result << BACKGROUND_TAG;
	result << VIDEO_TAG;
	result << VIDEOGAP_TAG;
	result << START_TAG;
	result << END_TAG;
	result << RELATIVE_TAG;
	result << BPM_TAG;
	result << GAP_TAG;

	// all custom tags that will be supported
	result << customTags();

	return result;
}

QStringList QUSongFile::customTags() {
	QSettings settings;
	QStringList result = settings.value("customTags", "").toString().split(" ");

	for(int i = 0; i < result.size(); i++)
		if(result.at(i).isEmpty())
			result.removeAt(i);

	return result;
}

QStringList QUSongFile::noteTypes() {
	QStringList result;

	result << ":"; // normal
	result << "*"; // golden note
	result << "F"; // freestyle
	result << "-"; // pause

	return result;
}

/*!
 * Checks whether the configuration information about the
 * tag order is correct (especially the count of the tags).
 */
void QUSongFile::verifyTags(QStringList &tags) {
	QSettings settings;

	if(tags.size() != QUSongFile::tags().size()) {
		settings.setValue("tagOrder", QVariant(QUSongFile::tags()));

		QUMessageBox::ask(QApplication::activeWindow(),
				tr("Deprecated tag information detected"),
				tr(
						"The number of available tags in your configuration and that one this application offers are different.<br><br>"
						"The tag order was reset to its <b>default order</b>. Check out the <b>options</b> to set up your custom order again."
				),
				":/marks/accept.png", tr("Continue"));

		tags = QUSongFile::tags();
	}
}

/*!
 * Checks whether the mp3 file really exists and can be used by US.
 * \returns True if the mp3 specified by the song file really exists.
 */
bool QUSongFile::hasMp3() const {
	return mp3FileInfo().exists();
}

/*!
 * Checks whether the cover picture file really exists and can be used by US.
 * \returns True if the cover specified by the song file really exists.
 */
bool QUSongFile::hasCover() const {
	return coverFileInfo().exists();
}

/*!
 * Checks whether the background picture file really exists and can be used by US.
 * \returns True if the background specified by the song file really exists.
 */
bool QUSongFile::hasBackground() const {
	return backgroundFileInfo().exists();
}

/*!
 * Checks whether the video file really exists and can be used by US.
 * \returns True if the video specified by the song file really exists.
 */
bool QUSongFile::hasVideo() const {
	return videoFileInfo().exists();
}

/*!
 * Checks whether the song file is "checked" (SC).
 */
bool QUSongFile::isSongChecked() const {
	return edition().contains("[SC]", Qt::CaseInsensitive);
}

/*!
 * \returns the length of this song accoring to BPM and length of lyrics, in seconds
 */
int QUSongFile::length() const {
	if(_lyrics.isEmpty())
		return 0;

	double bpm = QVariant(this->bpm().replace(",", ".")).toDouble();

	if(bpm == 0.0)
		return 0;

	int beats = 0;
	if(QString::compare(this->relative(), "yes", Qt::CaseInsensitive) == 0) {
		foreach(QString line, _lyrics) {
			if(line.startsWith("-"))
				beats += QVariant(line.section(" ", 2, 2, QString::SectionSkipEmpty)).toInt(); // e.g. "- 48 64"
		}
	} else
		beats = QVariant(_lyrics.last().section(" ", 1, 1, QString::SectionSkipEmpty)).toInt(); // use the number in the last line, e.g. ": 2000 5 60 boo", "- 2000"

	double gap = QVariant(this->gap()).toDouble() / 1000;

	return (int)((beats / (bpm * 4)) * 60 + gap); // result in seconds
}

/*!
 * \returns the length of the audio file, if one exists. That length can be reset by the #END tag.
 */
int QUSongFile::lengthMp3() const {
	TagLib::FileRef ref(this->mp3FileInfo().absoluteFilePath().toLocal8Bit().data());

	if(ref.isNull() or !ref.audioProperties())
		return 0;

	if(this->end() != N_A)
		return qMax(
				0,
				qMin(
						(int)(QVariant(this->end()).toDouble() / 1000),
						ref.audioProperties()->length()) );
	else
		return ref.audioProperties()->length();
}

int QUSongFile::lengthEffective() const {
	return qMax(0, this->lengthMp3() - QVariant(this->start()).toInt());
}

/*!
 * \returns the song lyrics in a human readable format
 */
QStringList QUSongFile::lyrics() const {
	QStringList _lyricsCopy = _lyrics;
	QStringList result; result << QString();

	QRegExp lineBreak("\\s*-.*");
	QRegExp linePrefix("\\s*[:\\*F]\\s*\\d+\\s+\\d+\\s+-?\\d+\\s");

	int lastBeat = -1;

	foreach(QString line, _lyricsCopy) {
		if(lineBreak.exactMatch(line)) {
			result << QString();

			line = line.remove("-").trimmed();
			lastBeat = QVariant(line.section(" ", 0, 0, QString::SectionSkipEmpty)).toInt();

			// insert an empty line?
			if(this->relative() != N_A) {
				int nextBeat = QVariant(line.section(" ", 1, 1, QString::SectionSkipEmpty)).toInt();
				if(nextBeat - lastBeat > 20)
					result << QString();
				lastBeat = -1; // reset beat counter
			}
		} else {
			if(lastBeat != -1) {
				// insert an empty line?
				QString lineCopy = line;
				lineCopy.remove(QRegExp("[:\\*F]"));
				int nextBeat = QVariant(lineCopy.section(" ", 0, 0, QString::SectionSkipEmpty)).toInt();

				if(nextBeat - lastBeat > 20)
					result << QString();
				lastBeat = -1; // reset beat counter
			}

			line.remove("\n");

			if(QRegExp("\\s*\\*.*").exactMatch(line)) // golden note
				result.last() += line.remove(linePrefix).append("</b>").prepend("<b>");
			else if(QRegExp("\\s*F.*").exactMatch(line)) // freestyle note
				result.last() += line.remove(linePrefix).append("</i>").prepend("<i>");
			else
				result.last() += line.remove(linePrefix);
		}
	}

	return result;
}

/*!
 * Creates a complete new song file for US. Any old data will be overwritten.
 * \param force Indicates whether to save the file although automatic file save was
 * disabled.
 * \returns True on success, otherwise false.
 */
bool QUSongFile::save(bool force) {
	if(!force && !monty->autoSaveEnabled()) {
		_hasUnsavedChanges = true;
		return true;
	}

	// TODO: replace with "QIODevice::Truncate"
	QFile::remove(_fi.filePath());

	QFile _file;
	_file.setFileName(_fi.filePath());

	if(!_file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		emit finished(QString(tr("Save error! The song file \"%1\" was NOT saved.")).arg(this->songFileInfo().fileName()), QU::warning);
		return false;
	}

	QStringList tags;
	QSettings settings;
	tags = settings.value("tagOrder", QUSongFile::tags()).toStringList();

	QUSongFile::verifyTags(tags);

	// write supported tags
	foreach(QString tag, tags) {
		if(_info.value(tag.toUpper()) != "") { // do not write empty tags
			_file.write("#");
			_file.write(tag.toUpper().toLocal8Bit());
			_file.write(":");
			_file.write(_info.value(tag.toUpper()).toLocal8Bit());
			_file.write("\n");
		}
	}

	// write unsupported tags
	foreach(QString uTag, _foundUnsupportedTags) {
		_file.write("#");
		_file.write(uTag.toLocal8Bit());
		_file.write(":");
		_file.write(_info.value(uTag).toLocal8Bit());
		_file.write("\n");
	}

	// write lyrics
	foreach(QString line, _lyrics) {
		_file.write(line.toLocal8Bit());
	}

	// write song ending
	_file.write("E\n");

	// write footer
	foreach(QString line, _footer) {
		_file.write(line.toLocal8Bit());
	}

	_file.close();

	emit finished(QString(tr("The song file \"%1\" was saved successfully.")).arg(this->songFileInfo().fileName()), QU::saving);
	_hasUnsavedChanges = false;
	return true;
}

/*!
 * Rename a file or a directory. Try to enable case-sensitive renaming under
 * Windows by inserting an additional step with a preceding "_".
 */
bool QUSongFile::rename(QDir dir, const QString &oldName, const QString &newName) {
	bool result = true;

	if(QString::compare(oldName, newName.trimmed(), Qt::CaseSensitive) == 0) {
		emit finished(QString(tr("Old name and new name match: \"%1\"")).arg(oldName), QU::warning);
		return false;
	}

	if(oldName.length() == newName.trimmed().length()) {
		dir.rename(oldName, "_" + oldName);
		result = dir.rename("_" + oldName, newName.trimmed());
	} else {
		result = dir.rename(oldName, newName.trimmed());
	}

	return result;
}

/*!
 * Try to rename the directory of the current song file.
 * \param newName the new name (avoid illegal characters for your OS)
 */
void QUSongFile::renameSongDir(const QString &newName) {
	QDir dir(_fi.dir());
	dir.cdUp();

	QString oldName(_fi.dir().dirName());

	if(!rename(dir, oldName, newName)) {
		emit finished(QString(tr("Could NOT rename the song directory \"%1\" to \"%2\".")).arg(oldName).arg(newName), QU::warning);
		return;
	}

	dir.cd(newName);
	_fi.setFile(dir.path() + "/" + _fi.fileName());

	emit finished(QString(tr("Song directory renamed from: \"%1\" to: \"%2\".")).arg(oldName).arg(newName), QU::information);
}

/*!
 * Try to rename the song file itself.
 * \param newName the new name (avoid illegal characters for your OS)
 */
void QUSongFile::renameSongTxt(const QString &newName) {
	QString oldName(_fi.fileName());

	if(!rename(_fi.dir(), oldName, newName)) {
		emit finished(QString(tr("Could NOT rename the song file \"%1\" to \"%2\".")).arg(oldName).arg(newName), QU::warning);
		return;
	}

	_fi.setFile(_fi.dir().path() + "/" + newName);

	emit finished(QString(tr("Song file renamed from: \"%1\" to: \"%2\".")).arg(oldName).arg(newName), QU::information);
}

/*!
 * Try to rename the mp3 file of the current song file. The information about the
 * current mp3 file has to be correct or the file cannot be found and renamed.
 * \param newName the new name (avoid illegal characters for your OS)
 */
void QUSongFile::renameSongMp3(const QString &newName) {
	QString oldName(mp3());

	if(!rename(_fi.dir(), oldName, newName)) {
		emit finished(QString(tr("Could NOT rename the audio file \"%1\" to \"%2\".")).arg(oldName).arg(newName), QU::warning);
		return;
	}

	setInfo(MP3_TAG, newName);

	emit finished(QString(tr("Audio file renamed from: \"%1\" to: \"%2\".")).arg(oldName).arg(newName), QU::information);
}

/*!
 * Try to rename the cover picture file of the current song file. The information about the
 * current cover has to be correct or the file cannot be found and renamed.
 * \param newName the new name (avoid illegal characters for your OS)
 */
void QUSongFile::renameSongCover(const QString &newName) {
	QString oldName(cover());

	if(!rename(_fi.dir(), oldName, newName)) {
		emit finished(QString(tr("Could NOT rename the cover picture \"%1\" to \"%2\".")).arg(oldName).arg(newName), QU::warning);
		return;
	}

	setInfo(COVER_TAG, newName);

	emit finished(QString(tr("Cover picture renamed from: \"%1\" to: \"%2\".")).arg(oldName).arg(newName), QU::information);
}

/*!
 * Try to rename the background picture file of the current song file. The information about the
 * current background has to be correct or the file cannot be found and renamed.
 * \param newName the new name (avoid illegal characters for your OS)
 */
void QUSongFile::renameSongBackground(const QString &newName) {
	QString oldName(background());

	if(!rename(_fi.dir(), oldName, newName)) {
		emit finished(QString(tr("Could NOT rename the background picture \"%1\" to \"%2\".")).arg(oldName).arg(newName), QU::warning);
		return;
	}

	setInfo(BACKGROUND_TAG, newName);

	emit finished(QString(tr("Background picture renamed from: \"%1\" to: \"%2\".")).arg(oldName).arg(newName), QU::information);
}

/*!
 * Try to rename the video file of the current song file. The information about the
 * current video has to be correct or the file cannot be found and renamed.
 * \param newName the new name (avoid illegal characters for your OS)
 */
void QUSongFile::renameSongVideo(const QString &newName) {
	QString oldName(video());

	if(!rename(_fi.dir(), oldName, newName)) {
		emit finished(QString(tr("Could NOT rename the video file \"%1\" to \"%2\".")).arg(oldName).arg(newName), QU::warning);
		return;
	}

	setInfo(VIDEO_TAG, newName);

	emit finished(QString(tr("Video file renamed from: \"%1\" to: \"%2\".")).arg(oldName).arg(newName), QU::information);
}

/*!
 * Reads the ID3 tag from the specified MP3-File (info has to be correct) and uses
 * artist and title information for the song file.
 */
void QUSongFile::useID3TagForArtist() {
	if(!hasMp3()) {
		emit finished(QString(tr("The song \"%1 - %2\" has no audio file assigned. Cannot use ID3 tag for artist.")).arg(this->artist()).arg(this->title()), QU::warning);
		return;
	}

	TagLib::FileRef ref(mp3FileInfo().absoluteFilePath().toLocal8Bit().data());

	QString oldArtist(this->artist());
	QString newArtist(TStringToQString(ref.tag()->artist()));

	if(newArtist.isEmpty()) {
		emit finished(QString(tr("The audio file %1 does not contain ID3 tag information about an artist.")).arg(this->mp3()), QU::warning);
		return;
	}

	setInfo(ARTIST_TAG, newArtist);
	emit finished(QString(tr("ID3 tag of \"%1\" used for artist. Changed from: \"%2\" to: \"%3\".")).arg(this->mp3()).arg(oldArtist).arg(this->artist()), QU::information);
}

void QUSongFile::useID3TagForTitle() {
	if(!hasMp3()) {
		emit finished(QString(tr("The song \"%1 - %2\" has no audio file assigned. Cannot use ID3 tag for title.")).arg(this->artist()).arg(this->title()), QU::warning);
		return;
	}

	TagLib::FileRef ref(mp3FileInfo().absoluteFilePath().toLocal8Bit().data());

	QString oldTitle(this->title());
	QString newTitle(TStringToQString(ref.tag()->title()));

	if(newTitle.isEmpty()) {
		emit finished(QString(tr("The audio file %1 does not contain ID3 tag information about a title.")).arg(this->mp3()), QU::warning);
		return;
	}

	setInfo(TITLE_TAG, newTitle);
	emit finished(QString(tr("ID3 tag of \"%1\" used for title. Changed from: \"%2\" to: \"%3\".")).arg(this->mp3()).arg(oldTitle).arg(this->title()), QU::information);
}

void QUSongFile::useID3TagForGenre() {
	if(!hasMp3()) {
		emit finished(QString(tr("The song \"%1 - %2\" has no audio file assigned. Cannot use ID3 tag for genre.")).arg(this->artist()).arg(this->title()), QU::warning);
		return;
	}

	TagLib::FileRef ref(mp3FileInfo().absoluteFilePath().toLocal8Bit().data());

	QString oldGenre(this->genre());
	QString newGenre(TStringToQString(ref.tag()->genre()));

	if(newGenre.isEmpty()) {
		emit finished(QString(tr("The audio file %1 does not contain ID3 tag information about a genre.")).arg(this->mp3()), QU::warning);
		return;
	}

	setInfo(GENRE_TAG, newGenre);
	emit finished(QString(tr("ID3 tag of \"%1\" used for genre. Changed from: \"%2\" to: \"%3\".")).arg(this->mp3()).arg(oldGenre).arg(this->genre()), QU::information);
}

void QUSongFile::useID3TagForYear() {
	if(!hasMp3()) {
		emit finished(QString(tr("The song \"%1 - %2\" has no audio file assigned. Cannot use ID3 tag for year.")).arg(this->artist()).arg(this->title()), QU::warning);
		return;
	}

	TagLib::FileRef ref(mp3FileInfo().absoluteFilePath().toLocal8Bit().data());

	QString oldYear(this->year());
	QString newYear(QVariant(ref.tag()->year()).toString());

	if(newYear == "0") {
		emit finished(QString(tr("The audio file %1 does not contain ID3 tag information about a year.")).arg(this->mp3()), QU::warning);
		return;
	}

	setInfo(YEAR_TAG, newYear);
	emit finished(QString(tr("ID3 tag of \"%1\" used for year. Changed from: \"%2\" to: \"%3\".")).arg(this->mp3()).arg(oldYear).arg(this->year()), QU::information);
}

void QUSongFile::removeUnsupportedTags() {
	if(!this->unsupportedTagsFound()) {
		emit finished(QString(tr("The song \"%1 - %2\" has no unsupported tags.")).arg(this->artist()).arg(this->title()), QU::information);
		return;
	}

	foreach(QString uTag, _foundUnsupportedTags) {
		_info.remove(uTag);
		emit finished(QString(tr("Unsupported tag removed: #%1.")).arg(uTag), QU::information);
	}

	_foundUnsupportedTags.clear();
}

/*!
 * Uses all files in the song directory to guess missing files according to some
 * common patterns: "cover", "[CO]" -> Cover; "back", "[BG]", -> Background, a.s.o.
 */
void QUSongFile::autoSetFiles() {
	QFileInfoList files = this->songFileInfo().dir().entryInfoList(
			QU::allowedAudioFiles() + QU::allowedPictureFiles() + QU::allowedVideoFiles(),
			QDir::Files);

	foreach(QFileInfo fi, files) {
		autoSetFile(fi);
	}
}

/*!
 * Uses the filename of the given QFileInfo object to assign it to a proper tag.
 * \param file That file will be used for auto-set.
 * \param force Use the file although the present tag is correct/valid.
 */
void QUSongFile::autoSetFile(const QFileInfo &fi, bool force) {
	QString fileScheme("*." + fi.suffix());

	if(QU::allowedAudioFiles().contains(fileScheme, Qt::CaseInsensitive)) {
		if(!this->hasMp3() || force) {
			this->setInfo(MP3_TAG, fi.fileName());
			emit finished(QString("Assigned \"%1\" as audio file for \"%2 - %3\".").arg(mp3()).arg(artist()).arg(title()), QU::information);
		}
	} else if(QU::allowedVideoFiles().contains(fileScheme, Qt::CaseInsensitive)) {
		if(!this->hasVideo() || force) {
			this->setInfo(VIDEO_TAG, fi.fileName());
			emit finished(QString(tr("Assigned \"%1\" as video file for \"%2 - %3\".")).arg(video()).arg(artist()).arg(title()), QU::information);
		}
	} else if(QU::allowedPictureFiles().contains(fileScheme, Qt::CaseInsensitive)) {
		QRegExp reCover("\\[CO\\]|cove?r?", Qt::CaseInsensitive);
		QRegExp reBackground("\\[BG\\]|back", Qt::CaseInsensitive);

		if(fi.fileName().contains(reCover) && (!this->hasCover() || force) ) {
			this->setInfo(COVER_TAG, fi.fileName());
			emit finished(QString(tr("Assigned \"%1\" as cover picture for \"%2 - %3\".")).arg(cover()).arg(artist()).arg(title()), QU::information);
		} else if(fi.fileName().contains(reBackground) && (!this->hasBackground() || force) ) {
			this->setInfo(BACKGROUND_TAG, fi.fileName());
			emit finished(QString(tr("Assigned \"%1\" as background picture for \"%2 - %3\".")).arg(background()).arg(artist()).arg(title()), QU::information);
		}
	}
}

/*!
 * Copy a file to the song folder and force its usage for a proper
 * tag. (e.g. blubb.mp3 will be used as #MP3).
 * \sa autoSetFiles()
 */
void QUSongFile::useExternalFile(const QString &filePath) {
	QFileInfo source(filePath);
	QFileInfo destination(this->songFileInfo().dir(), source.fileName());

	if(!QFile::copy(source.filePath(), destination.filePath())) {
		emit finished(QString(tr("Could not copy the file \"%1\" to \"%2\".")).arg(source.fileName()).arg(destination.path()), QU::warning);
		return;
	}

	emit finished(QString(tr("The file \"%1\" was successfully copied to \"%2\".")).arg(source.fileName()).arg(destination.path()), QU::saving);
	this->autoSetFile(destination, true);
}

/*!
 * Deletes any file in the song directory which is not used by the song itself.
 */
void QUSongFile::deleteUnusedFiles() {
	QFileInfoList fiList = _fi.dir().entryInfoList(QDir::Files, QDir::Name);

	foreach(QFileInfo fi, fiList) {
		bool isUsed = false;
		if(QString::compare(fi.fileName(), this->mp3(), Qt::CaseInsensitive) == 0)
			isUsed = true;
		if(QString::compare(fi.fileName(), this->cover(), Qt::CaseInsensitive) == 0)
			isUsed = true;
		if(QString::compare(fi.fileName(), this->background(), Qt::CaseInsensitive) == 0)
			isUsed = true;
		if(QString::compare(fi.fileName(), this->video(), Qt::CaseInsensitive) == 0)
			isUsed = true;
		if(QString::compare(fi.fileName(), this->songFileInfo().fileName(), Qt::CaseInsensitive) == 0)
			isUsed = true;

		if(!isUsed && QFile::remove(fi.filePath()))
			emit finished(QString(tr("File removed successfully: \"%1\".")).arg(fi.filePath()), QU::information);
	}
}

void QUSongFile::clearInvalidFileTags() {
	if( (this->mp3() != N_A || this->start() != N_A || this->end() != N_A) && !this->hasMp3()) {
		this->setInfo(MP3_TAG, "");
		this->setInfo(START_TAG, "");
		this->setInfo(END_TAG, "");
		emit finished(QString(tr("Audio file tag removed for \"%1 - %2\".")).arg(this->artist()).arg(this->title()), QU::information);
	}
	if(this->cover() != N_A && !this->hasCover()) {
		this->setInfo(COVER_TAG, "");
		emit finished(QString(tr("Cover tag removed for \"%1 - %2\".")).arg(this->artist()).arg(this->title()), QU::information);
	}
	if(this->background() != N_A && !this->hasBackground()) {
		this->setInfo(BACKGROUND_TAG, "");
		emit finished(QString(tr("Background tag removed for \"%1 - %2\".")).arg(this->artist()).arg(this->title()), QU::information);
	}
	if( (this->video() != N_A || this->videogap() != N_A) && !this->hasVideo()) {
		this->setInfo(VIDEO_TAG, "");
		this->setInfo(VIDEOGAP_TAG, "");
		emit finished(QString(tr("Video tag removed for \"%1 - %2\".")).arg(this->artist()).arg(this->title()), QU::information);
	}
}

/*!
 * Move all files in a song directory to a new location. This includes also files which
 * are not currently used by the song itself.
 * \param newRelativePath new relative path for the song; is relative to the UltraStar song folder
 */
void QUSongFile::moveAllFiles(const QString &newRelativePath) {
	QString source = QUMainWindow::BaseDir.relativeFilePath(_fi.path());
	QString destination = newRelativePath;

	if(QString::compare(source, destination, Qt::CaseInsensitive) == 0) {
		emit finished(QString(tr("Old path and new path match! Cannot change song path to: \"%1\"")).arg(destination), QU::warning);
		return;
	}

	if(!QUMainWindow::BaseDir.mkpath(newRelativePath)) {
		emit finished(QString(tr("Could not create new song path: \"%1\"")).arg(newRelativePath), QU::warning);
		return;
	}

	// move files to new location
	bool allFilesCopied = true;
	foreach(QFileInfo fi, _fi.dir().entryInfoList(QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot)) {
		QString from = fi.filePath();
		QString to = QFileInfo(QUMainWindow::BaseDir, destination + "/" + fi.fileName()).filePath();

		if(!QFile::rename(from, to)) {
			emit finished(QString(tr("Failed to move \"%1\" to \"%2\".")).arg(from).arg(to), QU::warning);
			allFilesCopied = false;
		} else
			emit finished(QString(tr("The file \"%1\" was successfully moved to \"%2\".")).arg(from).arg(to), QU::saving);
	}

	if(!allFilesCopied) {
		emit finished(QString(tr("Could NOT move all files of the song \"%2\" to a new location. Check out \"%1\" for the files which were copied.")).arg(newRelativePath).arg(QString("%1 - %2").arg(artist()).arg(title())), QU::warning);
		return;
	}

	// remove empty folders
	QDir oldDir = _fi.dir();
	while(!oldDir.isRoot() and oldDir.entryList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot).isEmpty() and oldDir != QUMainWindow::BaseDir) {
		QString dirName = oldDir.dirName();
		oldDir.cdUp();
		if(!oldDir.rmdir(dirName))
			emit finished(QString(tr("Could not remove old, empty folder \"%1\". Please do it manually.")).arg(dirName), QU::warning);
	}

	// change internal song location
	_fi = QFileInfo(QUMainWindow::BaseDir, destination + "/" + _fi.fileName()).filePath();
	emit finished(QString(tr("Location of song \"%1 - %2\" successfully changed to \"%3\" in your UltraStar song folder.")).arg(artist()).arg(title()).arg(newRelativePath), QU::information);
}

/*!
 * Uses the #END tag to close the gap between length() and lengthMp3(). But only if an
 * audio file is present and longer than the song.
 *
 * A little gap is appended.
 */
void QUSongFile::fixAudioLength() {
	if(!hasMp3()) {
		emit finished(QString(tr("Could not fix audio length because no audio file is present: \"%1 - %2\"")).arg(artist()).arg(title()), QU::warning);
		return;
	}

	// remove present #END value
	setInfo(END_TAG, "");

	int length = this->length();
	int lengthMp3 = this->lengthMp3(); // length of audio file because #END was set to 0

	if(length > lengthMp3) {
		emit finished(QString(tr("Could not fix audio length because audio file is shorter than song: \"%1 - %2\"")).arg(artist()).arg(title()), QU::warning);
		return;
	}

	QSettings settings;
	int timeDiffLower = settings.value("timeDiffLower", LOWER_TIME_BOUND_DEFAULT).toInt();
	int end = length + timeDiffLower; // calculate the new #END value

	if(end > lengthMp3) {
		emit finished(QString(tr("Could not fix audio length because new value for #END would be greater than length of audio file: \"%1 - %2\"")).arg(artist()).arg(title()), QU::warning);
		return;
	}

	// set new value for #END, in milliseconds
	setInfo(END_TAG, QVariant(end * 1000).toString());

	emit finished(QString(tr("Audio length was fixed for song \"%1 - %2\". #END changed to: %3")).arg(artist()).arg(title()).arg(end * 1000), QU::information);
}

/*!
 * Clear the #END tag.
 */
void QUSongFile::removeEndTag() {
	if(end() != N_A) {
		setInfo(END_TAG, "");
		emit finished(QString(tr("The tag #END was removed for: \"%1 - %2\"")).arg(artist()).arg(title()), QU::information);
	}
}
