#include "QUCleanTask.h"

QUCleanTask::QUCleanTask(QU::CleanTaskModes mode, QObject *parent):
	QUAbstractTask(parent),
	_mode(mode)
{
	switch(_mode) {
	case QU::unusedFiles:
		this->setIcon(QIcon(":/types/file_delete.png"));
		this->setDescription(tr("Delete unused files"));
		this->setToolTip(tr("Every file which is not used by UltraStar will be deleted.<br><br><b>This cannot be undone!</b>"));
		break;
	case QU::invalidFileTags:
		this->setIcon(QIcon(":/types/tag_delete.png"));
		this->setDescription(tr("Clear invalid file-related tags"));
		this->setToolTip(tr("Removes the value of invalid file-related tags so that they will not be saved into the song text file.<br>"
				"<br>"
				"This includes <b>#VIDEOGAP</b> for invalid video files as well as <b>#START</b> and <b>#END</b> for invalid audio files."));
		break;
	case QU::removeEndTag:
		this->setIcon(QIcon(":/types/no_end.png"));
		this->setDescription(tr("Remove #END tag."));
	}
}

void QUCleanTask::startOn(QUSongFile *song) {
	switch(_mode) {
	case QU::unusedFiles:
		song->deleteUnusedFiles();
		break;
	case QU::invalidFileTags:
		song->clearInvalidFileTags();
		break;
	case QU::removeEndTag:
		song->removeEndTag();
		break;
	}
}
