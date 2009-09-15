#include "QUCleanUpTask.h"
#include "QUSmartCheckBox.h"
#include "QUSmartInputCheck.h"
#include "QUSongSupport.h"

QUCleanUpTask::QUCleanUpTask(TaskModes mode, QObject *parent):
	QUSimpleTask(parent),
	_mode(mode)
{
	switch(_mode) {
	case UnusedFiles:
		this->setIcon(QIcon(":/types/file_delete.png"));
		this->setDescription(tr("Delete unused files"));
		this->setToolTip(tr("Every file which is not used by UltraStar will be deleted.<br><br><b>This cannot be undone!</b>"));
		break;
	case InvalidFileTags:
		this->setIcon(QIcon(":/types/tag_delete.png"));
		this->setDescription(tr("Clear invalid file-related tags"));
		this->setToolTip(tr("Removes the value of invalid file-related tags so that they will not be saved into the song text file.<br>"
				"<br>"
				"This includes <b>#VIDEOGAP</b> for invalid video files as well as <b>#START</b> and <b>#END</b> for invalid audio files."));
		break;
	case RemoveEndTag:
		this->setIcon(QIcon(":/types/no_end.png"));
		this->setDescription(tr("Remove #END tag."));
	}
}

void QUCleanUpTask::startOn(QUSongInterface *song) {
	QStringList filter;
	switch(_mode) {
	case UnusedFiles:
		if(smartSettings().at(0)->value().toBool()) filter << QUSongSupport::allowedAudioFiles();
		if(smartSettings().at(1)->value().toBool()) filter << QUSongSupport::allowedPictureFiles();
		if(smartSettings().at(2)->value().toBool()) filter << QUSongSupport::allowedVideoFiles();
		if(smartSettings().at(3)->value().isValid())
			song->deleteUnusedFiles(filter, smartSettings().at(3)->value().toString(), true);
		else
			song->deleteUnusedFiles(filter, "", false);
		break;
	case InvalidFileTags:
		song->clearInvalidFileTags();
		break;
	case RemoveEndTag:
		song->removeEndTag();
		break;
	}
}

QList<QUSmartSettingInterface*> QUCleanUpTask::smartSettings() const {
	if(_smartSettings.isEmpty())
		if(_mode == UnusedFiles) {
			_smartSettings.append(new QUSmartCheckBox("cleanup/unusedFiles_audio", "Audio files", true));
			_smartSettings.append(new QUSmartCheckBox("cleanup/unusedFiles_picture", "Picture files", true));
			_smartSettings.append(new QUSmartCheckBox("cleanup/unusedFiles_video", "Video files", true));
			_smartSettings.append(new QUSmartInputCheck("cleanup/unusedFiles_pattern", "\\.xml|\\.sco", 0, "Pattern:"));
		}
	return _smartSettings;
}
