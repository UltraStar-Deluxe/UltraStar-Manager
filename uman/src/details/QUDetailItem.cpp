#include "QUDetailItem.h"
#include "QUSongFile.h"
#include "QUMonty.h"

#include "fileref.h"
#include "tag.h"
#include "tstring.h"

#include <QBrush>
#include <QVariant>
#include <QMessageBox>

QUDetailItem::QUDetailItem(const QString &tag):
	QTableWidgetItem(),
	_tag(tag)
{}

QUDetailItem::QUDetailItem(const QString &tag, const QList<QUSongFile*> &songs):
	QTableWidgetItem(),
	_tag(tag)
{
	this->setSongs(songs);
}

void QUDetailItem::setSongs(const QList<QUSongFile*> &songs) {
	_songs = songs;

	if(songs.size() == 1) {
		this->updateText(this->tag(), songs.first());
		this->updateDefaultData();
	} else if(songs.size() > 1) {
		this->updateItemForMultipleSongs();
	} else {
		this->setText("");
	}
}

void QUDetailItem::updateDefaultData() {
	this->setData(Qt::UserRole, defaultData(songs().first()));
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
		this->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
		this->setText(song->mp3());
	} else if(QString::compare(tag, COVER_TAG) == 0) {
		this->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
		this->setText(song->cover());
	} else if(QString::compare(tag, BACKGROUND_TAG) == 0) {
		this->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
		this->setText(song->background());
	} else if(QString::compare(tag, VIDEO_TAG) == 0) {
		this->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
		this->setText(song->video());

	} else if(QString::compare(tag, VIDEOGAP_TAG) == 0) {
		this->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
		if(song->videogap() != N_A)
			this->setText(QString(QObject::tr("%1 seconds")).arg(song->videogap()));
		else
			this->setText(song->videogap());
	} else if(QString::compare(tag, START_TAG) == 0) {
		this->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
		if(song->start() != N_A)
			this->setText(QString(QObject::tr("%1 seconds")).arg(song->start()));
		else
			this->setText(song->start());
	} else if(QString::compare(tag, END_TAG) == 0) {
		this->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
		if(song->end() != N_A)
			this->setText(QString(QObject::tr("%1 milliseconds")).arg(song->end()));
		else
			this->setText(song->end());
	} else if(QString::compare(tag, RELATIVE_TAG) == 0) {
		this->setFlags(0);
		this->setText(song->relative());
	} else if(QString::compare(tag, BPM_TAG) == 0) {
		this->setFlags(0);
		this->setText(song->bpm());
	} else if(QString::compare(tag, GAP_TAG) == 0) {
		this->setFlags(0);
		if(song->gap() != N_A)
			this->setText(QString(QObject::tr("%1 milliseconds")).arg(song->gap()));
		else
			this->setText(song->gap());
	} else if( QUSongFile::customTags().contains(tag, Qt::CaseInsensitive) ) {
		this->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
		this->setText(song->customTag(tag));
	}
}

void QUDetailItem::updateItemForMultipleSongs() {
	if(QString::compare(tag(), TITLE_TAG) == 0) {
		this->setFlags(0);
		this->setText(QObject::tr("Multiple files selected."));
	} else if(QString::compare(tag(), ARTIST_TAG) == 0) {
		this->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);

		this->setText(songs().first()->artist());
		for(int i = 0; i < songs().size(); i++) {
			if(QString::compare(this->text(), songs()[i]->artist(), Qt::CaseInsensitive) != 0) {
				this->setText(QObject::tr("Click here to edit."));
				break;
			}
		}

		this->updateDefaultDataForMultipleSongs();
	} else if(QString::compare(tag(), LANGUAGE_TAG) == 0) {
		this->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);

		this->setText(songs().first()->language());
		for(int i = 0; i < songs().size(); i++) {
			if(QString::compare(this->text(), songs()[i]->language(), Qt::CaseInsensitive) != 0) {
				this->setText(QObject::tr("Click here to edit."));
				break;
			}
		}

		this->updateDefaultData();
	} else if(QString::compare(tag(), EDITION_TAG) == 0) {
		this->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);

		this->setText(songs().first()->edition());
		for(int i = 0; i < songs().size(); i++) {
			if(QString::compare(this->text(), songs()[i]->edition(), Qt::CaseInsensitive) != 0) {
				this->setText(QObject::tr("Click here to edit."));
				break;
			}
		}

		this->updateDefaultData();
	} else if(QString::compare(tag(), GENRE_TAG) == 0) {
		this->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);

		this->setText(songs().first()->genre());
		for(int i = 0; i < songs().size(); i++) {
			if(QString::compare(this->text(), songs()[i]->genre(), Qt::CaseInsensitive) != 0) {
				this->setText(QObject::tr("Click here to edit."));
				break;
			}
		}

		this->updateDefaultData();
	} else if(QString::compare(tag(), YEAR_TAG) == 0) {
		this->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);

		this->setText(songs().first()->year());
		for(int i = 0; i < songs().size(); i++) {
			if(QString::compare(this->text(), songs()[i]->year(), Qt::CaseInsensitive) != 0) {
				this->setText(QObject::tr("Click here to edit."));
				break;
			}
		}

		this->updateDefaultDataForMultipleSongs();
	} else if(QString::compare(tag(), CREATOR_TAG) == 0) {
		this->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);

		this->setText(songs().first()->creator());
		for(int i = 0; i < songs().size(); i++) {
			if(QString::compare(this->text(), songs()[i]->creator(), Qt::CaseInsensitive) != 0) {
				this->setText(QObject::tr("Click here to edit."));
				break;
			}
		}

	} else if(QString::compare(tag(), MP3_TAG) == 0) {
		this->setFlags(0);
		this->setText(QObject::tr("Multiple files selected."));
	} else if(QString::compare(tag(), COVER_TAG) == 0) {
		this->setFlags(0);
		this->setText(QObject::tr("Multiple files selected."));
	} else if(QString::compare(tag(), BACKGROUND_TAG) == 0) {
		this->setFlags(0);
		this->setText(QObject::tr("Multiple files selected."));
	} else if(QString::compare(tag(), VIDEO_TAG) == 0) {
		this->setFlags(0);
		this->setText(QObject::tr("Multiple files selected."));

	} else if(QString::compare(tag(), VIDEOGAP_TAG) == 0) {
		this->setFlags(0);
		this->setText(QObject::tr("Multiple files selected."));
	} else if(QString::compare(tag(), START_TAG) == 0) {
		this->setFlags(0);
		this->setText(QObject::tr("Multiple files selected."));
	} else if(QString::compare(tag(), END_TAG) == 0) {
		this->setFlags(0);
		this->setText(QObject::tr("Multiple files selected."));
	} else if(QString::compare(tag(), RELATIVE_TAG) == 0) {
		this->setFlags(0);
		this->setText(QObject::tr("Multiple files selected."));
	} else if(QString::compare(tag(), BPM_TAG) == 0) {
		this->setFlags(0);
		this->setText(QObject::tr("Multiple files selected."));
	} else if(QString::compare(tag(), GAP_TAG) == 0) {
		this->setFlags(0);
		this->setText(QObject::tr("Multiple files selected."));
	} else if( QUSongFile::customTags().contains(tag(), Qt::CaseInsensitive) ) {
		this->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);

		this->setText( songs().first()->customTag(tag()) );
		for(int i = 0; i < songs().size(); i++) {
			if(QString::compare(this->text(), songs()[i]->customTag(tag()), Qt::CaseInsensitive) != 0) {
				this->setText(QObject::tr("Click here to edit."));
				break;
			}
		}
	}
}

