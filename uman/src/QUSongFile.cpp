#include "QUSongFile.h"
#include "QUMonty.h"

#include <QByteArray>
#include <QVariant>
#include <QDir>
#include <QSettings>
#include <QMessageBox>
#include <QFileInfoList>

#include "fileref.h"
#include "tag.h"
#include "tstring.h"

/*!
 * Creates a new song file object.
 * \param file an existing US song file (normally a *.txt)
 * \param parent parent for Qt object tree
 */
QUSongFile::QUSongFile(const QString &file, QObject *parent): QObject(parent), _hasUnsavedChanges(false) {
	this->setFile(file);
}

QUSongFile::~QUSongFile() {
}

/*!
 * Here you can set up a new file that will be used for this song.
 */
void QUSongFile::setFile(const QString &file) {
	_fi.setFile(file);
	updateCache();
}

/*!
 * Reads the US data file and loads all data into memory. This is needed to be done
 * before any changes can be made.
 * \returns True on success, otherwise false.
 */
bool QUSongFile::updateCache() {
	_file.setFileName(_fi.filePath());
	
	if(!_file.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;
	
	// clear contents
	_lyrics.clear();
	_info.clear();
	_foundUnsupportedTags.clear();
	
	// read new content
	QString line;
	while( !(QRegExp("[:\\*FE].*").exactMatch(line) || _file.atEnd()) ) {
		line = QString(_file.readLine());
		
		// read supported tags
		bool isSupported = false;
		foreach(QString tag, QUSongFile::tags()) {
			if(line.startsWith("#" + tag + ":")) {
				setInfo(tag, line.section("#" + tag + ":", 0, 0, QString::SectionSkipEmpty));
				isSupported = true;
			}
		}
		
		// read unsupported tags
		if(!isSupported and line.startsWith("#")) {
			QString uTag(line.section(":", 0, 0).remove("#").trimmed());
			QString uValue(line.section("#" + uTag + ":", 0, 0, QString::SectionSkipEmpty));
			
			if(!uTag.isEmpty() and !uValue.isEmpty()) {
				setInfo(uTag, uValue);
				_foundUnsupportedTags << uTag;
			}
		}
	}
	
	_lyrics << line;
	
	while(!_file.atEnd()) {
		line = QString(_file.readLine());
		_lyrics << line;
	}
	
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
		_info.take(tag);
		return;
	}
	
	_info[tag] = value.trimmed();
}

/*!
 * \returns A list of strings with all available tags.
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
	result << COMMENT_TAG;
	
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
		
		QMessageBox::warning(0, "Deprecated tag information detected", 
				"The number of available tags in your configuration and that one this application offers are different.\n\n"
				"The tag order was reset to its default order. Check out the options to set up your custom order again.");
		
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
	
	QFile::remove(_fi.filePath());
	
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
		if(_info.value(tag) != "") { // do not write empty tags
			_file.write("#");
			_file.write(tag.toLocal8Bit());
			_file.write(":");
			_file.write(_info.value(tag).toLocal8Bit());
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
	
	foreach(QString line, _lyrics) {
		_file.write(line.toLocal8Bit());
	}
	
	_file.close();
	
	emit finished(QString("The song file \"%1\" was saved successfully.").arg(this->songFileInfo().fileName()), QU::saving);
	_hasUnsavedChanges = false;
	return true;
}

/*!
 * Rename a file or a directory. Try to enable case-sensitive renaming under
 * Windows.
 */
bool QUSongFile::rename(QDir dir, const QString &oldName, const QString &newName) {
	bool result = true;
	
	if(QString::compare(oldName, newName, Qt::CaseSensitive) == 0)
		return false;
	
	if(oldName.length() == newName.length()) {
		dir.rename(oldName, oldName + "_");
		result = dir.rename(oldName + "_", newName);
	} else {
		result = dir.rename(oldName, newName);
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
		emit finished(QString("Could NOT rename the song directory \"%1\" to \"%2\".").arg(oldName).arg(newName), QU::warning);
		return;
	}
		
	dir.cd(newName);
	_fi.setFile(dir.path() + "/" + _fi.fileName());
	
	emit finished(QString("Song directory renamed from: \"%1\" to: \"%2\".").arg(oldName).arg(newName), QU::information);
}

/*!
 * Try to rename the song file itself.
 * \param newName the new name (avoid illegal characters for your OS)
 */
void QUSongFile::renameSongTxt(const QString &newName) {
	QString oldName(_fi.fileName());
	
	if(!rename(_fi.dir(), oldName, newName)) {
		emit finished(QString("Could NOT rename the song file \"%1\" to \"%2\".").arg(oldName).arg(newName), QU::warning);
		return;
	}
	
	_fi.setFile(_fi.dir().path() + "/" + newName);
	
	emit finished(QString("Song file renamed from: \"%1\" to: \"%2\".").arg(oldName).arg(newName), QU::information);
}

/*!
 * Try to rename the mp3 file of the current song file. The information about the
 * current mp3 file has to be correct or the file cannot be found and renamed.
 * \param newName the new name (avoid illegal characters for your OS)
 */
