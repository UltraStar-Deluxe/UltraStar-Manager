#include "QURenameTask.h"

#include <QFileInfo>
#include <QVariant>
#include <QMessageBox>
#include <QRegExp>

QURenameTask::QURenameTask(QU::RenameTaskModes mode, QObject *parent):
	QUAbstractTask(parent),
	_mode(mode)
{
	switch(_mode) {
	case QU::renameDirectory:
		this->setIcon(QIcon(":/types/folder.png"));
		this->setDescription(tr("Rename directory to \"Artist - Title\""));
		break;
	case QU::renameDirectorySpecial:
		this->setIcon(QIcon(":/types/folder.png"));
		this->setDescription(tr("Rename directory to \"Artist - Title [VIDEO] [SC]\" if checked or video present. Keep other folder tags."));
		this->setToolTip(tr("Looks for <b>[SC]</b> in the <b>#EDITION</b> tag to find out whether it is checked or not.<br>"
					"<br>"
					"Other folder tags like <b>[KARAOKE]</b> will be appended to the end if present."));
		break;
	case QU::renameSongFile:
		this->setIcon(QIcon(":/types/text.png"));
		this->setDescription(tr("Rename songtext file to \"Artist - Title.txt\""));
		break;
	case QU::renameAudioFile:
		this->setIcon(QIcon(":/types/music.png"));
		this->setDescription(tr("Rename audio file to \"Artist - Title.*\""));
		break;
	case QU::renameCoverFile:
		this->setIcon(QIcon(":/types/cover.png"));
		this->setDescription(tr("Rename cover to \"Artist - Title [CO].*\""));
		break;
	case QU::renameBackgroundFile:
		this->setIcon(QIcon(":/types/background.png"));
		this->setDescription(tr("Rename background to \"Artist - Title [BG].*\""));
		break;
	case QU::renameVideoFile:
		this->setIcon(QIcon(":/types/film.png"));
		this->setDescription(tr("Rename video to \"Artist - Title.*\""));
		break;
	case QU::renameVideoFileSpecial:
		this->setIcon(QIcon(":/types/film.png"));
		this->setDescription(tr("Rename video to \"Artist - Title [VD#*].*\" consider VIDEOGAP"));
		this->setToolTip(tr("Uses <b>#VIDEOGAP:0</b> if tag not present."));
		break;
	}
}

QURenameTask::QURenameTask(QDomDocument *taskConfig, QObject *parent):
	QUAbstractTask(parent)
{
	// setup visual appearance
	QDomElement general(taskConfig->firstChild().firstChildElement("general"));
	if(!general.isNull()) {
		this->setIcon(QIcon(general.firstChildElement("icon").attribute("resource")));
		this->setDescription(tr(general.firstChildElement("description").firstChild().toCDATASection().data().trimmed().toLocal8Bit().data()));
		this->setToolTip(tr(general.firstChildElement("tooltip").firstChild().toCDATASection().data().trimmed().toLocal8Bit().data()));
	}

	// setup internal operations
	QDomElement rename(taskConfig->firstChild().firstChildElement("rename"));
	if(!rename.isNull()) {
		this->_target = rename.attribute("target");
		this->_schema = rename.attribute("schema");

		for(int i = 0; i < rename.childNodes().size(); i++) {
			QDomElement data = rename.childNodes().at(i).toElement();
			if(!data.isNull() and data.tagName() == "data") {
				QURenameData *newData = new QURenameData();
				newData->_source = data.attribute("source");
				newData->_text = data.attribute("text");
				newData->_if = data.attribute("if");
				newData->_default = data.attribute("default");
				newData->_keepSuffix = QVariant(data.attribute("keepSuffix", "false")).toBool();
				newData->_keepUnknownTags = QVariant(data.attribute("keepUnknownTags", "false")).toBool();
				_data.append(newData);
			}
		}
	}
}

/*!
 * Invokes the task on a song. Data priorities are: if, keepSuffix, text, source, keepUnknownTags
 *              if ... condition for any other attribute in data-element
 *      keepSuffix ... replaces placeholder with current file suffix
 *            text ... replaces placeholder with hard-coded text (usefull with 'if')
 *          source ... dynamically retrieves data from current song object
 * keepUnknownTags ... replaces placeholder with all unknown tags found
 *
 * \sa filterUnknownTags
 */
