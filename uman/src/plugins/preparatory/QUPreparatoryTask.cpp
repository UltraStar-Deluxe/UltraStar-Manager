#include "QUPreparatoryTask.h"
#include "QUSmartCheckBox.h"
#include "QUSmartInputField.h"
#include "QUSongSupport.h"

#include <QRegExpValidator>
#include <QRegExp>
#include <QSettings>
#include <QFileInfo>
#include <QDir>

QUPreparatoryTask::QUPreparatoryTask(QU::PreparatoryTaskModes mode, QObject *parent):
	QUSimpleTask(parent),
	_mode(mode)
{
	switch(_mode) {
	case QU::autoAssignFiles:
		this->setIcon(QIcon(":/marks/wand.png"));
		this->setDescription(tr("Assign missing files automatically"));
		this->setToolTip(tr("<b>#MP3</b> ... first audio file found.<br>"
				"<b>#COVER</b> ... first picture file, following a pattern.<br>"
				"<b>#BACKGROUND</b> ... first picture file, following a pattern.<br>"
				"<b>#VIDEO</b> ... first video file found."));
		break;
	case QU::removeUnsupportedTags:
		this->setIcon(QIcon(":/types/folder_blue.png"));
		this->setDescription(tr("Remove unsupported tags"));
		break;
	case QU::fixAudioLength:
		this->setIcon(QIcon(":/types/time_mp3.png"));
		this->setDescription(tr("Fix audio length via #END tag"));
		this->setToolTip(tr("Sets <b>#END</b> to the length of the song if audio file present and longer than song. <b>Appends a little time buffer.</b>"));
		break;
	case QU::roundGap:
		this->setIcon(QIcon(":/bullets/bullet_black.png"));
		this->setDescription(tr("Round #GAP to nearest integer."));
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
	case QU::autoAssignFiles:
		autoSetFiles(song, smartSettings().at(0)->value().toString(), smartSettings().at(1)->value().toString());
		break;
	case QU::removeUnsupportedTags:
		for(int i = 0; i < _unsupportedTags.size(); i++)
			if(smartSettings().at(i)->value().toBool())
				filter << _unsupportedTags.at(i);
		song->removeUnsupportedTags(filter, true);
		break;
	case QU::fixAudioLength:
		song->fixAudioLength(smartSettings().first()->value().toInt());
		break;
	case QU::roundGap:
		song->roundGap();
		break;
	}
}

QList<QUSmartSetting*> QUPreparatoryTask::smartSettings() const {
	if(_smartSettings.isEmpty()) {
		switch(_mode) {
		case QU::autoAssignFiles:
			_smartSettings.append(new QUSmartInputField("preparatory/autoAssignFiles_coverPattern", "\\[CO\\]|cove?r?", new QRegExpValidator(QRegExp(".*"), 0), "Pattern:", "(cover)"));
			_smartSettings.append(new QUSmartInputField("preparatory/autoAssignFiles_backgroundPattern", "\\[BG\\]|back", new QRegExpValidator(QRegExp(".*"), 0), "Pattern:", "(background)"));
			break;
		case QU::removeUnsupportedTags:
			foreach(QString unsupportedTag, _unsupportedTags)
				_smartSettings.append(new QUSmartCheckBox("preparatory/removeUnsupportedTags_" + unsupportedTag, unsupportedTag, true));
			break;
		case QU::fixAudioLength:
			QSettings settings;
			QString timeDiffLower = settings.value("timeDiffLower", LOWER_TIME_BOUND_DEFAULT).toString();
			_smartSettings.append(new QUSmartInputField("preparatory/fixAudioLength", timeDiffLower, new QRegExpValidator(QRegExp("\\d*"), 0), "Buffer:", "seconds"));
			break;
		}
	}

	return _smartSettings;
}

void QUPreparatoryTask::provideData(const QVariant &data, QU::TaskDataTypes type) {
	if(type != QU::unsupportedTags)
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
