#include "QUPreparatoryTask.h"

QUPreparatoryTask::QUPreparatoryTask(QU::PreparatoryTaskModes mode, QObject *parent):
	QUAbstractTask(parent),
	_mode(mode)
{
	switch(_mode) {
	case QU::autoAssignFiles:
		this->setIcon(QIcon(":/marks/wand.png"));
		this->setDescription(tr("Assign missing files automatically"));
		this->setToolTip(tr("<b>#MP3</b> ... first audio file found.<br>"
				"<b>#COVER</b> ... first picture file, including pattern: <i>[CO]|cove?r?</i>.<br>"
				"<b>#BACKGROUND</b> ... first picture file, including pattern: <i>[BG]|back</i>.<br>"
				"<b>#VIDEO</b> ... first video file found."));
		break;
	case QU::removeUnsupportedTags:
		this->setIcon(QIcon(":/types/folder_blue.png"));
		this->setDescription(tr("Remove unsupported tags"));
		break;
	case QU::fixAudioLength:
		this->setIcon(QIcon(":/types/time_mp3.png"));
		this->setDescription(tr("Fix audio length via #END tag."));
		this->setToolTip(tr("Sets <b>#END</b> to the length of the song if audio file present and longer than song. <b>Appends a little time buffer.</b>"));
		break;
	case QU::fixTimeStamps:
		this->setDescription(tr("Set first timestamp to 0."));
		break;
	case QU::roundGap:
		this->setDescription(tr("Round #GAP to nearest integer."));
		break;
	case QU::fixSpaces:
		this->setDescription(tr("Fix spaces at end of line."));
		this->setToolTip(tr("Moves spaces from the end of line to the beginning of the next syllable."));
		break;
	}
}

void QUPreparatoryTask::startOn(QUSongFile *song) {
	switch(_mode) {
	case QU::autoAssignFiles:
		song->autoSetFiles();
		break;
	case QU::removeUnsupportedTags:
		song->removeUnsupportedTags();
		break;
	case QU::fixAudioLength:
		song->fixAudioLength();
		break;
	case QU::fixTimeStamps:
		song->fixTimeStamps();
		break;
	case QU::roundGap:
		song->roundGap();
		break;
	case QU::fixSpaces:
		song->fixSpaces();
		break;
	}
}
