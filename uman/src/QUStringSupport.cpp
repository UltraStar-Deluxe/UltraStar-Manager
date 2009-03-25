#include "QUStringSupport.h"

#include <QRegExp>

QUStringSupport::QUStringSupport(QObject *parent): QObject(parent) {}
    
/*!
 * Removes all characters of the given text that cannot be used in a file or
 * directory name.
 *
 * Path separator '/' is not removed.
 *
 * \param text A single line of text without line breaks.
 */
QString QUStringSupport::withoutUnsupportedCharacters (const QString &text) {
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
QString QUStringSupport::withoutFolderTags(const QString &text) {
	QString result = text;
	QRegExp r("\\[.*\\]");
	r.setMinimal(true);

	return result.remove(r).trimmed();
}

/*!
 * Remove all leading spaces and tabs.
 */
QString QUStringSupport::withoutLeadingBlanks(const QString &text) {
	QString result = text;

	while(result.startsWith(" "))
		result.remove(0, 1);

	while(result.startsWith("\t"))
		result.remove(0, 1);

	return result;
}

QString QUStringSupport::withoutAnyUmlaut(const QString &text) {
	QString result = text;

	result.replace("ä", "ae", Qt::CaseInsensitive);
	result.replace("ö", "oe", Qt::CaseInsensitive);
	result.replace("ü", "ue", Qt::CaseInsensitive);
	result.replace("ß", "ss", Qt::CaseInsensitive);

	return result;
}

QString QUStringSupport::withoutAnyUmlautEx(const QString &text) {
	QString result = text;

	result.replace("ä", "ae", Qt::CaseInsensitive);
	result.replace("ö", "oe", Qt::CaseInsensitive);
	result.replace("ü", "ue", Qt::CaseInsensitive);
	result.replace("ß", "ss", Qt::CaseInsensitive);

	result.replace("ô", "o", Qt::CaseInsensitive);
	result.replace("é", "e", Qt::CaseInsensitive);
	result.replace("è", "e", Qt::CaseInsensitive);

	result.replace("_", " ");
	result.replace("-", " ");
	result.replace("~", " ");
	result.replace("#", " ");

	return result;
}

QStringList QUStringSupport::extractTags(const QString &text) {
	// Step 1: "Wizo - Hund [karaoke] [blubb].txt" -> "karaoke] ", "blubb].txt"
	QStringList tags(text.split("[").filter("]") << text.split("(").filter(")"));

	// Step 2: "karaoke] ", "blubb].txt" -> "karaoke", "blubb"
	foreach(QString tag, tags) {
		tag.remove(QRegExp("\\].*"));
		tag.remove(QRegExp("\\).*"));
		tag = tag.trimmed();
	}

	tags.removeDuplicates();
	return tags;
}
