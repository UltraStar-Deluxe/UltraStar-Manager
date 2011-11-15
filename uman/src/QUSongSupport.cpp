#include "QUSongSupport.h"
#include "QUSongInterface.h"

#include <QMap>
#include <QSettings>

QUSongSupport::QUSongSupport(QObject *parent): QObject(parent) {}

/*!
 * \returns A list of strings with all available tags. Custom tags are included.
 */
QStringList QUSongSupport::availableTags() {
	QStringList result;

	result << ENCODING_TAG;
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
	result << PREVIEWSTART_TAG;
	result << CALCMEDLEY_TAG;
	result << MEDLEYSTARTBEAT_TAG;
	result << MEDLEYENDBEAT_TAG;
	result << BPM_TAG;
	result << GAP_TAG;

	// all custom tags that will be supported
	result << availableCustomTags();

	return result;
}

QStringList QUSongSupport::availableCustomTags() {
	QSettings settings;
	return settings.value("customTags").toString().split(" ", QString::SkipEmptyParts);
}

QStringList QUSongSupport::availableNoteTypes() {
	QStringList result;

	result << ":"; // normal
	result << "*"; // golden note
	result << "F"; // freestyle
	result << "-"; // pause

	return result;
}

QStringList QUSongSupport::allowedSongFiles() {
	return registryKey("allowedSongFiles", "*.txt *.txd");
}

QStringList QUSongSupport::allowedLicenseFiles() {
	return registryKey("allowedLicenseFiles", "license.txt license.html license.htm");
}

QStringList QUSongSupport::allowedMidiFiles() {
	return registryKey("allowedMidiFiles", "*.mid *.midi");
}

QStringList QUSongSupport::allowedKaraokeFiles() {
	return registryKey("allowedKaraokeFiles", "*.kar");
}

QStringList QUSongSupport::allowedScoreFiles() {
	return registryKey("allowedScoreFiles", "*.sco");
}

QStringList QUSongSupport::allowedAudioFiles() {
	return registryKey("allowedAudioFiles", "*.mp3 *.ogg");
}

QStringList QUSongSupport::allowedImageFiles() {
	return registryKey("allowedImageFiles", "*.jpg *.jpeg *.png");
}

QStringList QUSongSupport::allowedVideoFiles() {
	return registryKey("allowedVideoFiles", "*.mpg *.mpeg *.avi *.flv *.ogm *.mp4 *.divx *.wmv *.mov *.mkv *.vob *.m2v *.m4v *.ts");
}

QStringList QUSongSupport::allowedPlaylistFiles() {
	return registryKey("allowedPlaylistFiles", "*.upl");
}

QStringList QUSongSupport::allowedEncodingTypes() {
	QStringList result;

	result << "CP1252"; // windows-1252 (Western European)
	result << "CP1250"; // windows-1250 (Central/Eastern European)
	result << "UTF8";   // UTF-8

	return result;
}

QStringList QUSongSupport::availableSongLanguages() {
	return registryKey("allowedSongLanguages", "Arabic Chinese Croatian Czech Danish Dutch English Finnish French German Hindi Italian Japanese Korean Latin Norwegian Polish Portuguese Romanian Russian Slovak Slowenian Spanish Swedish Turkish");
}

QString QUSongSupport::defaultInputEncoding() {
	return registryKey("defaultInputEncoding", "CP1252").first();
}

QString QUSongSupport::defaultOutputEncoding() {
	return registryKey("defaultOutputEncoding", "CP1252").first();
}

int QUSongSupport::mediumMp3Quality() {
	return registryKey("mediumMp3Quality", "96").first().toInt();
}

int QUSongSupport::highMp3Quality() {
	return registryKey("highMp3Quality", "128").first().toInt();
}

int QUSongSupport::mediumCoverQuality() {
	return registryKey("mediumCoverQuality", "200 x 200").first().toInt();
}

int QUSongSupport::highCoverQuality() {
	return registryKey("highCoverQuality", "300 x 300").first().toInt();
}

int QUSongSupport::mediumBackgroundQuality() {
	return registryKey("mediumBackgroundQuality", "800 x 450").first().toInt();
}

int QUSongSupport::highBackgroundQuality() {
	return registryKey("highBackgroundQuality", "1280 x 720").first().toInt();
}

int QUSongSupport::mediumVideoQuality() {
	return registryKey("mediumVideoQuality", "800 x 450").first().toInt();
}

int QUSongSupport::highVideoQuality() {
	return registryKey("highVideoQuality", "1280 x 720").first().toInt();
}

/*!
 * Looks for a value in the registry and sets the default, if key not present.
 */
QStringList QUSongSupport::registryKey(const QString &key, const QString &defaultValue) {
// MB: map-related stuff commented out because of update problems with defaultInput/OutputEncoding
//	static QMap<QString, QStringList> map;

//	if(map.contains(key))
//		return map.value(key);

	QSettings settings;
	if(!settings.contains(key))
		settings.setValue(key, defaultValue);

//	map.insert(key, settings.value(key, defaultValue).toString().split(" ", QString::SkipEmptyParts));

//	return map.value(key);
	return settings.value(key, defaultValue).toString().split(" ", QString::SkipEmptyParts);
}
