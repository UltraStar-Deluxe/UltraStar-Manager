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

#define TEXT_SOURCE         "*TEXT*"
#define KEEP_SUFFIX_SOURCE  "*SUFFIX*"
#define UNKNOWN_TAGS_SOURCE "*UNKNOWN_TAGS*"
#define CUSTOM_TAG_SUFFIX   " (custom)"

/*!
 * This class represents a data file which is used by UltraStar for every song.
 * It contains all tags that are available in US Deluxe 1.00.
 */
class QUSongFile: public QObject {
	Q_OBJECT
	// normal tag properties
	Q_PROPERTY(QString artist READ artist)
	Q_PROPERTY(QString title READ title)
	Q_PROPERTY(QString mp3 READ mp3)
	Q_PROPERTY(QString bpm READ bpm)
	Q_PROPERTY(QString gap READ gap)
	Q_PROPERTY(QString video READ video)
	Q_PROPERTY(QString videogap READ videogap)
	Q_PROPERTY(QString cover READ cover)
	Q_PROPERTY(QString background READ background)
	Q_PROPERTY(QString start READ start)
	Q_PROPERTY(QString language READ language)
	Q_PROPERTY(QString relative READ relative)
	Q_PROPERTY(QString edition READ edition)
	Q_PROPERTY(QString genre READ genre)
	Q_PROPERTY(QString year READ year)
	Q_PROPERTY(QString end READ end)
	Q_PROPERTY(QString creator READ creator)
	// additional properties
	Q_PROPERTY(QString dir READ dir)
	Q_PROPERTY(QString txt READ txt)
	Q_PROPERTY(bool hasMp3 READ hasMp3)
	Q_PROPERTY(bool hasCover READ hasCover)
	Q_PROPERTY(bool hasBackground READ hasBackground)
	Q_PROPERTY(bool hasVideo READ hasVideo)
	Q_PROPERTY(bool isSongChecked READ isSongChecked)

public:
	QUSongFile(const QString &filePath, QObject *parent = 0);
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

	static QString withoutUnsupportedCharacters (const QString &text);

public slots:
	QString artist() const     {return _info.value(ARTIST_TAG,     QString(N_A));}
	QString title() const      {return _info.value(TITLE_TAG,      QString(N_A));}
	QString mp3() const        {return _info.value(MP3_TAG,        QString(N_A));}
	QString bpm() const        {return _info.value(BPM_TAG,        QString(N_A));}
	QString gap() const        {return _info.value(GAP_TAG,        QString(N_A));}
	QString video() const      {return _info.value(VIDEO_TAG,      QString(N_A));}
	QString videogap() const   {return _info.value(VIDEOGAP_TAG,   QString(N_A));}
	QString cover() const      {return _info.value(COVER_TAG,      QString(N_A));}
	QString background() const {return _info.value(BACKGROUND_TAG, QString(N_A));}
	QString start() const      {return _info.value(START_TAG,      QString(N_A));}
	QString language() const   {return _info.value(LANGUAGE_TAG,   QString(N_A));}
	QString relative() const   {return _info.value(RELATIVE_TAG,   QString(N_A));}
	QString edition() const    {return _info.value(EDITION_TAG,    QString(N_A));}
	QString genre() const      {return _info.value(GENRE_TAG,      QString(N_A));}
	QString year() const       {return _info.value(YEAR_TAG,       QString(N_A));}
	QString end() const        {return _info.value(END_TAG,        QString(N_A));}
	QString creator() const    {return _info.value(CREATOR_TAG,    QString(N_A));}

	QString customTag(const QString &tag) const {return _info.value(tag.toUpper(), QString(N_A));}

	QString dir() const {return _fi.dir().dirName();}
	QString path() const {return _fi.path();}
	QString txt() const {return _fi.fileName();}

	bool hasMp3() const;
	bool hasCover() const;
	bool hasBackground() const;
	bool hasVideo() const;
	bool isSongChecked() const; // for [SC]

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
	static QStringList customTags();
	static QStringList noteTypes();

	static void verifyTags(QStringList &tags);
	static QStringList allowedSongFiles();
	static QStringList allowedAudioFiles();
	static QStringList allowedPictureFiles();
	static QStringList allowedVideoFiles();

	static QStringList availableTargets();
	static QStringList availableConditions();
	static QStringList availableSources();

	bool unsupportedTagsFound() const { return _foundUnsupportedTags.size() > 0; }
	QString unsupportedTags() const { return _foundUnsupportedTags.join("\n#"); }
	void removeUnsupportedTags();

	void useExternalFile(const QString &filePath);
	void autoSetFiles();
	void autoSetFile(const QFileInfo &fi, bool force = false);

	void deleteUnusedFiles();
	void clearInvalidFileTags();

	void moveAllFiles(const QString &newRelativePath);

signals:
	void finished(const QString &message, QU::EventMessageTypes type);
	void dataChanged();

private:
	QFileInfo _fi;

	QMap<QString, QString> _info; // song header
	QStringList _lyrics;          // lyrics
	QStringList _footer;          // other stuff after the end mark 'E' in the song file

	QStringList _foundUnsupportedTags;
	bool _hasUnsavedChanges;

	bool updateCache();
	bool rename(QDir dir, const QString &oldName, const QString &newName);
};

#endif /*QUSONGFILE_H_*/
