#ifndef QUSONGSUPPORT_H_
#define QUSONGSUPPORT_H_

#include <QObject>
#include <QStringList>

class QUSongSupport: QObject {
	Q_OBJECT

public:
	QUSongSupport(QObject *parent = nullptr);

	static QStringList availableTags();
	static QStringList availableCustomTags();
	static QStringList availableNoteTypes();

	static QStringList allowedSongFiles();
	static QStringList allowedLicenseFiles();
	static QStringList allowedMidiFiles();
	static QStringList allowedKaraokeFiles(); // MIDI + Lyrics
	static QStringList allowedScoreFiles();
	static QStringList allowedAudioFiles();
	static QStringList allowedImageFiles();
	static QStringList allowedVideoFiles();
	static QStringList allowedPlaylistFiles();
	static QStringList allowedUltraStarPlaylistFiles();
	static QStringList allowedVocaluxePlaylistFiles();
	static QStringList allowedSyncFiles();

	static QStringList allowedEncodingTypes();
	static QStringList availableSongLanguages();
	static QStringList availableSongEditions();
	static QStringList availableSongGenres();
	static QStringList availableSongYears();

	static QString defaultInputEncoding();

	static int mediumMp3Quality();
	static int highMp3Quality();
	static int mediumCoverQuality();
	static int highCoverQuality();
	static int mediumBackgroundQuality();
	static int highBackgroundQuality();
	static int mediumVideoQuality();
	static int highVideoQuality();
	
	static QString langToLangCode(const QString &language);

private:
	static QStringList registryKey(const QString &key, const QString &defaultValue);

};

#endif // QUSTRINGSUPPORT_H_
