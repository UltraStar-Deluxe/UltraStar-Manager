#include "QUDetailItem.h"
#include "QUSongFile.h"
#include "QUMonty.h"

#include "fileref.h"
#include "tag.h"
#include "tstring.h"

#include <QBrush>
#include <QVariant>

QUDetailItem::QUDetailItem(const QString &tag, const QList<QUSongFile*> &songs):
	QTableWidgetItem(),
	_tag(tag),
	_songs(songs)
{
	if(songs.size() == 1) {
		this->updateText(tag, songs.first());
		this->updateDefaultData(tag, songs.first());
	} else {
		this->updateItemForMultipleSongs();
	}
}

void QUDetailItem::updateDefaultData(const QString &tag, QUSongFile *song) {
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

void QUDetailItem::updateText(const QString &tag, QUSongFile *song) {
	if(QString::compare(tag, TITLE_TAG) == 0) {
		this->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
		this->setText(song->title());
	} else if(QString::compare(tag, ARTIST_TAG) == 0) {
		this->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
		this->setText(song->artist());
	} else if(QString::compare(tag, LANGUAGE_TAG) == 0) {
		this->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
		this->setText(song->language());
	} else if(QString::compare(tag, EDITION_TAG) == 0) {
		this->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
		this->setText(song->edition());
	} else if(QString::compare(tag, GENRE_TAG) == 0) {
		this->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
		this->setText(song->genre());
	} else if(QString::compare(tag, YEAR_TAG) == 0) {
		this->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
		this->setText(song->year());
	} else if(QString::compare(tag, CREATOR_TAG) == 0) {
		this->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
		this->setText(song->creator());
	
	} else if(QString::compare(tag, MP3_TAG) == 0) {
		this->setText(song->mp3());
	} else if(QString::compare(tag, COVER_TAG) == 0) {
		this->setText(song->cover());
	} else if(QString::compare(tag, BACKGROUND_TAG) == 0) {
		this->setText(song->background());
	} else if(QString::compare(tag, VIDEO_TAG) == 0) {
		this->setText(song->video());
	
	} else if(QString::compare(tag, VIDEOGAP_TAG) == 0) {
		this->setFlags(0);
		this->setText(QString("%1 seconds").arg(song->videogap()));
	} else if(QString::compare(tag, START_TAG) == 0) {
		this->setFlags(0);
		this->setText(QString("%1 seconds").arg(song->start()));
	} else if(QString::compare(tag, END_TAG) == 0) {
		this->setFlags(0);
		this->setText(QString("%1 milliseconds").arg(song->end()));
	} else if(QString::compare(tag, RELATIVE_TAG) == 0) {
		this->setFlags(0);
		this->setText(song->relative());
	} else if(QString::compare(tag, BPM_TAG) == 0) {
		this->setFlags(0);
		this->setText(song->bpm());
	} else if(QString::compare(tag, GAP_TAG) == 0) {
		this->setFlags(0);
		this->setText(QString("%1 milliseconds").arg(song->gap()));
	}
}

void QUDetailItem::updateItemForMultipleSongs() {
	if(QString::compare(tag(), TITLE_TAG) == 0) {
		this->setFlags(0);
		this->setText("Multiple files selected.");
	} else if(QString::compare(tag(), ARTIST_TAG) == 0) {
		this->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
		this->setText("");
	} else if(QString::compare(tag(), LANGUAGE_TAG) == 0) {
		this->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
		this->setText("");
	} else if(QString::compare(tag(), EDITION_TAG) == 0) {
		this->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
		this->setText("");
	} else if(QString::compare(tag(), GENRE_TAG) == 0) {
		this->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
		this->setText("");
	} else if(QString::compare(tag(), YEAR_TAG) == 0) {
		this->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
		this->setText("");
	} else if(QString::compare(tag(), CREATOR_TAG) == 0) {
		this->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
		this->setText("");
	
	} else if(QString::compare(tag(), MP3_TAG) == 0) {
		this->setFlags(0);
		this->setText("Multiple files selected.");
	} else if(QString::compare(tag(), COVER_TAG) == 0) {
		this->setFlags(0);
		this->setText("Multiple files selected.");
	} else if(QString::compare(tag(), BACKGROUND_TAG) == 0) {
		this->setFlags(0);
		this->setText("Multiple files selected.");
	} else if(QString::compare(tag(), VIDEO_TAG) == 0) {
		this->setFlags(0);
		this->setText("Multiple files selected.");
	
	} else if(QString::compare(tag(), VIDEOGAP_TAG) == 0) {
		this->setFlags(0);
		this->setText("Multiple files selected.");
	} else if(QString::compare(tag(), START_TAG) == 0) {
		this->setFlags(0);
		this->setText("Multiple files selected.");
	} else if(QString::compare(tag(), END_TAG) == 0) {
		this->setFlags(0);
		this->setText("Multiple files selected.");
	} else if(QString::compare(tag(), RELATIVE_TAG) == 0) {
		this->setFlags(0);
		this->setText("Multiple files selected.");
	} else if(QString::compare(tag(), BPM_TAG) == 0) {
		this->setFlags(0);
		this->setText("Multiple files selected.");
	} else if(QString::compare(tag(), GAP_TAG) == 0) {
		this->setFlags(0);
		this->setText("Multiple files selected.");
	}
}
