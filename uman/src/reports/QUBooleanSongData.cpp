#include "QUBooleanSongData.h"

QUBooleanSongData::QUBooleanSongData(const QString &tag, QObject *parent): QUAbstractReportData(parent) {
	_tag = tag;
	
	if(QString::compare(_tag, MP3_TAG, Qt::CaseInsensitive) == 0) {
		this->setIcon(QIcon(":/types/music.png"));
		this->setDescription(tr("Audio file exists?"));
	} else if(QString::compare(_tag, COVER_TAG, Qt::CaseInsensitive) == 0) {
		this->setIcon(QIcon(":/types/cover.png"));
		this->setDescription(tr("Cover file exists?"));
	} else if(QString::compare(_tag, BACKGROUND_TAG, Qt::CaseInsensitive) == 0) {
		this->setIcon(QIcon(":/types/background.png"));
		this->setDescription(tr("Background file exists?"));
	} else if(QString::compare(_tag, VIDEO_TAG, Qt::CaseInsensitive) == 0) {
		this->setIcon(QIcon(":/types/film.png"));
		this->setDescription(tr("Video file exists?"));
	}
}

QString QUBooleanSongData::iconData(QUSongFile *song) {
	bool result = false;
	
	if(QString::compare(_tag, MP3_TAG, Qt::CaseInsensitive) == 0)
		result = song->hasMp3();
	else if(QString::compare(_tag, COVER_TAG, Qt::CaseInsensitive) == 0)
		result = song->hasCover();
	else if(QString::compare(_tag, BACKGROUND_TAG, Qt::CaseInsensitive) == 0)
		result = song->hasBackground();
	else if(QString::compare(_tag, VIDEO_TAG, Qt::CaseInsensitive) == 0)
		result = song->hasVideo();

	if(result)
		return ":/marks/tick.png";
	else
		return ":/marks/cross.png";
}

QString QUBooleanSongData::headerTextData() {
	return this->description();
}

QString QUBooleanSongData::headerIconData() {
	if(QString::compare(_tag, MP3_TAG, Qt::CaseInsensitive) == 0)
		return ":/types/music.png";
	else if(QString::compare(_tag, COVER_TAG, Qt::CaseInsensitive) == 0)
		return ":/types/cover.png";
	else if(QString::compare(_tag, BACKGROUND_TAG, Qt::CaseInsensitive) == 0)
		return ":/types/background.png";
	else if(QString::compare(_tag, VIDEO_TAG, Qt::CaseInsensitive) == 0)
		return ":/types/film.png";
	
	return QString();
}

void QUBooleanSongData::sort(QList<QUSongFile*> &songs) {
	if(this->next())
		this->next()->sort(songs);
	
	
}
