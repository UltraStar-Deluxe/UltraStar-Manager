#include "QUBooleanSongData.h"

#include <QSettings>

QUBooleanSongData::QUBooleanSongData(const QString &tag, QObject *parent): QUAbstractReportData(parent) {
	_tag = tag;

	if(QString::compare(_tag, MP3_TAG, Qt::CaseInsensitive) == 0) {
		this->setIcon(QIcon(":/types/music.png"));
		this->setDescription(tr("Audio file exists?"));
		this->setID(AUDIO_EXISTS_COL);
	} else if(QString::compare(_tag, COVER_TAG, Qt::CaseInsensitive) == 0) {
		this->setIcon(QIcon(":/types/cover.png"));
		this->setDescription(tr("Cover file exists?"));
		this->setID(COVER_EXISTS_COL);
	} else if(QString::compare(_tag, BACKGROUND_TAG, Qt::CaseInsensitive) == 0) {
		this->setIcon(QIcon(":/types/background.png"));
		this->setDescription(tr("Background file exists?"));
		this->setID(BACKGROUND_EXISTS_COL);
	} else if(QString::compare(_tag, VIDEO_TAG, Qt::CaseInsensitive) == 0) {
		this->setIcon(QIcon(":/types/video.png"));
		this->setDescription(tr("Video file exists?"));
		this->setID(VIDEO_EXISTS_COL);
	} else if(QString::compare(_tag, MEDLEY_TAGS, Qt::CaseInsensitive) == 0) {
		this->setIcon(QIcon(":/types/medley.png"));
		this->setDescription(tr("Medley exists?"));
		this->setID(MEDLEY_EXISTS_COL);
	} else if(QString::compare(_tag, GOLDEN_NOTES, Qt::CaseInsensitive) == 0) {
		this->setIcon(QIcon(":/types/golden_notes.png"));
		this->setDescription(tr("Golden Notes exist?"));
		this->setID(GOLDEN_NOTES_EXIST_COL);
	} else if(QString::compare(_tag, RAP_NOTES, Qt::CaseInsensitive) == 0) {
		this->setIcon(QIcon(":/types/rap_notes.png"));
		this->setDescription(tr("Rap Notes exist?"));
		this->setID(RAP_NOTES_EXIST_COL);
	}
}

QString QUBooleanSongData::textData(QUSongFile *song) {
	bool result = false;

	if(QString::compare(_tag, MP3_TAG, Qt::CaseInsensitive) == 0)
		result = song->hasMp3();
	else if(QString::compare(_tag, COVER_TAG, Qt::CaseInsensitive) == 0)
		result = song->hasCover();
	else if(QString::compare(_tag, BACKGROUND_TAG, Qt::CaseInsensitive) == 0)
		result = song->hasBackground();
	else if(QString::compare(_tag, VIDEO_TAG, Qt::CaseInsensitive) == 0)
		result = song->hasVideo();
	else if(QString::compare(_tag, MEDLEY_TAGS, Qt::CaseInsensitive) == 0)
		result = song->hasMedley();
	else if(QString::compare(_tag, GOLDEN_NOTES, Qt::CaseInsensitive) == 0)
		result = song->hasGoldenNotes();
	else if(QString::compare(_tag, RAP_NOTES, Qt::CaseInsensitive) == 0)
		result = song->hasRapNotes();

	if(result)
		return tr("yes");
	else
		return tr("no");
}

QString QUBooleanSongData::iconData(QUSongFile *song) {
	QSettings settings;
	bool useAltIcons = settings.value("altSongTree", false).toBool();

	if(QString::compare(_tag, MP3_TAG, Qt::CaseInsensitive) == 0) {
		if (song->hasMp3())
			return useAltIcons ? ":/types/music.png" : ":/marks/tick.png";
		else
			return useAltIcons ? "" : ":/marks/cross.png";
	} else if(QString::compare(_tag, COVER_TAG, Qt::CaseInsensitive) == 0) {
		if (song->hasCover())
			return useAltIcons ? ":/types/cover.png" : ":/marks/tick.png";
		else
			return useAltIcons ? "" : ":/marks/cross.png";
	} else if(QString::compare(_tag, BACKGROUND_TAG, Qt::CaseInsensitive) == 0) {
		if (song->hasBackground())
			return useAltIcons ? ":/types/background.png" : ":/marks/tick.png";
		else
			return useAltIcons ? "" : ":/marks/cross.png";
	} else if(QString::compare(_tag, VIDEO_TAG, Qt::CaseInsensitive) == 0) {
		if (song->hasVideo())
			return useAltIcons ? ":/types/video.png" : ":/marks/tick.png";
		else
			return useAltIcons ? "" : ":/marks/cross.png";
	} else if(QString::compare(_tag, MEDLEY_TAGS, Qt::CaseInsensitive) == 0) {
		if (song->hasMedley())
			return useAltIcons ? ":/types/medley.png" : ":/marks/tick.png";
		else
			return useAltIcons ? "" : ":/marks/cross.png";
	} else if(QString::compare(_tag, GOLDEN_NOTES, Qt::CaseInsensitive) == 0) {
		if (song->hasGoldenNotes())
			return useAltIcons ? ":/types/golden_notes.png" : ":/marks/tick.png";
		else
			return useAltIcons ? "" : ":/marks/cross.png";
	} else if(QString::compare(_tag, RAP_NOTES, Qt::CaseInsensitive) == 0) {
		if (song->hasRapNotes())
			return useAltIcons ? ":/types/rap_notes.png" : ":/marks/tick.png";
		else
			return useAltIcons ? "" : ":/marks/cross.png";
	}

	return "";
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
		return ":/types/video.png";
	else if(QString::compare(_tag, MEDLEY_TAGS, Qt::CaseInsensitive) == 0)
		return ":/types/medley.png";
	else if(QString::compare(_tag, GOLDEN_NOTES, Qt::CaseInsensitive) == 0)
		return ":/types/golden_notes.png";
	else if(QString::compare(_tag, RAP_NOTES, Qt::CaseInsensitive) == 0)
		return ":/types/rap_notes.png";

	return QString();
}

void QUBooleanSongData::sort(QList<QUSongFile*> &songs) {
	if(this->next())
		this->next()->sort(songs);

	if(QString::compare(_tag, MP3_TAG, Qt::CaseInsensitive) == 0)
		qStableSort(songs.begin(), songs.end(), QUSongFile::hasMp3LessThan);
	else if(QString::compare(_tag, COVER_TAG, Qt::CaseInsensitive) == 0)
		qStableSort(songs.begin(), songs.end(), QUSongFile::hasCoverLessThan);
	else if(QString::compare(_tag, BACKGROUND_TAG, Qt::CaseInsensitive) == 0)
		qStableSort(songs.begin(), songs.end(), QUSongFile::hasBackgroundLessThan);
	else if(QString::compare(_tag, VIDEO_TAG, Qt::CaseInsensitive) == 0)
		qStableSort(songs.begin(), songs.end(), QUSongFile::hasVideoLessThan);
	else if(QString::compare(_tag, MEDLEY_TAGS, Qt::CaseInsensitive) == 0)
		qStableSort(songs.begin(), songs.end(), QUSongFile::hasMedleyLessThan);
	else if(QString::compare(_tag, GOLDEN_NOTES, Qt::CaseInsensitive) == 0)
		qStableSort(songs.begin(), songs.end(), QUSongFile::hasGoldenNotesLessThan);
	else if(QString::compare(_tag, RAP_NOTES, Qt::CaseInsensitive) == 0)
		qStableSort(songs.begin(), songs.end(), QUSongFile::hasRapNotesLessThan);
}
