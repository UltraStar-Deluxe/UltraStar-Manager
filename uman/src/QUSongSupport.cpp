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
	QStringList result;

	result << "Arabic";
	result << "Chinese";
	result << "Croatian";
	result << "Czech";
	result << "Danish";
	result << "Dutch";
	result << "English";
	result << "Finnish";
	result << "French";
	result << "German";
	result << "Hindi";
	result << "Italian";
	result << "Japanese";
	result << "Korean";
	result << "Latin";
	result << "Norwegian";
	result << "Polish";
	result << "Portuguese";
	result << "Romanian";
	result << "Russian";
	result << "Slovak";
	result << "Slowenian";
	result << "Spanish";
	result << "Swedish";
	result << "Turkish";

	return result;
}

QStringList QUSongSupport::availableSongEditions() {
	QStringList result;

	result << "[SC]-Songs";
	result << "Band Hero";
	result << "Disney Sing It";
	result << "Disney Sing It Family Hits";
	result << "Disney Sing It Pop Hits";
	result << "Disney Sing It Pop Hits USA";
	result << "Disney Sing It! High School Musical 3";
	result << "Disney Sing It! High School Musical 3 USA";
	result << "Green Day Rock Band";
	result << "Guitar Hero 5";
	result << "Guitar Hero Greatest Hits";
	result << "Guitar Hero Metallica";
	result << "Guitar Hero Warriors of Rock";
	result << "Guitar Hero World Tour";
	result << "Lego Rock Band";
	result << "Rock Band";
	result << "Rock Band 2";
	result << "Rock Band 3";
	result << "Rock Band Country Track Pack";
	result << "Rock Band Europe";
	result << "Rock Band Store 2007";
	result << "Rock Band Store 2008 Vol. 1";
	result << "Rock Band Store 2008 Vol. 2";
	result << "Rock Band Store 2008 Vol. 3";
	result << "Rock Band Store 2008 Vol. 4";
	result << "Rock Band Store 2009 Vol. 1";
	result << "Rock Band Store 2009 Vol. 2";
	result << "Rock Band Store 2009 Vol. 3";
	result << "Rock Band Store 2009 Vol. 4";
	result << "Rock Band Store 2010 Vol. 1";
	result << "Rock Band Store 2010 Vol. 3";
	result << "Rock Band Store 2010 Vol. 4";
	result << "Rock Band Store 2011 Vol. 1";
	result << "Rock Band Store 2011 Vol. 2";
	result << "Rock Band Store 2011 Vol. 3";
	result << "Rock Band Store 2011 Vol. 4";
	result << "Rock Band Track Pack AC/DC Live";
	result << "Rock Band Track Pack Classic Rock";
	result << "Rock Band Track Pack Vol. 1";
	result << "Rock Band Track Pack Vol. 2";
	result << "Rock Band Wii";
	result << "SingStar";
	result << "SingStar '80s";
	result << "SingStar '80s Dutch";
	result << "SingStar '80s French";
	result << "SingStar '80s German";
	result << "SingStar '80s Italian";
	result << "SingStar '80s Polish";
	result << "SingStar '80s Swedish";
	result << "SingStar '80s USA";
	result << "SingStar '90s";
	result << "SingStar '90s USA";
	result << "SingStar ABBA";
	result << "SingStar Amped";
	result << "SingStar Amped Australian";
	result << "SingStar Amped German";
	result << "SingStar Amped USA";
	result << "SingStar Anthems";
	result << "SingStar Anthems UK";
	result << "SingStar Après-Ski Party";
	result << "SingStar Aprés-Ski Party German";
	result << "SingStar Aprés-Ski Party 2";
	result << "SingStar Aprés-Ski Party 2 German";
	result << "SingStar best of Disney";
	result << "SingStar best of Disney German";
	result << "SingStar Bollywood";
	result << "SingStar Bollywood UK";
	result << "SingStar Boy Bands vs Girl Bands";
	result << "SingStar Boy Bands vs Girl Bands UK";
	result << "SingStar Canções Disney";
	result << "SingStar canciones Disney";
	result << "SingStar Chansons Magiques de Disney";
	result << "SingStar Chansons Magiques de Disney [FR]";
	result << "SingStar Chartbreaker";
	result << "SingStar Chartbreaker [DE]";
	result << "SingStar Clásicos";
	result << "SingStar Clásicos [ES]";
	result << "SingStar Country";
	result << "SingStar Country [US]";
	result << "SingStar Deutsch Rock-Pop";
	result << "SingStar Deutsch Rock-Pop Vol. 2";
	result << "SingStar Deutsch Rock-Pop Vol. 2 [DE]";
	result << "SingStar Deutsch Rock-Pop [DE]";
	result << "SingStar Die größten Solokünstler";
	result << "SingStar Die größten Solokünstler [DE]";
	result << "SingStar Die Toten Hosen";
	result << "SingStar Die Toten Hosen [DE]";
	result << "SingStar e la magia Disney";
	result << "SingStar e la magia Disney [IT]";
	result << "SingStar English";
	result << "SingStar ESKA Hity Na Czasie";
	result << "SingStar French";
	result << "SingStar Fußballhits [DE]";
	result << "SingStar German";
	result << "SingStar Hottest Hits";
	result << "SingStar Hottest Hits [DE] [UK]";
	result << "SingStar Italian";
	result << "SingStar Italian Greatest Hits";
	result << "SingStar Italian Greatest Hits [IT]";
	result << "SingStar Italian Party";
	result << "SingStar Italian Party 2";
	result << "SingStar Italian Party 2 [IT]";
	result << "SingStar Italian Party [IT]";
	result << "SingStar La Edad de Oro del Pop Español";
	result << "SingStar Latino";
	result << "SingStar Latino Portuguese";
	result << "SingStar Latino [ES] [PT]";
	result << "SingStar Legendat";
	result << "SingStar Legendat [FI]";
	result << "SingStar Legends";
	result << "SingStar Legends Australian";
	result << "SingStar Legends Danish";
	result << "SingStar Legends French";
	result << "SingStar Legends German";
	result << "SingStar Legends Italian";
	result << "SingStar Legends USA";
	result << "SingStar Legends [DK]";
	result << "SingStar Legends [FR]";
	result << "SingStar Legends [IT]";
	result << "SingStar Legends [UK] [AUS [US] [DE] [DK]";
	result << "SingStar Made in Germany";
	result << "SingStar Made In Germany [DE]";
	result << "SingStar Mallorca Party";
	result << "SingStar Mallorca Party [DE]";
	result << "SingStar Mecano";
	result << "SingStar Miliki";
	result << "SingStar Morangos com Açúcar [PT]";
	result << "SingStar Morangos com Açucar";
	result << "SingStar Motown";
	result << "SingStar Motown [UK]";
	result << "SingStar Norsk På Norsk";
	result << "SingStar Norske Hits";
	result << "SingStar Norske Hits [NO]";
	result << "SingStar NRJ";
	result << "SingStar NRJ Music Tour [FR]";
	result << "SingStar Operación Triunfo";
	result << "SingStar Operación Triunfo [ES]";
	result << "SingStar Party";
	result << "SingStar Party English";
	result << "SingStar Party German";
	result << "SingStar Party Hits";
	result << "SingStar Party Italian";
	result << "SingStar Party [IT]";
	result << "SingStar Party [UK] [DE] [IT] [DE] - SingStar NRJ Music Tour [FR]";
	result << "SingStar Patito Feo";
	result << "SingStar Polskie Hity";
	result << "SingStar Pop";
	result << "SingStar Pop - SingStar Popworld [UK] - SingStar The Dome [DE]";
	result << "SingStar Pop 2009";
	result << "SingStar Pop 2009 [ES]";
	result << "SingStar Pop Australian";
	result << "SingStar Pop Hits";
	result << "SingStar Pop Hits 2";
	result << "SingStar Pop Hits 2 [FR]";
	result << "SingStar Pop Hits 3";
	result << "SingStar Pop Hits 3 [FR]";
	result << "SingStar Pop Hits 4";
	result << "SingStar Pop Hits 4 [FR]";
	result << "SingStar Pop Hits Australian";
	result << "SingStar Pop Hits de Los 40 Principales";
	result << "SingStar Pop Hits French";
	result << "SingStar Pop Hits German";
	result << "SingStar Pop Hits Portuguese";
	result << "SingStar Pop Hits Scandinavisk";
	result << "SingStar Pop Hits Skandinavisk [DK]";
	result << "SingStar Pop Hits [FR]";
	result << "SingStar Pop Hits [UK] [AUS] [DE] [ES]";
	result << "SingStar Pop USA";
	result << "SingStar Pop Vol. 2";
	result << "SingStar Pop Vol. 2 [US]";
	result << "SingStar Popworld";
	result << "SingStar Portugal Hits";
	result << "SingStar Queen";
	result << "SingStar Queen";
	result << "SingStar R&amp;B";
	result << "SingStar R&amp;B Polish";
	result << "SingStar R&B";
	result << "SingStar Radio 105";
	result << "SingStar Radio 105 [IT]";
	result << "SingStar Rock Ballads";
	result << "SingStar Rock Ballads [UK]";
	result << "SingStar Rocks!";
	result << "SingStar Rocks!";
	result << "SingStar Rocks! Australian";
	result << "SingStar Rocks! English";
	result << "SingStar Rocks! French";
	result << "SingStar Rocks! HRVATSKA!";
	result << "SingStar Rocks! HRVATSKA! [HR]";
	result << "SingStar Rocks! TMF";
	result << "SingStar Rocks! TMF [NL]";
	result << "SingStar Rocks! USA";
	result << "SingStar Rocks! [FR]";
	result << "SingStar Russian Hits";
	result << "SingStar Schlager";
	result << "SingStar Schlager [DE]";
	result << "SingStar Singalong with Disney";
	result << "SingStar Singalong With Disney [US]";
	result << "SingStar sjung med Disney";
	result << "SingStar Summer Party";
	result << "SingStar Summer Party Danish";
	result << "SingStar Summer Party English";
	result << "SingStar Summer Party German";
	result << "SingStar Summer Party Portuguese";
	result << "SingStar Summer Party [DK]";
	result << "SingStar Summer Party [UK] [DE] - SingStar Party Hits [AUS]";
	result << "SingStar SuomiPop";
	result << "SingStar SuomiPop [FI]";
	result << "SingStar SuomiRock";
	result << "SingStar SuomiRock [FI]";
	result << "SingStar Svenska Hits";
	result << "SingStar Svenska Hits Schlager";
	result << "SingStar Svenska Hits [SE]";
	result << "SingStar Take That";
	result << "SingStar Take That [UK]";
	result << "SingStar The Dome";
	result << "SingStar The Wiggles";
	result << "SingStar The Wiggles [AU]";
	result << "SingStar top.it";
	result << "SingStar top.it [IT]";
	result << "SingStar Turkish Party";
	result << "SingStar Turkish Party [DE]";
	result << "SingStar Vasco [IT]";
	result << "SingStar Vasco";
	result << "SingStar zingt met Disney [NL]";
	result << "SingStar [DE] [UK] [FR]";
	result << "SingStar [FR]";
	result << "SingStar [IT]";

	return result;
}

