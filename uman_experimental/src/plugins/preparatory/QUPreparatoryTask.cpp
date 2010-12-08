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
		this->setDescription(tr("Remove unsupported tags"));
		break;
	case FixAudioLength:
		this->setIcon(QIcon(":/types/time_mp3.png"));
		this->setDescription(tr("Fix audio length via #END tag"));
		this->setToolTip(tr("Sets <b>#END</b> to the length of the song if audio file present and longer than song. <b>Appends a little time buffer.</b>"));
		break;
	case RoundGap:
		this->setIcon(QIcon(":/bullets/bullet_black.png"));
		this->setDescription(tr("Round #GAP to nearest integer."));
		break;
	case FixCapitalization:
		this->setIcon(QIcon(":/control/text_allcaps.png"));
		this->setDescription(tr("Fix capitalization of #TITLE tag"));
		this->setToolTip(tr("Applies some rules of capitalization to form a consistent appearance for all song titles.<br><br><b>Useful for english songs.</b>"));
		break;
	case CapitalizeArtist:
		this->setIcon(QIcon(":/control/text_allcaps.png"));
		this->setDescription(tr("Capitalize #ARTIST tag"));
		this->setToolTip(tr("Capitalize each word of the artist, e.g. \"michael jackson\" -> \"Michael Jackson\"."));
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
	case FixCapitalization:
		fixCapitalization(song);
		break;
	case CapitalizeArtist:
		capitalizeArtist(song);
		break;
	}
}

QList<QUSmartSettingInterface*> QUPreparatoryTask::smartSettings() const {
	QSettings settings;
	QString timeDiffLower = settings.value("timeDiffLower", 15).toString();

	if(_smartSettings.isEmpty()) {
		switch(_mode) {
		case AutoAssignFiles:
			_smartSettings.append(new QUSmartInputField("preparatory/autoAssignFiles_coverPattern", "\\[CO\\]|cove?r?", new QRegExpValidator(QRegExp(".*"), 0), "Pattern:", "(cover)"));
			_smartSettings.append(new QUSmartInputField("preparatory/autoAssignFiles_backgroundPattern", "\\[BG\\]|back", new QRegExpValidator(QRegExp(".*"), 0), "Pattern:", "(background)"));
			break;
		case RemoveUnsupportedTags:
			foreach(QString unsupportedTag, _unsupportedTags)
				_smartSettings.append(new QUSmartCheckBox("preparatory/removeUnsupportedTags_" + unsupportedTag, unsupportedTag, true));
			break;
		case FixAudioLength:
			_smartSettings.append(new QUSmartInputField("preparatory/fixAudioLength", timeDiffLower, new QRegExpValidator(QRegExp("\\d*"), 0), "Buffer:", "seconds"));
			break;
		case FixCapitalization:
			_smartSettings.append(new QUSmartCheckBox("preparatory/fixCapitalization_onlyEnglish", tr("Capitalize English songs only"), true));
			_smartSettings.append(new QUSmartCheckBox("preparatory/fixCapitalization_allCaps", tr("Capitalize each word"), false));
			_smartSettings.append(new QUSmartCheckBox("preparatory/fixCapitalization_onlyFirstWord", tr("Capitalize first word only"), false));
			break;
		case CapitalizeArtist:
			_smartSettings.append(new QUSmartCheckBox("preparatory/capitalizeArtist_onlyFirstWord", tr("Capitalize first word only"), false));
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
			QUSongSupport::allowedAudioFiles() + QUSongSupport::allowedPictureFiles() + QUSongSupport::allowedVideoFiles(),
			QDir::Files);

	foreach(QFileInfo fi, files) {
		song->autoSetFile(fi, false, coverPattern, backgroundPattern);
	}
}

/*!
 * See http://aitech.ac.jp/~ckelly/midi/help/caps.html for the rules.
 */
void QUPreparatoryTask::fixCapitalization(QUSongInterface *song) {
	bool onlyEnglish = smartSettings().at(0)->value().toBool();
	bool allCaps = smartSettings().at(1)->value().toBool();
	bool onlyFirstWord = smartSettings().at(2)->value().toBool();

	QStringList l = QString("english englisch american us").split(" ");
	if(onlyEnglish && !song->language().isEmpty() && !l.contains(song->language(), Qt::CaseInsensitive)) {
		song->log(tr("Capitalization fix not applicable due to non-english song: \"%1 - %2\". Try to configure the task.").arg(song->artist()).arg(song->title()), QU::Warning);
		return;
	}

	QStringList words = song->titleCompact().toLower().split(" ", QString::SkipEmptyParts);
	QStringList suffixes = song->title().split(" ", QString::SkipEmptyParts).filter(QRegExp("\\[.*\\]")); // extract []-tags

	if(words.isEmpty()) {
		song->log(tr("Capitalization fix not applicable due to empty title: \"%1\"").arg(song->artist()), QU::Warning);
		return;
	}

	QStringList articles = QString("a an the").split(" ");
	QStringList conjunctions = QString("and but or nor feat. con").split(" ");
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

	song->log(tr("Capitalization of #TITLE fixed for \"%1 - %2\"").arg(song->artist()).arg(song->title()), QU::Information);
}

/*!
 * The main difference to the capitalization fix for #TITLE tags is
 * that only the case of the first letter of each word is changed.
 */
void QUPreparatoryTask::capitalizeArtist(QUSongInterface *song) {
	bool onlyFirstWord = smartSettings().at(0)->value().toBool();

	QStringList words = song->artist().split(" ", QString::SkipEmptyParts);

	if(words.isEmpty()) {
		song->log(tr("Capitalization fix not applicable due to empty artist: \"%1\"").arg(song->title()), QU::Warning);
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
	song->log(tr("Capitalization of #ARTIST fixed for \"%1 - %2\"").arg(song->artist()).arg(song->title()), QU::Information);
}
