#include "QULyricTask.h"

QULyricTask::QULyricTask(QU::LyricTaskModes mode, QObject *parent):
	QUSimpleTask(parent),
	_mode(mode)
{
	switch(_mode) {
	case QU::fixTimeStamps:
		this->setIcon(QIcon(":/control/zero.png"));
		this->setDescription(tr("Set first timestamp to zero"));
		break;
	case QU::fixSpaces:
		this->setIcon(QIcon(":/control/space.png"));
		this->setDescription(tr("Fix spaces at end of line"));
		this->setToolTip(tr("Moves spaces from the end of line to the beginning of the next syllable."));
		break;
	case QU::removeEmptySyllables:
		this->setIcon(QIcon(":/control/empty_syllable.png"));
		this->setDescription(tr("Remove empty syllables"));
		break;
	}
}

void QULyricTask::startOn(QUSongInterface *song) {
	switch(_mode) {
	case QU::fixTimeStamps:
		song->fixTimeStamps();
		break;
	case QU::fixSpaces:
		song->fixSpaces();
		break;
	case QU::removeEmptySyllables:
		song->removeEmptySyllables();
		break;
	}
}
