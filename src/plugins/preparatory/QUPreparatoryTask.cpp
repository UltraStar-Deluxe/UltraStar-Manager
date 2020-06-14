#include "QUPreparatoryTask.h"
#include "QUSmartCheckBox.h"
#include "QUSmartInputField.h"
#include "QUSongSupport.h"
#include "QU.h"

#include <QRegExpValidator>
#include <QRegExp>
#include <QSettings>
#include <QFileInfo>
#include <QDir>

QUPreparatoryTask::QUPreparatoryTask(TaskModes mode, QObject *parent):
	QUSimpleTask(parent),
	_mode(mode)
{
	switch(_mode) {
	case AutoAssignFiles:
		this->setIcon(QIcon(":/marks/wand.png"));
		this->setDescription(tr("Assign missing files automatically"));
		this->setToolTip(tr("<b>#MP3</b> ... first audio file found.<br>"
				"<b>#COVER</b> ... first picture file, following a pattern.<br>"
				"<b>#BACKGROUND</b> ... first picture file, following a pattern.<br>"
				"<b>#VIDEO</b> ... first video file found.<br><br>"
				"Does not reset tags that have already been set."));
		break;
	case RemoveUnsupportedTags:
		this->setIcon(QIcon(":/types/folder_blue.png"));
		this->setDescription(tr("Fix or remove unsupported tags"));
		this->setToolTip(tr("Replaces the incorrect <b>#AUTHOR</b> tag by the correct <b>#CREATOR</b> tag and deletes all other unsupported tags."));
		break;
	case FixAudioLength:
		this->setIcon(QIcon(":/types/time_mp3.png"));
		this->setDescription(tr("Fix audio length via #END tag"));
		this->setToolTip(tr("Sets <b>#END</b> to the length of the song if audio file present and longer than song. <b>Appends a little time buffer.</b>"));
		break;
	case RoundGap:
		this->setIcon(QIcon(":/bullets/bullet_black.png"));
		this->setDescription(tr("Round #GAP to nearest integer"));
		break;
	case CapitalizeTitle:
		this->setIcon(QIcon(":/control/text_allcaps.png"));
		this->setDescription(tr("Capitalize #TITLE tag"));
		this->setToolTip(tr("Applies some rules of capitalization to form a consistent appearance for all song titles.<br><br><b>Useful for english songs.</b>"));
		break;
	case CapitalizeArtist:
		this->setIcon(QIcon(":/control/text_allcaps.png"));
		this->setDescription(tr("Capitalize #ARTIST tag"));
		this->setToolTip(tr("Capitalize each word of the artist, e.g. \"michael jackson\" -> \"Michael Jackson\"."));
		break;
	case SetEditionIfEmpty:
		this->setIcon(QIcon(":/types/edition.png"));
		this->setDescription(tr("Set #EDITION tag if it is empty"));
		this->setToolTip(tr("Sets <b>#EDITION</b> to a defined value if it is empty or not present."));
		break;
	case SetGenreIfEmpty:
		this->setIcon(QIcon(":/types/genre.png"));
		this->setDescription(tr("Set #GENRE tag if it is empty"));
		this->setToolTip(tr("Sets <b>#GENRE</b> to a defined value if it is empty or not present."));
		break;
	case FixLanguage:
		this->setIcon(QIcon(":/types/language.png"));
		this->setDescription(tr("Fix #LANGUAGE tag to contain English language names"));
		this->setToolTip(tr("Sets <b>#LANGUAGE</b> to contain English language names (e.g. Deutsch -> German)."));
		break;
	case FixApostrophes:
		this->setIcon(QIcon(":/control/edit_apostrophe.png"));
		this->setDescription(tr("Fix apostrophes in song artist and title"));
		this->setToolTip(tr("Replaces wrongfully used apostrophe symbols `, ´, ' by the correct apostrophe ’"));
		break;
	}
}

QUPreparatoryTask::~QUPreparatoryTask() {
	qDeleteAll(_smartSettings);
	_smartSettings.clear();
}