void QUSongFile::renameSongMp3(const QString &newName) {
	QString oldName(mp3());
	
	if(!rename(_fi.dir(), oldName, newName)) {
		emit finished(QString("Could NOT rename the audio file \"%1\" to \"%2\".").arg(oldName).arg(newName), QU::warning);
		return;
	}
	
	setInfo(MP3_TAG, newName);
	
	emit finished(QString("Audio file renamed from: \"%1\" to: \"%2\".").arg(oldName).arg(newName), QU::information);
}

/*!
 * Try to rename the cover picture file of the current song file. The information about the
 * current cover has to be correct or the file cannot be found and renamed.
 * \param newName the new name (avoid illegal characters for your OS)
 */
void QUSongFile::renameSongCover(const QString &newName) {
	QString oldName(cover());
	
	if(!rename(_fi.dir(), oldName, newName)) {
		emit finished(QString("Could NOT rename the cover picture \"%1\" to \"%2\".").arg(oldName).arg(newName), QU::warning);
		return;
	}
	
	setInfo(COVER_TAG, newName);
	
	emit finished(QString("Cover picture renamed from: \"%1\" to: \"%2\".").arg(oldName).arg(newName), QU::information);
}

/*!
 * Try to rename the background picture file of the current song file. The information about the
 * current background has to be correct or the file cannot be found and renamed.
 * \param newName the new name (avoid illegal characters for your OS)
 */
void QUSongFile::renameSongBackground(const QString &newName) {
	QString oldName(background());
	
	if(!rename(_fi.dir(), oldName, newName)) {
		emit finished(QString("Could NOT rename the background picture \"%1\" to \"%2\".").arg(oldName).arg(newName), QU::warning);
		return;
	}
	
	setInfo(BACKGROUND_TAG, newName);
	
	emit finished(QString("Background picture renamed from: \"%1\" to: \"%2\".").arg(oldName).arg(newName), QU::information);
}

/*!
 * Try to rename the video file of the current song file. The information about the
 * current video has to be correct or the file cannot be found and renamed.
 * \param newName the new name (avoid illegal characters for your OS)
 */
void QUSongFile::renameSongVideo(const QString &newName) {
	QString oldName(video());
	
	if(!rename(_fi.dir(), oldName, newName)) {
		emit finished(QString("Could NOT rename the video file \"%1\" to \"%2\".").arg(oldName).arg(newName), QU::warning);
		return;
	}
	
	setInfo(VIDEO_TAG, newName);
	
	emit finished(QString("Video file renamed from: \"%1\" to: \"%2\".").arg(oldName).arg(newName), QU::information);
}

/*!
 * Reads the ID3 tag from the specified MP3-File (info has to be correct) and uses
 * artist and title information for the song file.
 */
void QUSongFile::useID3TagForArtist() {
	if(!hasMp3()) {
		emit finished(QString("The song \"%1 - %2\" has no audio file assigned. Cannot use ID3 tag for artist.").arg(this->artist()).arg(this->title()), QU::warning);
		return;
	}
	
	TagLib::FileRef ref(mp3FileInfo().absoluteFilePath().toLocal8Bit().data());
	
	QString oldArtist(this->artist());
	QString newArtist(TStringToQString(ref.tag()->artist()));
	
	if(newArtist.isEmpty()) {
		emit finished(QString("The audio file %1 does not contain ID3 tag information about an artist.").arg(this->mp3()), QU::warning);
		return;
	}
	
	setInfo(ARTIST_TAG, newArtist);
	emit finished(QString("ID3 tag of \"%1\" used for artist. Changed from: \"%2\" to: \"%3\".").arg(this->mp3()).arg(oldArtist).arg(this->artist()), QU::information);
}

void QUSongFile::useID3TagForTitle() {
	if(!hasMp3()) {
		emit finished(QString("The song \"%1 - %2\" has no audio file assigned. Cannot use ID3 tag for title.").arg(this->artist()).arg(this->title()), QU::warning);
		return;
	}
	
	TagLib::FileRef ref(mp3FileInfo().absoluteFilePath().toLocal8Bit().data());
	
	QString oldTitle(this->title());
	QString newTitle(TStringToQString(ref.tag()->title()));
	
	if(newTitle.isEmpty()) {
		emit finished(QString("The audio file %1 does not contain ID3 tag information about a title.").arg(this->mp3()), QU::warning);
		return;
	}
	
	setInfo(TITLE_TAG, newTitle);
	emit finished(QString("ID3 tag of \"%1\" used for title. Changed from: \"%2\" to: \"%3\".").arg(this->mp3()).arg(oldTitle).arg(this->title()), QU::information);
}

