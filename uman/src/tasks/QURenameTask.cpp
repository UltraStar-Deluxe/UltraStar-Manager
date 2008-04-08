#include "QURenameTask.h"

#include <QFileInfo>

QURenameTask::QURenameTask(QU::RenameTaskModes mode, QObject *parent): 
	QUAbstractTask(parent),
	_mode(mode)
{
	switch(_mode) {
	case QU::renameDirectory:
		this->setIcon(QIcon(":/types/folder.png"));
		this->setDescription("Rename directory to \"Artist - Title\"");
		break;
	case QU::renameDirectorySpecial:
		this->setIcon(QIcon(":/types/folder.png"));
		this->setDescription("Rename directory to \"Artist - Title [VIDEO] [SC]\" if checked or video present. Keep other folder tags.");
		this->setToolTip("Looks for <b>[SC]</b> in the <b>#EDITION</b> tag to find out whether it is checked or not.<br>"
					"<br>"
					"Other folder tags like <b>[KARAOKE]</b> will be appended to the end if present.");
		break;
	case QU::renameSongFile:
		this->setIcon(QIcon(":/types/text.png"));
		this->setDescription("Rename songtext file to \"Artist - Title.txt\"");
		break;
	case QU::renameAudioFile:
		this->setIcon(QIcon(":/types/music.png"));
		this->setDescription("Rename audio file to \"Artist - Title.*\"");
		break;
	case QU::renameCoverFile:
		this->setIcon(QIcon(":/types/picture.png"));
		this->setDescription("Rename cover to \"Artist - Title [CO].*\"");
		break;
	case QU::renameBackgroundFile:
		this->setIcon(QIcon(":/types/picture.png"));
		this->setDescription("Rename background to \"Artist - Title [BG].*\"");
		break;
	case QU::renameVideoFile:
		this->setIcon(QIcon(":/types/film.png"));
		this->setDescription("Rename video to \"Artist - Title.*\"");
		break;
	case QU::renameVideoFileSpecial:
		this->setIcon(QIcon(":/types/film.png"));
		this->setDescription("Rename video to \"Artist - Title [VD#*].*\" consider VIDEOGAP");
		this->setToolTip("Uses <b>#VIDEOGAP:0</b> if tag not present.");
		break;
	}
}

void QURenameTask::startOn(QUSongFile *song) {
	switch(_mode) {
	case QU::renameDirectory:
		song->renameSongDir(song->artist() + " - " + song->title());
		break;
	case QU::renameDirectorySpecial:
		this->renameSongDirSpecial(song);
		break;
	case QU::renameSongFile:
		song->renameSongTxt(song->artist() + " - " + song->title() + ".txt");
		break;
	case QU::renameAudioFile:
		song->renameSongMp3(song->artist() + " - " + song->title() + "." + QFileInfo(song->mp3()).suffix().toLower());
		break;
	case QU::renameCoverFile:
		song->renameSongCover(song->artist() + " - " + song->title() + " [CO]." + QFileInfo(song->cover()).suffix().toLower());
		break;
	case QU::renameBackgroundFile:
		song->renameSongBackground(song->artist() + " - " + song->title() + " [BG]." + QFileInfo(song->background()).suffix().toLower());
		break;
	case QU::renameVideoFile:
		song->renameSongVideo(song->artist() + " - " + song->title() + "." + QFileInfo(song->video()).suffix().toLower());
		break;
	case QU::renameVideoFileSpecial:
		this->renameSongVideoSpecial(song);
		break;
	}
}

void QURenameTask::renameSongDirSpecial(QUSongFile *song) {
	QString newName("%1 - %2");
	
	if(song->hasVideo())
		newName.append(" [VIDEO]");
	
	if(song->edition().contains("[SC]", Qt::CaseInsensitive))
		newName.append(" [SC]");
	
	// extract additional folder tags from old name -> "[...]"
	QStringList folderTags(song->songFileInfo().dir().dirName().split("[").filter("]"));
	folderTags.sort();
	foreach(QString folderTag, folderTags) {
		if(!folderTag.contains("sc", Qt::CaseInsensitive) and !folderTag.contains("video", Qt::CaseInsensitive))
			newName.append(" [" + folderTag.trimmed());
	}
	
	// do the renaming...
	song->renameSongDir(newName.arg(song->artist()).arg(song->title()));
}

void QURenameTask::renameSongVideoSpecial(QUSongFile *song) {
	QString newName("%1 - %2 [VD#%3]." + QFileInfo(song->video()).suffix().toLower());
	
	// set videogap 0 for the videoname
	QString vgap(song->videogap()); if(vgap == "n/a") vgap = "0";
	
	song->renameSongVideo(newName.arg(song->artist()).arg(song->title()).arg(vgap));
}
