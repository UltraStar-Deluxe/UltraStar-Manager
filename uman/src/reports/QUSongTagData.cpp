#include "QUSongTagData.h"

QUSongTagData::QUSongTagData(const QString &tag, QObject *parent): QUAbstractReportData(parent) {
	_tag = tag;
	this->setType(QU::text);
	
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
	}
}

QString QUSongTagData::data(QUSongFile *song) {
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
	else
		return QString();
}

QString QUSongTagData::headerData() {
	return this->description();
}