QStringList QUSongSupport::availableSongGenres() {
	QStringList result;

	result << "A capella";
	result << "Acid";
	result << "Acid Jazz";
	result << "Acid Punk";
	result << "Acoustic";
	result << "AlternRock";
	result << "Alternative";
	result << "Ambient";
	result << "Avantgarde";
	result << "Ballad";
	result << "Bass";
	result << "Bebob";
	result << "Big Band";
	result << "Bluegrass";
	result << "Blues";
	result << "Booty Bass";
	result << "Cabaret";
	result << "Celtic";
	result << "Chamber Music";
	result << "Chanson";
	result << "Chorus";
	result << "Christian Rap";
	result << "Classic Rock";
	result << "Classical";
	result << "Club";
	result << "Comedy";
	result << "Country";
	result << "Cult";
	result << "Dance";
	result << "Dance Hall";
	result << "Darkwave";
	result << "Death Metal";
	result << "Disco";
	result << "Dream";
	result << "Duet";
	result << "Easy Listening";
	result << "Electronic";
	result << "Ethnic";
	result << "Euro-House";
	result << "Euro-Techno";
	result << "Eurodance";
	result << "Fast Fusion";
	result << "Folk";
	result << "Folk-Rock";
	result << "Folklore";
	result << "Freestyle";
	result << "Funk";
	result << "Fusion";
	result << "Game";
	result << "Gangsta";
	result << "Gospel";
	result << "Gothic";
	result << "Gothic Rock";
	result << "Grunge";
	result << "Hard Rock";
	result << "Hip-Hop";
	result << "House";
	result << "Humour";
	result << "Industrial";
	result << "Instrumental";
	result << "Instrumental Pop";
	result << "Instrumental Rock";
	result << "Jazz";
	result << "Jazz+Funk";
	result << "Jungle";
	result << "Latin";
	result << "Lo-Fi";
	result << "Meditative";
	result << "Metal";
	result << "Musical";
	result << "National Folk";
	result << "Native American";
	result << "New Age";
	result << "New Wave";
	result << "Noise";
	result << "Oldies";
	result << "Opera";
	result << "Other";
	result << "Polka";
	result << "Pop";
	result << "Pop-Folk";
	result << "Pop/Funk";
	result << "Porn Groove";
	result << "Power Ballad";
	result << "Pranks";
	result << "Primus";
	result << "Progressive Rock";
	result << "Psychadelic";
	result << "Psychedelic Rock";
	result << "Punk";
	result << "Punk Rock";
	result << "R&B";
	result << "Rap";
	result << "Rave";
	result << "Reggae";
	result << "Retro";
	result << "Revival";
	result << "Rhythmic Soul";
	result << "Rock";
	result << "Rock & Roll";
	result << "Samba";
	result << "Satire";
	result << "Showtunes";
	result << "Ska";
	result << "Slow Jam";
	result << "Slow Rock";
	result << "Sonata";
	result << "Soul";
	result << "Sound Clip";
	result << "Soundtrack";
	result << "Southern Rock";
	result << "Space";
	result << "Speech";
	result << "Swing";
	result << "Symphonic Rock";
	result << "Symphony";
	result << "Tango";
	result << "Techno";
	result << "Techno-Industrial";
	result << "Top";
	result << "Trailer";
	result << "Trance";
	result << "Tribal";
	result << "Trip-Hop";
	result << "Vocal";

	return result;
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
