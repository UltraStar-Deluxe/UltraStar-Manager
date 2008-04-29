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
	}	
}