void QUPreparatoryTask::startOn(QUSongInterface *song) {
	QStringList filter;

	switch(_mode) {
	case AutoAssignFiles:
		autoSetFiles(song, smartSettings().at(0)->value().toString(), smartSettings().at(1)->value().toString());
		break;
	case RemoveUnsupportedTags:
		for(int i = 0; i < _unsupportedTags.size(); i++)
			if(smartSettings().at(i)->value().toBool())
				filter << _unsupportedTags.at(i);
		song->removeUnsupportedTags(filter, true);
		break;
	case FixAudioLength:
		song->fixAudioLength(smartSettings().first()->value().toInt());
		break;
	case RoundGap:
		song->roundGap();
		break;
	case CapitalizeTitle:
		capitalizeTitle(song);
		break;
	case CapitalizeArtist:
		capitalizeArtist(song);
		break;
	case SetEditionIfEmpty:
		setEditionIfEmpty(song);
		break;
	case SetGenreIfEmpty:
		setGenreIfEmpty(song);
		break;
	case FixLanguage:
		fixLanguage(song);
		break;
	case FixApostrophes:
		fixApostrophes(song);
		break;
	}
}

QList<QUSmartSettingInterface*> QUPreparatoryTask::smartSettings() const {
	QSettings settings;
	QString timeDiffLower = settings.value("timeDiffLower", 15).toString();

	if(_smartSettings.isEmpty()) {
		switch(_mode) {
		case AutoAssignFiles:
			_smartSettings.append(new QUSmartInputField("preparatory/autoAssignFiles_coverPattern", "\\[CO\\]", new QRegExpValidator(QRegExp(".*"), nullptr), tr("Pattern:"), tr("(cover)")));
			_smartSettings.append(new QUSmartInputField("preparatory/autoAssignFiles_backgroundPattern", "\\[BG\\]", new QRegExpValidator(QRegExp(".*"), nullptr), tr("Pattern:"), tr("(background)")));
			break;
		case RemoveUnsupportedTags:
			foreach(QString unsupportedTag, _unsupportedTags)
				_smartSettings.append(new QUSmartCheckBox("preparatory/removeUnsupportedTags_" + unsupportedTag, unsupportedTag, true));
			break;
		case FixAudioLength:
			_smartSettings.append(new QUSmartInputField("preparatory/fixAudioLength", timeDiffLower, new QRegExpValidator(QRegExp("\\d*"), nullptr), tr("Buffer:"), tr("seconds")));
			break;
		case CapitalizeTitle:
			_smartSettings.append(new QUSmartCheckBox("preparatory/capitalizeTitle_onlyEnglish", tr("Capitalize English songs only"), true));
			_smartSettings.append(new QUSmartCheckBox("preparatory/capitalizeTitle_allCaps", tr("Capitalize each word"), false));
			_smartSettings.append(new QUSmartCheckBox("preparatory/capitalizeTitle_onlyFirstWord", tr("Capitalize first word only"), false));
			break;
		case CapitalizeArtist:
			_smartSettings.append(new QUSmartCheckBox("preparatory/capitalizeArtist_onlyFirstWord", tr("Capitalize first word only"), false));
			break;
		case SetEditionIfEmpty:
			_smartSettings.append(new QUSmartInputField("preparatory/setEditionIfEmpty_edition", "None", new QRegExpValidator(QRegExp(".*"), nullptr), tr("Edition:"), ""));
			break;
		case SetGenreIfEmpty:
			_smartSettings.append(new QUSmartInputField("preparatory/setGenreIfEmpty_genre", "None", new QRegExpValidator(QRegExp(".*"), nullptr), tr("Genre:"), ""));
			break;
		}
	}

	return _smartSettings;
}

void QUPreparatoryTask::provideData(const QVariant &data, TaskDataTypes type) {
	if(type != UnsupportedTags)
		return;

	_unsupportedTags = data.toStringList();
}

/*!
 * Uses all files in the song directory to guess missing files according to some
 * common patterns: "cover", "[CO]" -> Cover; "back", "[BG]", -> Background, a.s.o.
 */
