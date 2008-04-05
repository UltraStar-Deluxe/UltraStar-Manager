#include "QUDetailItem.h"
#include "QUSongFile.h"
#include "QUMonty.h"

#include "fileref.h"
#include "tag.h"
#include "tstring.h"

#include <QBrush>
#include <QVariant>

QUDetailItem::QUDetailItem(const QString &text, const QString &tag, QUSongFile *song): 
	QTableWidgetItem(text),
	_tag(tag),
	_song(song) 
{
	// prepare data for combo box editor
	QStringList dropDownData;
	
	if(QString::compare(tag, MP3_TAG) == 0)
		dropDownData = song->songFileInfo().dir().entryList(QUSongFile::allowedAudioFiles(), QDir::NoDotAndDotDot | QDir::Files, QDir::Name);
	
	else if(QString::compare(tag, COVER_TAG) == 0)
		dropDownData = song->songFileInfo().dir().entryList(QUSongFile::allowedPictureFiles(), QDir::NoDotAndDotDot | QDir::Files, QDir::Name);

	else if(QString::compare(tag, BACKGROUND_TAG) == 0)
		dropDownData = song->songFileInfo().dir().entryList(QUSongFile::allowedPictureFiles(), QDir::NoDotAndDotDot | QDir::Files, QDir::Name);
	
	else if(QString::compare(tag, VIDEO_TAG) == 0)
		dropDownData = song->songFileInfo().dir().entryList(QUSongFile::allowedVideoFiles(), QDir::NoDotAndDotDot | QDir::Files, QDir::Name);
	
	else if(QString::compare(tag, GENRE_TAG) == 0)
		dropDownData = monty->genres();

	else if(QString::compare(tag, LANGUAGE_TAG) == 0)
		dropDownData = monty->languages();

	else if(QString::compare(tag, ARTIST_TAG) == 0) {
		if(song->hasMp3()) {
			TagLib::FileRef ref(song->mp3FileInfo().absoluteFilePath().toLocal8Bit().data());
			QString artist(TStringToQString(ref.tag()->artist()));
			if(!artist.isEmpty())
				dropDownData << artist;
		}
	}
	else if(QString::compare(tag, TITLE_TAG) == 0) {
		if(song->hasMp3()) {
			TagLib::FileRef ref(song->mp3FileInfo().absoluteFilePath().toLocal8Bit().data());
			QString title(TStringToQString(ref.tag()->title()));
			if(!title.isEmpty())
				dropDownData << title;
		}
	}
	else if(QString::compare(tag, YEAR_TAG) == 0) {
		if(song->hasMp3()) {
			TagLib::FileRef ref(song->mp3FileInfo().absoluteFilePath().toLocal8Bit().data());
			QString year(QVariant(ref.tag()->year()).toString());
			if(year != "0")
				dropDownData << year;
		}
	}
	
	this->setData(Qt::UserRole, QVariant(dropDownData));
}

QUDetailItem::~QUDetailItem() {
	
}

void QUDetailItem::setFontColor(const QColor &color) {
	QBrush brush = foreground();
	brush.setColor(color);
	setForeground(brush);
}
