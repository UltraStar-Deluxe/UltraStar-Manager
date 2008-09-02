#include "QU.h"
#include "QUMetaphoneString.h"

#include <QList>
#include <QPair>

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

/*!
 * Remove all leading spaces and tabs.
 */
QString QU::withoutLeadingBlanks(const QString &text) {
	QString result = text;

	while(result.startsWith(" "))
		result.remove(0, 1);

	while(result.startsWith("\t"))
		result.remove(0, 1);

	return result;
}

QString QU::withoutAnyUmlaut(const QString &text) {
	QString result = text;

	result.replace("ä", "ae", Qt::CaseInsensitive);
	result.replace("ö", "oe", Qt::CaseInsensitive);
	result.replace("ü", "ue", Qt::CaseInsensitive);
	result.replace("ß", "ss", Qt::CaseInsensitive);

	return result;
}

QString QU::withoutAnyUmlautEx(const QString &text) {
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

/*!
 * Checks whether the given tokens could be meant as equal.
 */
bool QU::equal(QString token1, QString token2, bool ignoreEmpty) {
	token1 = QU::withoutAnyUmlautEx(token1).toUpper();
	token2 = QU::withoutAnyUmlautEx(token2).toUpper();

	// remove special characters
	for(int i = 0; i < token1.length(); i++) if(token1[i] != QChar(' ') and token1[i].category() != QChar::Letter_Uppercase) token1.remove(i, 1);
	for(int i = 0; i < token2.length(); i++) if(token2[i] != QChar(' ') and token2[i].category() != QChar::Letter_Uppercase) token2.remove(i, 1);

	// replace "lonely, unimportant" words
	token1.append(' '); token1.prepend(' ');
	token2.append(' '); token2.prepend(' ');
	QRegExp rx(" THE | DER | DIE | DAS | A | AN | EIN ");
	token1.replace(rx, " ");
	token2.replace(rx, " ");

	token1 = token1.simplified().trimmed();
	token2 = token2.simplified().trimmed();

	if(token1.isEmpty() or token2.isEmpty())
		return !ignoreEmpty;

	/* Use MetaPhone */

	QList<QPair<QString, QString> > mp1;
	QList<QPair<QString, QString> > mp2;

	foreach(QString text, token1.split(" ")) {
		QUMetaphoneString mps(text);

		QString m1, m2;
		mps.doDoubleMetaphone(m1, m2);

		mp1.append(QPair<QString, QString>(m1, m2));
	}

	foreach(QString text, token2.split(" ")) {
		QUMetaphoneString mps(text);

		QString m1, m2;
		mps.doDoubleMetaphone(m1, m2);

		mp2.append(QPair<QString, QString>(m1, m2));
	}

	int neededHits = qMin(mp1.size(), mp2.size());
	int hits = 0;


	qDebug("BEGIN");
	for(int i = 0; i < mp1.size(); i++) {
		if(mp1.at(i).first.isEmpty())
			continue; // should not happen...

		for(int j = 0; j < mp2.size(); j++) {
//			if(mp2.at(j).first.isEmpty())
//				continue; // should not happen...

			if(QString::compare(mp1.at(i).first, mp2.at(j).first) == 0) {
				qDebug("[fst, fst] mp1: %s mp2: %s", mp1.at(i).first.toAscii().data(), mp2.at(j).first.toAscii().data());
				hits++;
			} else if(mp2.at(j).second.length() > 1 and mp1.at(i).first == mp2.at(j).second) {
				qDebug("[fst, snd] mp1: %s mp2: %s", mp1.at(i).first.toAscii().data(), mp2.at(j).second.toAscii().data());
				hits++;
			} else if(mp1.at(i).second.length() > 1 and mp1.at(i).second == mp2.at(j).first) {
				qDebug("[snd, fst] mp1: %s mp2: %s", mp1.at(i).second.toAscii().data(), mp2.at(j).first.toAscii().data());
				hits++;
			} else if(mp1.at(i).second.length() > 1 and mp2.at(j).second.length() > 1 and mp1.at(i).second == mp2.at(j).second) {
				qDebug("[snd, snd] mp1: %s mp2: %s", mp1.at(i).second.toAscii().data(), mp2.at(j).second.toAscii().data());
				hits++;
			}

			if(hits >= neededHits) {
				qDebug("END> token1: %s (%d) token2: %s (%d)", token1.toAscii().data(), token1.split(" ").size(), token2.toAscii().data(), token2.split(" ").size());
				return true;
			}
		}
	}

	return false;
}