void QUPreparatoryTask::autoSetFiles(QUSongInterface *song, const QString &coverPattern, const QString &backgroundPattern) {
	QFileInfoList files = song->songFileInfo().dir().entryInfoList(
			QUSongSupport::allowedAudioFiles() + QUSongSupport::allowedImageFiles() + QUSongSupport::allowedVideoFiles(),
			QDir::Files);

	foreach(QFileInfo fi, files) {
		song->autoSetFile(fi, false, coverPattern, backgroundPattern);
	}
}

/*!
 * See http://aitech.ac.jp/~ckelly/midi/help/caps.html for the rules.
 */
void QUPreparatoryTask::capitalizeTitle(QUSongInterface *song) {
	bool onlyEnglish = smartSettings().at(0)->value().toBool();
	bool allCaps = smartSettings().at(1)->value().toBool();
	bool onlyFirstWord = smartSettings().at(2)->value().toBool();

	QStringList l = QString("english englisch american us").split(" ");
	if(onlyEnglish && !song->language().isEmpty() && !l.contains(song->language(), Qt::CaseInsensitive)) {
		song->log(tr("Capitalization fix not applicable due to non-english song: \"%1 - %2\". Try to configure the task.").arg(song->artist()).arg(song->title()), QU::Warning);
		return;
	}

	QStringList words = song->titleCompact().toLower().split(" ", Qt::SkipEmptyParts);
	QStringList suffixes = song->title().split(" ", Qt::SkipEmptyParts).filter(QRegExp("\\[.*\\]")); // extract []-tags

	if(words.isEmpty()) {
		song->log(tr("Capitalization fix not applicable due to empty title: \"%1\".").arg(song->artist()), QU::Warning);
		return;
	}

	QStringList articles = QString("a an the").split(" ");
	QStringList conjunctions = QString("and but or nor feat. ft. con").split(" ");
	QStringList prepositions = QString("as at by for from in into of off on onto out over to up with").split(" ");

	if(!onlyFirstWord) {
		for(int i = 0; i < words.size(); i++) {
			// FIX surrounding parenthesis -> "(about", "to)"
			bool prepP = false, appP = false;
			if(words[i].startsWith("(") && words[i].length() > 1) {
				words[i].remove(0, 1);
				prepP = true;
			}
			if(words[i].endsWith(")") && words[i].length() > 1) {
				words[i].chop(1);
				appP = true;
			}

			// STEP 1 - Capitalize each word
			words[i][0] = words[i][0].toUpper();

			// STEP 2 - Lower-case for articles, conjunctions and short prepositions
			if(!allCaps && (
			   articles.contains(words[i], Qt::CaseInsensitive) ||
			   conjunctions.contains(words[i], Qt::CaseInsensitive) ||
			   prepositions.contains(words[i], Qt::CaseInsensitive)))
				words[i][0] = words[i][0].toLower();

			// RECOVER removed parenthesis
			if(prepP) words[i].prepend("(");
			if(appP) words[i].append(")");
		}
	}

	// STEP 3 - Capitalize "phrasal verbs"
	QStringList phrasalVerbs = QString("Beat Up,Blow Out,Break Down,Break Into,Break Up,Bring Up,Call Off,Call On,Call Up,Carry On,Come Back,Come Down,Come On,Come Out,Come Over,Do Over,Fill In,Fill Out,Find Out,Get Along,Get Around,Get By,Get Over,Get Through,Get Up,Give Back,Give Up,Go Along,Go Away,Go On,Go Over,Hand In,Hang Up,Hold On,Keep On,Keep Up,Leave Out,Let Down,Look For,Look Into,Look Like,Look Out,Look Over,Look Up,Make Out,Make Up,Pack Up,Pass Out,Pick Out,Pick Up,Put Away,Put Off,Put On,Put Out,Put Up,Roll Over,Run Into,Run Out,Run Over,Show Up,Take After,Take Back,Take Off,Take On,Take Up,Talk Back,Talk Over,Throw Away,Try On,Turn Down,Turn In,Turn Off,Turn On,Use Up,Wait On").split(",");
	if(!onlyFirstWord) {
		for(int i = 0; i < words.size() - 1; i++) {
			if(phrasalVerbs.contains(QString("%1 %2").arg(words[i]).arg(words[i+1]), Qt::CaseInsensitive) ||
			   phrasalVerbs.contains(QString("%1 %2") // fix surrounding parenthesis
									 .arg(words[i].right(words[i].length() - 1))
									 .arg(words[i+1].left(words[i+1].length() - 1)), Qt::CaseInsensitive)) {
				words[i][0] = words[i][0].toUpper();
				words[i+1][0] = words[i+1][0].toUpper();
			}
		}
	}

	// LAST STEP - Capitalize first and last word again
	if(words.first().startsWith("(") && words.first().length() > 1)
		words.first()[1] = words.first()[1].toUpper();
	else
		words.first()[0] = words.first()[0].toUpper();

	if(!onlyFirstWord)
		words.last()[0] = words.last()[0].toUpper();

	if(suffixes.isEmpty())
		song->setInfo(TITLE_TAG, words.join(" "));
	else
		song->setInfo(TITLE_TAG, QString("%1 %2").arg(words.join(" ")).arg(suffixes.join(" ")));

	song->log(tr("Capitalization of #TITLE fixed for \"%1 - %2\".").arg(song->artist()).arg(song->title()), QU::Information);
}

