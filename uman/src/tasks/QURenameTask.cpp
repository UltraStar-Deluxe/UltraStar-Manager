#include "QURenameTask.h"

#include <QFileInfo>
#include <QVariant>
#include <QMessageBox>
#include <QRegExp>

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
	QString schema = this->_schema.trimmed(); // DO NOT overwrite the template!

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

	     if (QString::compare(this->_target, "dir", Qt::CaseInsensitive) == 0)        song->renameSongDir(schema);
	else if (QString::compare(this->_target, "txt", Qt::CaseInsensitive) == 0)        song->renameSongTxt(schema);
	else if (QString::compare(this->_target, "mp3", Qt::CaseInsensitive) == 0)        song->renameSongMp3(schema);
	else if (QString::compare(this->_target, "cover", Qt::CaseInsensitive) == 0)      song->renameSongCover(schema);
	else if (QString::compare(this->_target, "background", Qt::CaseInsensitive) == 0) song->renameSongBackground(schema);
	else if (QString::compare(this->_target, "video", Qt::CaseInsensitive) == 0)      song->renameSongVideo(schema);
	// TODO: What's with unknown targets?! Error Message?

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
			cleanTags << tag.trimmed();
		}
	}

	cleanTags.sort();
	return cleanTags;
}
