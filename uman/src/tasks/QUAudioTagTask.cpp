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

	schema = schema.trimmed();

	if(availableTargets().contains(_target, Qt::CaseInsensitive)) {
		QString old = this->currentContentAll();
		song->setInfo(_target, schema);
		emit finished(QString(tr("The song tag \"%1\" of \"%4 - %5\" was changed from \"%2\" to \"%3\"."))
				.arg(_target)
				.arg(old)
				.arg(this->currentContentAll())
				.arg(song->artist())
				.arg(song->title()), QU::information);
	} else {
		emit finished(QString(tr("The song tag \"%1\" is NOT VALID.")).arg(_target), QU::warning);
	}
}

QStringList QUAudioTagTask::availableSources() {
	return availableCommonSources() + availableSpecialSources();
}

QStringList QUAudioTagTask::availableSpecialSources() {
	return QString("target targetfull").split(" ");
}

QStringList QUAudioTagTask::availableCommonSources() {
	return QString("artist title album genre year track length bitrate").split(" ");
}

QStringList QUAudioTagTask::availableTargets() {
	return availableInfoTargets() + availableCustomTargets();
}

/*!
 * \returns a list of all possible targets used by audiotag tasks.
 */
QStringList QUAudioTagTask::availableInfoTargets() {
	return QString("artist title language edition genre year creator").split(" ");
}

/*!
 * \returns a list of all possible custom targets used by audiotag tasks.
 */
QStringList QUAudioTagTask::availableCustomTargets() {
	return QUSongFile::customTags();
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

QString QUAudioTagTask::album() {
	if(this->ref().isNull())
		return QString();

	return TStringToQString(ref().tag()->album());
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

QString QUAudioTagTask::track() {
	if(this->ref().isNull())
		return QString();

	QString track(QVariant(ref().tag()->track()).toString());

	if(track == "0") track = "";

	return track;
}

QString QUAudioTagTask::length() {
	TagLib::FileRef ref = this->ref();

	if(ref.isNull() or !ref.audioProperties())
		return QString();

	int length = ref.audioProperties()->length();

	return QString("%1:%2").arg(length / 60).arg(length % 60, 2, 10, QChar('0'));
}

QString QUAudioTagTask::bitrate() {
	TagLib::FileRef ref = this->ref();

	if(ref.isNull() or !ref.audioProperties())
		return QString();

	int bitrate = ref.audioProperties()->bitrate();

	return QVariant(bitrate).toString();
}

QString QUAudioTagTask::currentContent() {
	return QU::withoutFolderTags(currentContentAll());
}

QString QUAudioTagTask::currentContentAll() {
	if(!_currentSong)
		return QString();

	QString value;

	if(availableCustomSources().contains(_target, Qt::CaseInsensitive))
		value = _currentSong->customTag(_target);
	else
		value = _currentSong->property(_target.toLower().toLocal8Bit().data()).toString();

	if(value == N_A)
		return QString();
	else
		return value;
}
