#include "QUDetailItem.h"
#include "QUSongFile.h"
#include "QUMonty.h"

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
	
	if(QString::compare(tag, COVER_TAG) == 0)
		dropDownData = song->songFileInfo().dir().entryList(QUSongFile::allowedPictureFiles(), QDir::NoDotAndDotDot | QDir::Files, QDir::Name);

	if(QString::compare(tag, BACKGROUND_TAG) == 0)
		dropDownData = song->songFileInfo().dir().entryList(QUSongFile::allowedPictureFiles(), QDir::NoDotAndDotDot | QDir::Files, QDir::Name);
	
	if(QString::compare(tag, VIDEO_TAG) == 0)
		dropDownData = song->songFileInfo().dir().entryList(QUSongFile::allowedVideoFiles(), QDir::NoDotAndDotDot | QDir::Files, QDir::Name);
	
	if(QString::compare(tag, GENRE_TAG) == 0)
		dropDownData = monty->genres();
	
	this->setData(Qt::UserRole, QVariant(dropDownData));
}

QUDetailItem::~QUDetailItem() {
	
}

void QUDetailItem::setFontColor(const QColor &color) {
	QBrush brush = foreground();
	brush.setColor(color);
	setForeground(brush);
}
