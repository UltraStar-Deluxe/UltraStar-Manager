#include "QUSongTagData.h"
#include "QUSongSupport.h"

QUSongTagData::QUSongTagData(const QString &tag, QObject *parent): QUAbstractReportData(parent) {
	_tag = tag;

	if(QString::compare(_tag, ARTIST_TAG, Qt::CaseInsensitive) == 0) {
		this->setIcon(QIcon(":/types/user.png"));
		this->setDescription(tr("Artist"));
	} else if(QString::compare(_tag, TITLE_TAG, Qt::CaseInsensitive) == 0) {
		this->setIcon(QIcon(":/types/font.png"));
		this->setDescription(tr("Title"));
	} else if(QString::compare(_tag, LANGUAGE_TAG, Qt::CaseInsensitive) == 0) {
		this->setIcon(QIcon(":/types/language.png"));
		this->setDescription(tr("Language"));
	} else if(QString::compare(_tag, EDITION_TAG, Qt::CaseInsensitive) == 0) {
		this->setIcon(QIcon(":/types/edition.png"));
		this->setDescription(tr("Edition"));
	} else if(QString::compare(_tag, GENRE_TAG, Qt::CaseInsensitive) == 0) {
		this->setIcon(QIcon(":/types/genre.png"));
		this->setDescription(tr("Genre"));
	} else if(QString::compare(_tag, YEAR_TAG, Qt::CaseInsensitive) == 0) {
		this->setIcon(QIcon(":/types/date.png"));
		this->setDescription(tr("Year"));
	} else if(QString::compare(_tag, CREATOR_TAG, Qt::CaseInsensitive) == 0) {
		this->setIcon(QIcon(":/types/creator.png"));
		this->setDescription(tr("Creator"));
	} else if(QUSongSupport::availableCustomTags().contains(tag, Qt::CaseInsensitive)) {
		this->setIcon(QIcon(":/bullets/bullet_star.png"));
		this->setDescription(tag);
	}
}

QString QUSongTagData::textData(QUSongFile *song) {
	if(QString::compare(_tag, ARTIST_TAG, Qt::CaseInsensitive) == 0)
		return song->artist();
	if(QString::compare(_tag, TITLE_TAG, Qt::CaseInsensitive) == 0)
		return song->title();
	if(QString::compare(_tag, LANGUAGE_TAG, Qt::CaseInsensitive) == 0)
		return song->language();
	if(QString::compare(_tag, EDITION_TAG, Qt::CaseInsensitive) == 0)
		return song->edition();
	if(QString::compare(_tag, GENRE_TAG, Qt::CaseInsensitive) == 0)
		return song->genre();
	if(QString::compare(_tag, YEAR_TAG, Qt::CaseInsensitive) == 0)
		return song->year();
	if(QString::compare(_tag, CREATOR_TAG, Qt::CaseInsensitive) == 0)
		return song->creator();
	if(QUSongSupport::availableCustomTags().contains(_tag, Qt::CaseInsensitive))
		return song->customTag(_tag);

	return QString();
}

QString QUSongTagData::headerTextData() {
	return this->description();
}

void QUSongTagData::sort(QList<QUSongFile*> &songs) {
	if(this->next())
		this->next()->sort(songs);

	if(QString::compare(_tag, ARTIST_TAG, Qt::CaseInsensitive) == 0)
		qStableSort(songs.begin(), songs.end(), QUSongFile::artistLessThan);
	else if(QString::compare(_tag, TITLE_TAG, Qt::CaseInsensitive) == 0)
		qStableSort(songs.begin(), songs.end(), QUSongFile::titleLessThan);
	else if(QString::compare(_tag, LANGUAGE_TAG, Qt::CaseInsensitive) == 0)
		qStableSort(songs.begin(), songs.end(), QUSongFile::languageLessThan);
	else if(QString::compare(_tag, EDITION_TAG, Qt::CaseInsensitive) == 0)
		qStableSort(songs.begin(), songs.end(), QUSongFile::editionLessThan);
	else if(QString::compare(_tag, GENRE_TAG, Qt::CaseInsensitive) == 0)
		qStableSort(songs.begin(), songs.end(), QUSongFile::genreLessThan);
	else if(QString::compare(_tag, YEAR_TAG, Qt::CaseInsensitive) == 0)
		qStableSort(songs.begin(), songs.end(), QUSongFile::yearLessThan);
	else if(QString::compare(_tag, CREATOR_TAG, Qt::CaseInsensitive) == 0)
		qStableSort(songs.begin(), songs.end(), QUSongFile::creatorLessThan);

	// TODO: sort custom tag columns
}