void QUSongFile::useID3TagForGenre() {
	if(!hasMp3()) {
		emit finished(QString("The song \"%1 - %2\" has no audio file assigned. Cannot use ID3 tag for genre.").arg(this->artist()).arg(this->title()), QU::warning);
		return;
	}
	
	TagLib::FileRef ref(mp3FileInfo().absoluteFilePath().toLocal8Bit().data());
	
	QString oldGenre(this->genre());
	QString newGenre(TStringToQString(ref.tag()->genre()));
	
	if(newGenre.isEmpty()) {
		emit finished(QString("The audio file %1 does not contain ID3 tag information about a genre.").arg(this->mp3()), QU::warning);
		return;
	}
	
	setInfo(GENRE_TAG, newGenre);
	emit finished(QString("ID3 tag of \"%1\" used for genre. Changed from: \"%2\" to: \"%3\".").arg(this->mp3()).arg(oldGenre).arg(this->genre()), QU::information);
}

void QUSongFile::useID3TagForYear() {
	if(!hasMp3()) {
		emit finished(QString("The song \"%1 - %2\" has no audio file assigned. Cannot use ID3 tag for year.").arg(this->artist()).arg(this->title()), QU::warning);
		return;
	}
	
	TagLib::FileRef ref(mp3FileInfo().absoluteFilePath().toLocal8Bit().data());
	
	QString oldYear(this->year());
	QString newYear(QVariant(ref.tag()->year()).toString());
	
	if(newYear == "0") {
		emit finished(QString("The audio file %1 does not contain ID3 tag information about a year.").arg(this->mp3()), QU::warning);
		return;
	}
	
	setInfo(YEAR_TAG, newYear);
	emit finished(QString("ID3 tag of \"%1\" used for year. Changed from: \"%2\" to: \"%3\".").arg(this->mp3()).arg(oldYear).arg(this->year()), QU::information);
}

void QUSongFile::removeUnsupportedTags() {
	if(!this->unsupportedTagsFound()) {
		emit finished(QString("The song \"%1 - %2\" has no unsupported tags.").arg(this->artist()).arg(this->title()), QU::information);
		return;
	}
	
	foreach(QString uTag, _foundUnsupportedTags) {
		_info.remove(uTag);
		emit finished(QString("Unsupported tag removed: #%1.").arg(uTag), QU::information);
	}
	
	_foundUnsupportedTags.clear();
}

/*!
 * Uses all files in the song directory to guess missing files according to some
 * common patterns: "cover", "[CO]" -> Cover; "back", "[BG]", -> Background, a.s.o.
 */
void QUSongFile::autoSetFiles() {
	QFileInfoList files = this->songFileInfo().dir().entryInfoList(
			QUSongFile::allowedAudioFiles() + QUSongFile::allowedPictureFiles() + QUSongFile::allowedVideoFiles(), 
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
	
	if(QUSongFile::allowedAudioFiles().contains(fileScheme, Qt::CaseInsensitive)) {
		if(!this->hasMp3() || force) {
			this->setInfo(MP3_TAG, fi.fileName());
			emit finished(QString("Assigned \"%1\" as audio file for \"%2 - %3\".").arg(mp3()).arg(artist()).arg(title()), QU::information);
		}
	} else if(QUSongFile::allowedVideoFiles().contains(fileScheme, Qt::CaseInsensitive)) {
		if(!this->hasVideo() || force) {
			this->setInfo(VIDEO_TAG, fi.fileName());
			emit finished(QString("Assigned \"%1\" as video file for \"%2 - %3\".").arg(video()).arg(artist()).arg(title()), QU::information);
		}
	} else if(QUSongFile::allowedPictureFiles().contains(fileScheme, Qt::CaseInsensitive)) {
		QRegExp reCover("\\[CO\\]|cove?r?", Qt::CaseInsensitive);
		QRegExp reBackground("\\[BG\\]|back", Qt::CaseInsensitive);
		
		if(fi.fileName().contains(reCover) && (!this->hasCover() || force) ) {
			this->setInfo(COVER_TAG, fi.fileName());
			emit finished(QString("Assigned \"%1\" as cover picture for \"%2 - %3\".").arg(cover()).arg(artist()).arg(title()), QU::information);
		} else if(fi.fileName().contains(reBackground) && (!this->hasBackground() || force) ) {
			this->setInfo(BACKGROUND_TAG, fi.fileName());
			emit finished(QString("Assigned \"%1\" as background picture for \"%2 - %3\".").arg(background()).arg(artist()).arg(title()), QU::information);
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
		emit finished(QString("Could not copy the file \"%1\" to \"%2\".").arg(source.fileName()).arg(destination.path()), QU::warning);
		return;
	}
	
	emit finished(QString("The file \"%1\" was successfully copied to \"%2\".").arg(source.fileName()).arg(destination.path()), QU::saving);
	this->autoSetFile(destination, true);
}

/*
 * STATIC MEMBERS
 */

QStringList QUSongFile::allowedAudioFiles() {
	return QString("*.mp3 *.ogg").split(" ");
}

QStringList QUSongFile::allowedPictureFiles() {
	return QString("*.jpg *.png *.bmp").split(" ");
}

QStringList QUSongFile::allowedVideoFiles() {
	return QString("*.mpg *.mpeg *.avi *.flv *.ogm").split(" ");
}
