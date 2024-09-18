#include "QUSongTagData.h"
#include "QUSongSupport.h"

QUSongTagData::QUSongTagData(const QString &tag, QObject *parent): QUAbstractReportData(parent) {
	_tag = tag;

	if(QString::compare(_tag, ARTIST_TAG, Qt::CaseInsensitive) == 0) {
		this->setIcon(QIcon(":/types/user.png"));
		this->setDescription(tr("Artist"));
		this->setID(ARTIST_COL);
	} else if(QString::compare(_tag, TITLE_TAG, Qt::CaseInsensitive) == 0) {
		this->setIcon(QIcon(":/types/font.png"));
		this->setDescription(tr("Title"));
		this->setID(TITLE_COL);
	} else if(QString::compare(_tag, LANGUAGE_TAG, Qt::CaseInsensitive) == 0) {
		this->setIcon(QIcon(":/types/language.png"));
		this->setDescription(tr("Language"));
		this->setID(LANGUAGE_COL);
	} else if(QString::compare(_tag, EDITION_TAG, Qt::CaseInsensitive) == 0) {
		this->setIcon(QIcon(":/types/edition.png"));
		this->setDescription(tr("Edition"));
		this->setID(EDITION_COL);
	} else if(QString::compare(_tag, GENRE_TAG, Qt::CaseInsensitive) == 0) {
		this->setIcon(QIcon(":/types/genre.png"));
		this->setDescription(tr("Genre"));
		this->setID(GENRE_COL);
	} else if(QString::compare(_tag, YEAR_TAG, Qt::CaseInsensitive) == 0) {
		this->setIcon(QIcon(":/types/date.png"));
		this->setDescription(tr("Year"));
		this->setID(YEAR_COL);
	} else if(QString::compare(_tag, CREATOR_TAG, Qt::CaseInsensitive) == 0) {
		this->setIcon(QIcon(":/types/creator.png"));
		this->setDescription(tr("Creator"));
		this->setID(CREATOR_COL);
	} else if(QString::compare(_tag, TAGS_TAG, Qt::CaseInsensitive) == 0) {
		this->setIcon(QIcon(":/types/tags.png"));
		this->setDescription(tr("Tags"));
		this->setID(TAGS_COL);
	} else if(QUSongSupport::availableCustomTags().contains(tag, Qt::CaseInsensitive)) {
		this->setIcon(QIcon(":/bullets/bullet_star.png"));
		this->setDescription(tag);
		this->setID(CUSTOM_TAG_COL);
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
	if(QString::compare(_tag, TAGS_TAG, Qt::CaseInsensitive) == 0)
		return song->tags();
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
		std::stable_sort(songs.begin(), songs.end(), QUSongFile::artistLessThan);
	else if(QString::compare(_tag, TITLE_TAG, Qt::CaseInsensitive) == 0)
		std::stable_sort(songs.begin(), songs.end(), QUSongFile::titleLessThan);
	else if(QString::compare(_tag, LANGUAGE_TAG, Qt::CaseInsensitive) == 0)
		std::stable_sort(songs.begin(), songs.end(), QUSongFile::languageLessThan);
	else if(QString::compare(_tag, EDITION_TAG, Qt::CaseInsensitive) == 0)
		std::stable_sort(songs.begin(), songs.end(), QUSongFile::editionLessThan);
	else if(QString::compare(_tag, GENRE_TAG, Qt::CaseInsensitive) == 0)
		std::stable_sort(songs.begin(), songs.end(), QUSongFile::genreLessThan);
	else if(QString::compare(_tag, YEAR_TAG, Qt::CaseInsensitive) == 0)
		std::stable_sort(songs.begin(), songs.end(), QUSongFile::yearLessThan);
	else if(QString::compare(_tag, CREATOR_TAG, Qt::CaseInsensitive) == 0)
		std::stable_sort(songs.begin(), songs.end(), QUSongFile::creatorLessThan);
	else if(QString::compare(_tag, TAGS_TAG, Qt::CaseInsensitive) == 0)
		std::stable_sort(songs.begin(), songs.end(), QUSongFile::tagsLessThan);

	// TODO: sort custom tag columns
}
