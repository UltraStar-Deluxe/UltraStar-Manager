#include "QUSongFile.h"

#include <QByteArray>
#include <QVariant>
#include <QDir>
#include <QSettings>
#include <QMessageBox>

#include "fileref.h"
#include "tag.h"
#include "tstring.h"

/*!
 * Creates a new song file object.
 * \param file an existing US song file (normally a *.txt)
 * \param parent parent for Qt object tree
 */
QUSongFile::QUSongFile(const QString &file, QObject *parent): QObject(parent) {
	_fi.setFile(file);
	updateCache();
}

QUSongFile::~QUSongFile() {
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
	
	_lyrics.clear();
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
 * \returns True on success, otherwise false.
 */
bool QUSongFile::save() {
	QFile::remove(_fi.filePath());
	
	_file.setFileName(_fi.filePath());
	
	if(!_file.open(QIODevice::WriteOnly | QIODevice::Text))
		return false;
	
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
	return true;
}

/*!
 * Rename a file or a directory. Try to enable case-sensitive renaming under
 * Windows.
 */
bool QUSongFile::rename(QDir &dir, const QString &oldName, const QString &newName) {
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
 * \returns True on success, otherwise false.
 */
bool QUSongFile::renameSongDir(const QString &newName) {
	QDir dir(_fi.dir());	
	dir.cdUp();
	
	bool result = rename(dir, _fi.dir().dirName(), newName);
	
	if(result) {
		dir.cd(newName);
		_fi.setFile(dir.path() + "/" + _fi.fileName());
	}
	
	return result;
}

/*!
 * Try to rename the song file itself.
 * \param newName the new name (avoid illegal characters for your OS)
 * \returns True on success, otherwise false.
 */
bool QUSongFile::renameSongTxt(const QString &newName) {
	bool result = _fi.dir().rename(_fi.fileName(), newName);
	
	if(result) {
		_fi.setFile(_fi.dir().path() + "/" + newName);
	}
	
	return result;	
}

/*!
 * Try to rename the mp3 file of the current song file. The information about the
 * current mp3 file has to be correct or the file cannot be found and renamed.
 * \param newName the new name (avoid illegal characters for your OS)
 * \returns True on success, otherwise false.
 */
bool QUSongFile::renameSongMp3(const QString &newName) {
	bool result = _fi.dir().rename(mp3(), newName);
	
	if(result) {
		setInfo(MP3_TAG, newName);
	}
	
	return result;
}

/*!
 * Try to rename the cover picture file of the current song file. The information about the
 * current cover has to be correct or the file cannot be found and renamed.
 * \param newName the new name (avoid illegal characters for your OS)
 * \returns True on success, otherwise false.
 */
bool QUSongFile::renameSongCover(const QString &newName) {
	bool result = _fi.dir().rename(cover(), newName);
	
	if(result) {
		setInfo(COVER_TAG, newName);
	}
	
	return result;
}

/*!
 * Try to rename the background picture file of the current song file. The information about the
 * current background has to be correct or the file cannot be found and renamed.
 * \param newName the new name (avoid illegal characters for your OS)
 * \returns True on success, otherwise false.
 */
bool QUSongFile::renameSongBackground(const QString &newName) {
	bool result = _fi.dir().rename(background(), newName);
	
	if(result) {
		setInfo(BACKGROUND_TAG, newName);
	}
	
	return result;
}

/*!
 * Try to rename the video file of the current song file. The information about the
 * current video has to be correct or the file cannot be found and renamed.
 * \param newName the new name (avoid illegal characters for your OS)
 * \returns True on success, otherwise false.
 */
bool QUSongFile::renameSongVideo(const QString &newName) {
	bool result = _fi.dir().rename(video(), newName);
	
	if(result) {
		setInfo(VIDEO_TAG, newName);
	}
	
	return result;
}

/*!
 * Reads the ID3 tag from the specified MP3-File (info has to be correct) and uses
 * artist and title information for the song file.
 */
bool QUSongFile::useID3TagForArtist() {
	if(!hasMp3())
		return false;
	
	TagLib::FileRef ref(mp3FileInfo().absoluteFilePath().toLocal8Bit().data());
	
	QString newArtist(TStringToQString(ref.tag()->artist()));
	
	if(newArtist.isEmpty())
		return false;
	
	setInfo(ARTIST_TAG, newArtist);
	
	return true;
}

bool QUSongFile::useID3TagForTitle() {
	if(!hasMp3())
		return false;
	
	TagLib::FileRef ref(mp3FileInfo().absoluteFilePath().toLocal8Bit().data());
	
	QString newTitle(TStringToQString(ref.tag()->title()));
	
	if(newTitle.isEmpty())
		return false;
	
	setInfo(TITLE_TAG, newTitle);
	
	return true;
}

bool QUSongFile::useID3TagForGenre() {
	if(!hasMp3())
		return false;
	
	TagLib::FileRef ref(mp3FileInfo().absoluteFilePath().toLocal8Bit().data());
	
	QString newGenre(TStringToQString(ref.tag()->genre()));
	
	if(newGenre.isEmpty())
		return false;
	
	setInfo(GENRE_TAG, newGenre);
	
	return true;
}

bool QUSongFile::useID3TagForYear() {
	if(!hasMp3())
		return false;
	
	TagLib::FileRef ref(mp3FileInfo().absoluteFilePath().toLocal8Bit().data());
	
	QString newYear(QVariant(ref.tag()->year()).toString());
	
	if(newYear == "0")
		return false;
	
	setInfo(YEAR_TAG, newYear);
	
	return true;
}

bool QUSongFile::removeUnsupportedTags() {
	foreach(QString uTag, _foundUnsupportedTags) {
		_info.remove(uTag);
	}
	
	_foundUnsupportedTags.clear();
	
	return true;
}


QStringList QUSongFile::allowedAudioFiles() {
	return QString("*.mp3 *.ogg").split(" ");
}

QStringList QUSongFile::allowedPictureFiles() {
	return QString("*.jpg *.png *.bmp").split(" ");
}

QStringList QUSongFile::allowedVideoFiles() {
	return QString("*.mpg *.mpeg *.avi *.flv *.ogm").split(" ");
}