void QURenameTask::startOn(QUSongFile *song) {
	QString schema = this->_schema; // DO NOT overwrite the template!

	foreach(QURenameData* currentData, _data) {
		if(currentData->_if.isEmpty() or song->property(currentData->_if.toLocal8Bit().data()).toBool()) {
			if(currentData->_keepSuffix) {
				schema = schema.arg(QFileInfo(song->property(this->_target.toLower().toLocal8Bit().data()).toString()).suffix().toLower());
			} else if (!currentData->_text.isEmpty()) {
				schema = schema.arg(currentData->_text);
			} else if (!currentData->_source.isEmpty()) {
				QString value = song->property(currentData->_source.toLower().toLocal8Bit().data()).toString();
				if(value == N_A)
					value = currentData->_default;
				schema = schema.arg(value);
			} else if (currentData->_keepUnknownTags) {
				QStringList unknownTags(this->filterUnknownTags(song->property(this->_target.toLower().toLocal8Bit().data()).toString()));
				if(!unknownTags.isEmpty())
					schema = schema.arg(" " + unknownTags.join(" "));
			} else {
				schema = schema.arg("");
			}
		} else {
			schema = schema.arg("");
		}
	}

	// remove any unused placeholders
	while(schema.contains("%"))
		schema = schema.arg("");

	QMessageBox::information(0, "Uh-Oh!", schema);

	     if (QString::compare(this->_target, "dir", Qt::CaseInsensitive) == 0)        song->renameSongDir(schema);
	else if (QString::compare(this->_target, "txt", Qt::CaseInsensitive) == 0)        song->renameSongTxt(schema);
	else if (QString::compare(this->_target, "mp3", Qt::CaseInsensitive) == 0)        song->renameSongMp3(schema);
	else if (QString::compare(this->_target, "cover", Qt::CaseInsensitive) == 0)      song->renameSongCover(schema);
	else if (QString::compare(this->_target, "background", Qt::CaseInsensitive) == 0) song->renameSongBackground(schema);
	else if (QString::compare(this->_target, "video", Qt::CaseInsensitive) == 0)      song->renameSongVideo(schema);
	// TODO: What's with unknown targets?! Error Message?

//	switch(_mode) {
//	case QU::renameDirectory:
//		song->renameSongDir(song->artist() + " - " + song->title());
//		break;
//	case QU::renameDirectorySpecial:
//		this->renameSongDirSpecial(song);
//		break;
//	case QU::renameSongFile:
//		song->renameSongTxt(song->artist() + " - " + song->title() + ".txt");
//		break;
//	case QU::renameAudioFile:
//		song->renameSongMp3(song->artist() + " - " + song->title() + "." + QFileInfo(song->mp3()).suffix().toLower());
//		break;
//	case QU::renameCoverFile:
//		song->renameSongCover(song->artist() + " - " + song->title() + " [CO]." + QFileInfo(song->cover()).suffix().toLower());
//		break;
//	case QU::renameBackgroundFile:
//		song->renameSongBackground(song->artist() + " - " + song->title() + " [BG]." + QFileInfo(song->background()).suffix().toLower());
//		break;
//	case QU::renameVideoFile:
//		song->renameSongVideo(song->artist() + " - " + song->title() + "." + QFileInfo(song->video()).suffix().toLower());
//		break;
//	case QU::renameVideoFileSpecial:
//		this->renameSongVideoSpecial(song);
//		break;
//	}
}

void QURenameTask::renameSongDirSpecial(QUSongFile *song) {
	QString newName("%1 - %2");

	if(song->hasVideo())
		newName.append(" [VIDEO]");

	if(song->isSongChecked())
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
	QString vgap(song->videogap()); if(vgap == N_A) vgap = "0";

	song->renameSongVideo(newName.arg(song->artist()).arg(song->title()).arg(vgap));
}

/*!
 * Scans the given text for unknown (folder) tags. These tags have nothing in common
 * with the UltraStar tags (MP3, BACKGROUND a.s.o.). Example: "Happy Day [WHO] [VIDEO] [BLESS]"
 * contains the unknown tags "who" and "bless".
 * At the moment, only "sc", "video", "bg", "co" are known as tags.
 */
QStringList QURenameTask::filterUnknownTags(const QString &text) {
	QStringList dirtyTags(text.split("[").filter("]"));
	QStringList cleanTags;

	QStringList knownTags(QString("sc video bg co").split(" "));

	foreach(QString tag, dirtyTags) {
		bool keep = true;
		foreach(QString knownTag, knownTags) {
			keep &= !tag.contains(knownTag, Qt::CaseInsensitive);
		}

		if(keep) {
			tag.replace(QRegExp("\\].*"), "]");
			tag.prepend("[");
			QMessageBox::information(0, "", tag);
			cleanTags << tag.trimmed();
		}
	}

	cleanTags.sort();
	return cleanTags;
}