/*!
 * The main difference to the capitalization fix for #TITLE tags is
 * that only the case of the first letter of each word is changed.
 */
void QUPreparatoryTask::capitalizeArtist(QUSongInterface *song) {
	bool onlyFirstWord = smartSettings().at(0)->value().toBool();

	QStringList words = song->artist().split(" ", Qt::SkipEmptyParts);

	if(words.isEmpty()) {
		song->log(tr("Capitalization fix not applicable due to empty artist: \"%1\".").arg(song->title()), QU::Warning);
		return;
	}

	if(!onlyFirstWord) {
		for(int i = 0; i < words.size(); i++)
			words[i][0] = words[i][0].toUpper();
	} else {
		for(int i = 0; i < words.size(); i++)
			words[i][0] = words[i][0].toLower();
	}

	words.first()[0] = words.first()[0].toUpper();

	song->setInfo(ARTIST_TAG, words.join(" "));
	song->log(tr("Capitalization of #ARTIST fixed for \"%1 - %2\".").arg(song->artist()).arg(song->title()), QU::Information);
}

/*!
 * Set #EDITION to predefined value if it is empty or not present
 */
void QUPreparatoryTask::setEditionIfEmpty(QUSongInterface *song) {
	if(song->edition() == N_A) {
		song->setInfo(EDITION_TAG, smartSettings().at(0)->value().toString());
		song->log(tr("#EDITION set to \"%1\".").arg(song->edition()), QU::Information);
	}
}

/*!
 * Set #GENRE to predefined value if it is empty or not present
 */
void QUPreparatoryTask::setGenreIfEmpty(QUSongInterface *song) {
	if(song->genre() == N_A) {
		song->setInfo(GENRE_TAG, smartSettings().at(0)->value().toString());
		song->log(tr("#GENRE set to \"%1\".").arg(song->genre()), QU::Information);
	}
}

/*!
 * Fix #LANGUAGE to hold the English word for the language, e.g. Deutsch --> German
 */
