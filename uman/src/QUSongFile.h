#ifndef QUSONGFILE_H_
#define QUSONGFILE_H_

#include <QObject>
#include <QFileInfo>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QDir>

/*!
 * This class represents a data file which is used by UltraStar for every song.
 * It contains all tags that are available in US Deluxe 1.00.
 */
class QUSongFile: public QObject {
	Q_OBJECT
	
public:
	QUSongFile(const QString &file, QObject *parent = 0);
	~QUSongFile();
	
public slots:
	QString artist() const     {return _info.value("ARTIST", QString("n/a"));}
	QString title() const      {return _info.value("TITLE", QString("n/a"));}
	QString mp3() const        {return _info.value("MP3", QString("n/a"));}
	QString bpm() const        {return _info.value("BPM", QString("n/a"));}
	QString gap() const        {return _info.value("GAP", QString("n/a"));}
	QString video() const      {return _info.value("VIDEO", QString("n/a"));}
	QString videogap() const   {return _info.value("VIDEOGAP", QString("n/a"));}
	QString cover() const      {return _info.value("COVER", QString("n/a"));}
	QString background() const {return _info.value("BACKGROUND", QString("n/a"));}
	QString start() const      {return _info.value("START", QString("n/a"));}
	QString language() const   {return _info.value("LANGUAGE", QString("n/a"));}
	QString relative() const   {return _info.value("RELATIVE", QString("n/a"));}
	QString edition() const    {return _info.value("EDITION", QString("n/a"));}
	QString genre() const      {return _info.value("GENRE", QString("n/a"));}
	
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
	
	bool save();
	bool renameSongDir(const QString &newName);
	bool renameSongTxt(const QString &newName);
	bool renameSongMp3(const QString &newName);
	bool renameSongCover(const QString &newName);
	bool renameSongBackground(const QString &newName);
	bool renameSongVideo(const QString &newName);
	
	bool useID3TagForArtist();
	bool useID3TagForTitle();
	bool useID3TagForGenre();
	
	static QStringList tags();
	static void verifyTags(QStringList &tags);
	static QStringList allowedAudioFiles();
	static QStringList allowedPictureFiles();
	static QStringList allowedVideoFiles();
	
	bool unsupportedTagsFound() const { return _foundUnsupportedTags.size() > 0; }
	QString unsupportedTags() const { return _foundUnsupportedTags.join("\n#"); }
	
private:
	QFileInfo _fi;
	QFile     _file;
	QMap<QString, QString> _info;
	QStringList _lyrics;
	QStringList _foundUnsupportedTags;
	
	bool updateCache();
};

#endif /*QUSONGFILE_H_*/
