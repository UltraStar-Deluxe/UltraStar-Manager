#include "QU.h"

QStringList QU::allowedSongFiles() {
	return QString("*.txt *.kar").split(" ");
}

QStringList QU::allowedAudioFiles() {
	return QString("*.mp3 *.ogg").split(" ");
}

QStringList QU::allowedPictureFiles() {
	return QString("*.jpg *.png *.bmp *.gif").split(" ");
}

QStringList QU::allowedVideoFiles() {
	return QString("*.mpg *.mpeg *.avi *.flv *.ogm *.mp4 *.divx").split(" ");
}

QStringList QU::allowedPlaylistFiles() {
	return QString("*.upl").split(" ");
}

/*!
 * Removes all characters of the given text that cannot be used in a file or
 * directory name.
 *
 * Path separator '/' is not removed.
 *
 * \param text A single line of text without line breaks.
 */
QString QU::withoutUnsupportedCharacters (const QString &text) {
	QString cleanText = text;
#ifdef Q_OS_WIN32
	cleanText.remove(QRegExp("[\\\\:\\*\\?\"\\|<>]"));

	// remove trailing dots
	while(cleanText.endsWith("."))
		cleanText.remove(cleanText.length() - 1, 1);

	while (cleanText.startsWith("."))
		cleanText.remove(0, 1);
#endif
	return cleanText;
}

/*!
 * Remove all "folder tags" like [SC], [VIDEO], a.s.o. from the given text.
 */
QString QU::withoutFolderTags(const QString &text) {
	QString result = text;
	QRegExp r("\\[.*\\]");
	r.setMinimal(true);

	return result.remove(r).trimmed();
}