void QUPreparatoryTask::fixLanguage(QUSongInterface *song) {
	QString lang = song->language();
	QStringList Arabic, Chinese, Croatian, Czech, Danish, Dutch, English, Finnish, French, German, Hindi, Italian, Japanese, Korean, Latin, Norwegian, Polish, Portuguese, Romanian, Russian, Slovak, Slowenian, Spanish, Swedish, Turkish;
	//				English			German				French				Spanish				Italian				Portuguese			Polish
	Arabic		<< "arabic"		<< "arabisch"		<< "arabe"			<< "árabe"			<< "arabo"			<< "árabe"			<< "arabski";
	Chinese		<< "chinese"	<< "chinesisch"		<< "chinois"		<< "chino"			<< "cinese"			<< "chinês"			<< "chiński";
	Croatian	<< "croatian"	<< "kroatisch"		<< "croate"			<< "croata"			<< "croato"			<< "croata"			<< "chorwacki";
	Czech		<< "czech"		<< "tschechisch"	<< "tchèque"		<< "checo"			<< "ceco"			<< "checo"			<< "czeski";
	Danish		<< "danish"		<< "dänisch"		<< "danois"			<< "danés"			<< "danese"			<< "dinamarquês"	<< "duński";
	Dutch		<< "dutch"		<< "holländisch"	<< "hollandais"		<< "holandés"		<< "olandese"		<< "holandês"		<< "holenderski";
	English		<< "english"	<< "englisch"		<< "anglais"		<< "inglés"			<< "inglese"		<< "inglês"			<< "angielski";
	Finnish		<< "finnish"	<< "finnisch"		<< "finois"			<< "finlandés"		<< "finlandese"		<< "finlandês"		<< "fiński";
	French		<< "french"		<< "französisch"	<< "français"		<< "francés"		<< "francese"		<< "francês"		<< "francuski";
	German		<< "german"		<< "deutsch"		<< "allemand"		<< "alemán"			<< "tedesco"		<< "alemão"			<< "niemiecki";
	Hindi		<< "hindi"		<< "indisch"		<< "indien"			<< "indio"			<< "indiano"		<< "indiano"		<< "indyjski";
	Italian		<< "italian"	<< "italienisch"	<< "italien"		<< "italiano"		<< "italiano"		<< "italiano"		<< "włoski";
	Japanese	<< "japanese"	<< "japanisch"		<< "japonais"		<< "japonés"		<< "giapponese"		<< "japonês"		<< "japoński";
	Korean		<< "korean"		<< "koreanisch"		<< "coréen"			<< "coreano"		<< "coreano"		<< "coreano"		<< "koreański";
	Latin		<< "latin"		<< "lateinisch"		<< "latin"			<< "latino"			<< "latino"			<< "latino"			<< "łacina";
	Norwegian	<< "norwegian"	<< "norwegisch"		<< "norvégien"		<< "noruego"		<< "norvegese"		<< "norueguês"		<< "norweski";
	Polish		<< "polish"		<< "polnisch"		<< "polonais"		<< "polaco"			<< "polacco,"		<< "polaco"			<< "polski";
	Portuguese	<< "portuguese"	<< "portugiesisch"	<< "portugais"		<< "portugués"		<< "portoghese"		<< "português"		<< "portugalski";
	Romanian	<< "romanian"	<< "rumänisch"		<< "roumain"		<< "rumano"			<< "rumeno"			<< "romeno"			<< "rumuński";
	Russian		<< "russian"	<< "russisch"		<< "russe"			<< "ruso"			<< "russo"			<< "russo"			<< "rosyjski" << "ruski";
	Slovak		<< "slovak"		<< "slowakisch"		<< "slovaque"		<< "eslovaco"		<< "slovacco"		<< "eslovaco"		<< "słowacki";
	Slowenian	<< "slowenian"	<< "slowenisch"		<< "slovène"		<< "esloveno"		<< "sloveno"		<< "esloveno"		<< "słoweński";
	Spanish		<< "spanish"	<< "spanisch"		<< "espagnol"		<< "español"		<< "spagnolo"		<< "espanhol"		<< "hiszpański";
	Swedish		<< "swedish"	<< "schwedisch"		<< "suédois"		<< "sueco"			<< "svedese"		<< "sueco"			<< "szwedzki";
	Turkish		<< "turkish"	<< "türkisch"		<< "turc"			<< "turco"			<< "turco"			<< "turco"			<< "turecki ";

	if(Arabic.contains(lang, Qt::CaseInsensitive))			song->setInfo(LANGUAGE_TAG, "Arabic");
	else if(Chinese.contains(lang, Qt::CaseInsensitive))	song->setInfo(LANGUAGE_TAG, "Chinese");
	else if(Croatian.contains(lang, Qt::CaseInsensitive))	song->setInfo(LANGUAGE_TAG, "Croatian");
	else if(Czech.contains(lang, Qt::CaseInsensitive))		song->setInfo(LANGUAGE_TAG, "Czech");
	else if(Danish.contains(lang, Qt::CaseInsensitive))		song->setInfo(LANGUAGE_TAG, "Danish");
	else if(Dutch.contains(lang, Qt::CaseInsensitive))		song->setInfo(LANGUAGE_TAG, "Dutch");
	else if(English.contains(lang, Qt::CaseInsensitive))	song->setInfo(LANGUAGE_TAG, "English");
	else if(Finnish.contains(lang, Qt::CaseInsensitive))	song->setInfo(LANGUAGE_TAG, "Finnish");
	else if(French.contains(lang, Qt::CaseInsensitive))		song->setInfo(LANGUAGE_TAG, "French");
	else if(German.contains(lang, Qt::CaseInsensitive))		song->setInfo(LANGUAGE_TAG, "German");
	else if(Hindi.contains(lang, Qt::CaseInsensitive))		song->setInfo(LANGUAGE_TAG, "Hindi");
	else if(Italian.contains(lang, Qt::CaseInsensitive))	song->setInfo(LANGUAGE_TAG, "Italian");
	else if(Japanese.contains(lang, Qt::CaseInsensitive))	song->setInfo(LANGUAGE_TAG, "Japanese");
	else if(Korean.contains(lang, Qt::CaseInsensitive))		song->setInfo(LANGUAGE_TAG, "Korean");
	else if(Latin.contains(lang, Qt::CaseInsensitive))		song->setInfo(LANGUAGE_TAG, "Latin");
	else if(Norwegian.contains(lang, Qt::CaseInsensitive))	song->setInfo(LANGUAGE_TAG, "Norwegian");
	else if(Polish.contains(lang, Qt::CaseInsensitive))		song->setInfo(LANGUAGE_TAG, "Polish");
	else if(Portuguese.contains(lang, Qt::CaseInsensitive))	song->setInfo(LANGUAGE_TAG, "Portuguese");
	else if(Romanian.contains(lang, Qt::CaseInsensitive))	song->setInfo(LANGUAGE_TAG, "Romanian");
	else if(Russian.contains(lang, Qt::CaseInsensitive))	song->setInfo(LANGUAGE_TAG, "Russian");
	else if(Slovak.contains(lang, Qt::CaseInsensitive))		song->setInfo(LANGUAGE_TAG, "Slovak");
	else if(Slowenian.contains(lang, Qt::CaseInsensitive))	song->setInfo(LANGUAGE_TAG, "Slowenian");
	else if(Spanish.contains(lang, Qt::CaseInsensitive))	song->setInfo(LANGUAGE_TAG, "Spanish");
	else if(Swedish.contains(lang, Qt::CaseInsensitive))	song->setInfo(LANGUAGE_TAG, "Swedish");
	else if(Turkish.contains(lang, Qt::CaseInsensitive))	song->setInfo(LANGUAGE_TAG, "Turkish");

	if(lang != song->language())
		song->log(tr("#LANGUAGE changed from \"%1\" to \"%2\".").arg(lang).arg(song->language()), QU::Information);
}