void QUDetailItem::updateDefaultDataForMultipleSongs() {
	if (data(Qt::UserRole).toStringList().size() >= 5)
		return;

	QStringList dropDownData;

	int i = 0;
	foreach(QUSongFile *song, songs()) {
		i++;
		dropDownData << defaultData(song);
		if(i >= 5) {
			dropDownData << "...";
			break;
		}
	}

	this->setData(Qt::UserRole, dropDownData);
}

QStringList QUDetailItem::defaultData(QUSongFile *song) {
	// for performance reasons
	if(!data(Qt::UserRole).toStringList().isEmpty()) {
		if(QString::compare(tag(), GENRE_TAG) == 0) return data(Qt::UserRole).toStringList(); // there is no dynamic content
		if(QString::compare(tag(), LANGUAGE_TAG) == 0) return data(Qt::UserRole).toStringList(); // there is no dynamic content
		if(QString::compare(tag(), EDITION_TAG) == 0) return data(Qt::UserRole).toStringList(); // there is no dynamic content
	}

	// -----------------------------

	QStringList dropDownData;

	if(!song)
		return dropDownData;

	if(QString::compare(tag(), MP3_TAG) == 0)
		dropDownData = song->songFileInfo().dir().entryList(QU::allowedAudioFiles(), QDir::NoDotAndDotDot | QDir::Files, QDir::Name);

	else if(QString::compare(tag(), COVER_TAG) == 0)
		dropDownData = song->songFileInfo().dir().entryList(QU::allowedPictureFiles(), QDir::NoDotAndDotDot | QDir::Files, QDir::Name);

	else if(QString::compare(tag(), BACKGROUND_TAG) == 0)
		dropDownData = song->songFileInfo().dir().entryList(QU::allowedPictureFiles(), QDir::NoDotAndDotDot | QDir::Files, QDir::Name);

	else if(QString::compare(tag(), VIDEO_TAG) == 0)
		dropDownData = song->songFileInfo().dir().entryList(QU::allowedVideoFiles(), QDir::NoDotAndDotDot | QDir::Files, QDir::Name);

	else if(QString::compare(tag(), GENRE_TAG) == 0)
		dropDownData = monty->genres();

	else if(QString::compare(tag(), LANGUAGE_TAG) == 0)
		dropDownData = monty->languages();

	else if(QString::compare(tag(), ARTIST_TAG) == 0) {
		if(song->hasMp3()) {
			TagLib::FileRef ref(song->mp3FileInfo().absoluteFilePath().toLocal8Bit().data());
			QString artist(ref.isNull() ? "" : TStringToQString(ref.tag()->artist()));
			if(!artist.isEmpty())
				dropDownData << artist;
		}
	}
	else if(QString::compare(tag(), TITLE_TAG) == 0) {
		if(song->hasMp3()) {
			TagLib::FileRef ref(song->mp3FileInfo().absoluteFilePath().toLocal8Bit().data());
			QString title(ref.isNull() ? "" : TStringToQString(ref.tag()->title()));
			if(!title.isEmpty())
				dropDownData << title;
		}
	}
	else if(QString::compare(tag(), YEAR_TAG) == 0) {
		if(song->hasMp3()) {
			TagLib::FileRef ref(song->mp3FileInfo().absoluteFilePath().toLocal8Bit().data());
			QString year(ref.isNull()? "0" : QVariant(ref.tag()->year()).toString());
			if(year != "0")
				dropDownData << year;
		}
	}
	else if(QString::compare(tag(), EDITION_TAG) == 0) {
		dropDownData << "[SC]-Songs";
	}

	return dropDownData;
}
