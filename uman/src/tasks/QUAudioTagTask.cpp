#include "QUAudioTagTask.h"

#include <QVariant>

QUAudioTagTask::QUAudioTagTask(QDomDocument *taskConfig, QObject *parent): QUScriptableTask(taskConfig), _currentSong(0) {
	// setup internal operations
	QDomElement id3(taskConfig->firstChild().firstChildElement("id3"));
	if(!id3.isNull()) {
		this->_target = id3.attribute("target");
		this->_schema = id3.attribute("schema");

		for(int i = 0; i < id3.childNodes().size(); i++) {
			QDomElement data = id3.childNodes().at(i).toElement();
			if(!data.isNull() and data.tagName() == "data") {
				QUScriptData *newData = new QUScriptData();
				newData->_source = data.attribute("source");
				newData->_text = data.attribute("text");
				newData->_if = data.attribute("if");
				newData->_default = data.attribute("default");
				newData->_ignoreEmpty = data.attribute("ignoreEmpty");
				_data.append(newData);
			}
		}
	}
}

void QUAudioTagTask::startOn(QUSongFile *song) {
	_currentSong = song;

	QString schema = this->_schema.trimmed(); // DO NOT overwrite the template!

	foreach(QUScriptData* currentData, _data) {
		if(currentData->_if.isEmpty() or song->property(currentData->_if.toLocal8Bit().data()).toBool()) {
			if (!currentData->_text.isEmpty()) {
				schema = schema.arg(currentData->_text);
			} else if (!currentData->_source.isEmpty()) {
				QString value = this->property(currentData->_source.toLower().toLocal8Bit().data()).toString();

				if(value.isEmpty()) {
					if(!currentData->_ignoreEmpty.isEmpty()) // get old data if new would be empty
						value = this->property(currentData->_ignoreEmpty.toLower().toLocal8Bit().data()).toString();
					else
						value = currentData->_default;
				}

				schema = schema.arg(value);
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

	// a '/' stands for the root with path renaming -> remove it!
	while(schema.startsWith("/"))
		schema.remove(0, 1);

	// remove unsupported characters (windows only)
	schema = QUSongFile::withoutUnsupportedCharacters(schema);

	// you must not use trailing spaces - could corrupt the file system
	schema = schema.trimmed();

	if(QUSongFile::availableInfoTargets().contains(_target, Qt::CaseInsensitive) or QUSongFile::availableCustomTargets().contains(_target, Qt::CaseInsensitive)) {
		song->setInfo(_target, schema);
	}
}

QStringList QUAudioTagTask::availableID3Sources() {
	return QString("artist title genre year target targetfull").split(" ");
}

TagLib::FileRef QUAudioTagTask::ref() {
	if(!_currentSong)
		return TagLib::FileRef(0);

	return TagLib::FileRef(_currentSong->mp3FileInfo().absoluteFilePath().toLocal8Bit().data());
}

QString QUAudioTagTask::artist() {
	if(this->ref().isNull())
		return QString();

	return TStringToQString(ref().tag()->artist());
}

QString QUAudioTagTask::title() {
	if(this->ref().isNull())
		return QString();

	return TStringToQString(ref().tag()->title());
}

QString QUAudioTagTask::genre() {
	if(this->ref().isNull())
		return QString();

	return TStringToQString(ref().tag()->genre());
}

QString QUAudioTagTask::year() {
	if(this->ref().isNull())
		return QString();

	QString year(QVariant(ref().tag()->year()).toString());

	if(year == "0") year = "";

	return year;
}

QString QUAudioTagTask::currentContent() {
	return QU::withoutFolderTags(currentContentAll());
}

QString QUAudioTagTask::currentContentAll() {
	if(!_currentSong)
		return QString();

	QString value;

	if(QUSongFile::availableCustomSources().contains(_target, Qt::CaseInsensitive))
		value = _currentSong->customTag(_target);
	else
		value = _currentSong->property(_target.toLower().toLocal8Bit().data()).toString();

	if(value == N_A)
		return QString();
	else
		return value;
}