/*!
 * Replace wrongfully used apostrophe symbols `, ´ and ASCII ' by typographically correct apostrophe ’.
 */
void QUPreparatoryTask::fixApostrophes(QUSongInterface *song) {
	song->setInfo(ARTIST_TAG, song->artist().replace("´", "’")); // acute accent (U+00B4)
	song->setInfo(ARTIST_TAG, song->artist().replace("`", "’")); // grave accent (U+0060)
	song->setInfo(ARTIST_TAG, song->artist().replace("′", "’")); // prime (U+2032)
	song->setInfo(ARTIST_TAG, song->artist().replace("'", "’")); // ASCII apostrophe (not available in ASCII/Latin-9/ISO 8859-15)
	song->setInfo(TITLE_TAG, song->title().replace("´", "’")); // acute accent (U+00B4)
	song->setInfo(TITLE_TAG, song->title().replace("`", "’")); // grave accent (U+0060)
	song->setInfo(TITLE_TAG, song->title().replace("′", "’")); // prime (U+2032)
	song->setInfo(TITLE_TAG, song->title().replace("'", "’")); // ASCII apostrophe (not available in ASCII/Latin-9/ISO 8859-15)
	
	song->log(tr("Apostrophes fixed for \"%1 - %2\".").arg(song->artist()).arg(song->title()), QU::Information);
}
