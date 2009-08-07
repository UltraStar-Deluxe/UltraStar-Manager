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
		this->setDescription(tr("Fix capitalization of #TITLE"));
		this->setToolTip(tr("Applies some rules of capitalization to form a consistent appearance for all song titles."));
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
	}
}

QList<QUSmartSetting*> QUPreparatoryTask::smartSettings() const {
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
			QSettings settings;
			QString timeDiffLower = settings.value("timeDiffLower", 15).toString();
			_smartSettings.append(new QUSmartInputField("preparatory/fixAudioLength", timeDiffLower, new QRegExpValidator(QRegExp("\\d*"), 0), "Buffer:", "seconds"));
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

void QUPreparatoryTask::fixCapitalization(QUSongInterface *song) {
	song->log(tr("NOT IMPLEMENTED"), QU::Warning);
}
