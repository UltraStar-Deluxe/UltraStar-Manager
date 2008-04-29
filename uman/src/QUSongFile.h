#ifndef QUSONGFILE_H_
#define QUSONGFILE_H_

#include <QObject>
#include <QFileInfo>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QDir>

#include "QU.h"

#define SONG_FILE_SUFFIX "txt"
#define N_A "-"

#define TITLE_TAG      "TITLE"
#define ARTIST_TAG     "ARTIST"
#define LANGUAGE_TAG   "LANGUAGE"
#define EDITION_TAG    "EDITION"
#define GENRE_TAG      "GENRE"
#define YEAR_TAG       "YEAR"
#define CREATOR_TAG    "CREATOR"
#define MP3_TAG        "MP3"
#define COVER_TAG      "COVER"
#define BACKGROUND_TAG "BACKGROUND"
#define VIDEO_TAG      "VIDEO"
#define VIDEOGAP_TAG   "VIDEOGAP"
#define START_TAG      "START"
#define END_TAG        "END"
#define RELATIVE_TAG   "RELATIVE"
#define BPM_TAG        "BPM"
#define GAP_TAG        "GAP"
#define COMMENT_TAG    "COMMENT" /* not supported by UltraStar */

/*!
 * This class represents a data file which is used by UltraStar for every song.
 * It contains all tags that are available in US Deluxe 1.00.
 */
class QUSongFile: public QObject {
	Q_OBJECT
	
public:
	QUSongFile(const QString &file, QObject *parent = 0);
	~QUSongFile();
	
	bool hasUnsavedChanges() const { return _hasUnsavedChanges; }
	void setFile(const QString &file);
	
	// sorting functions
	static bool artistLessThan (QUSongFile *s1, QUSongFile *s2)   { return QString::compare(s1->artist(), s2->artist(), Qt::CaseInsensitive) < 0; }
	static bool titleLessThan (QUSongFile *s1, QUSongFile *s2)    { return QString::compare(s1->title(), s2->title(), Qt::CaseInsensitive) < 0; }
	static bool languageLessThan (QUSongFile *s1, QUSongFile *s2) { return QString::compare(s1->language(), s2->language(), Qt::CaseInsensitive) < 0; }
	static bool editionLessThan (QUSongFile *s1, QUSongFile *s2)  { return QString::compare(s1->edition(), s2->edition(), Qt::CaseInsensitive) < 0; }
	static bool genreLessThan (QUSongFile *s1, QUSongFile *s2)    { return QString::compare(s1->genre(), s2->genre(), Qt::CaseInsensitive) < 0; }
	static bool yearLessThan (QUSongFile *s1, QUSongFile *s2)     { return QString::compare(s1->year(), s2->year(), Qt::CaseInsensitive) < 0; }
	static bool creatorLessThan (QUSongFile *s1, QUSongFile *s2)  { return QString::compare(s1->creator(), s2->creator(), Qt::CaseInsensitive) < 0; }
	
	static bool hasMp3LessThan (QUSongFile *s1, QUSongFile *s2)         { return !s1->hasMp3() && s2->hasMp3(); }
	static bool hasCoverLessThan (QUSongFile *s1, QUSongFile *s2)       { return !s1->hasCover() && s2->hasCover(); }
	static bool hasBackgroundLessThan (QUSongFile *s1, QUSongFile *s2)  { return !s1->hasBackground() && s2->hasBackground(); }
	static bool hasVideoLessThan (QUSongFile *s1, QUSongFile *s2)       { return !s1->hasVideo() && s2->hasVideo(); }
	
public slots:
	QString artist() const     {return _info.value("ARTIST", QString(N_A));}
	QString title() const      {return _info.value("TITLE", QString(N_A));}
	QString mp3() const        {return _info.value("MP3", QString(N_A));}
	QString bpm() const        {return _info.value("BPM", QString(N_A));}
	QString gap() const        {return _info.value("GAP", QString(N_A));}
	QString video() const      {return _info.value("VIDEO", QString(N_A));}
	QString videogap() const   {return _info.value("VIDEOGAP", QString(N_A));}
	QString cover() const      {return _info.value("COVER", QString(N_A));}
	QString background() const {return _info.value("BACKGROUND", QString(N_A));}
	QString start() const      {return _info.value("START", QString(N_A));}
	QString language() const   {return _info.value("LANGUAGE", QString(N_A));}
	QString relative() const   {return _info.value("RELATIVE", QString(N_A));}
	QString edition() const    {return _info.value("EDITION", QString(N_A));}
	QString genre() const      {return _info.value("GENRE", QString(N_A));}
	QString year() const       {return _info.value("YEAR", QString(N_A));}
	QString end() const        {return _info.value("END", QString(N_A));}
	QString creator() const    {return _info.value("CREATOR", QString(N_A));}
	QString comment() const    {return _info.value(COMMENT_TAG, QString(N_A));}
	
	bool hasMp3() const;
	bool hasCover() const;
	bool hasBackground() const;
	bool hasVideo() const;
	
	QFileInfo songFileInfo() const {return _fi;} //!< \returns a file info for the current US song file
	
	QFileInfo mp3FileInfo() const {return QFileInfo(_fi.dir(), mp3());} //!< \returns a file info for the mp3 file
	QFileInfo coverFileInfo() const {return QFileInfo(_fi.dir(), cover());} //!< \returns a file info for the cover file
	QFileInfo backgroundFileInfo() const {return QFileInfo(_fi.dir(), background());} //!< \returns a file info for the background file
	QFileInfo videoFileInfo() const {return QFileInfo(_fi.dir(), video());} //!< \returns a file info for the video file
	
	void setInfo(const QString &key, const QString &value);
	
	bool save(bool force = false);
	void renameSongDir(const QString &newName);
	void renameSongTxt(const QString &newName);
	void renameSongMp3(const QString &newName);
	void renameSongCover(const QString &newName);
	void renameSongBackground(const QString &newName);
	void renameSongVideo(const QString &newName);
	
	void useID3TagForArtist();
	void useID3TagForTitle();
	void useID3TagForGenre();
	void useID3TagForYear();
	
	static QStringList tags();
	static QStringList noteTypes();
	
	static void verifyTags(QStringList &tags);
	static QStringList allowedAudioFiles();
	static QStringList allowedPictureFiles();
	static QStringList allowedVideoFiles();
	
	bool unsupportedTagsFound() const { return _foundUnsupportedTags.size() > 0; }
	QString unsupportedTags() const { return _foundUnsupportedTags.join("\n#"); }
	void removeUnsupportedTags();
	
	void useExternalFile(const QString &filePath);
	void autoSetFiles();
	void autoSetFile(const QFileInfo &fi, bool force = false);
	
signals:
	void finished(const QString &message, QU::EventMessageTypes type);
	
private:
	QFileInfo _fi;
	QFile     _file;
	
	QMap<QString, QString> _info; // song header
	QStringList _lyrics;          // lyrics
	QStringList _footer;          // other stuff after the end mark 'E' in the song file
	
	QStringList _foundUnsupportedTags;
	bool _hasUnsavedChanges;
	
	bool updateCache();
	bool rename(QDir dir, const QString &oldName, const QString &newName);
};

#endif /*QUSONGFILE_H_*/
