#include "QUDetailItem.h"
#include "QUMonty.h"

#include "fileref.h"
#include "tag.h"
#include "tstring.h"

#include <QBrush>
#include <QVariant>
#include <QMessageBox>

#include "QUSongSupport.h"

QUDetailItem::QUDetailItem(const QString &tag):
	QTableWidgetItem(),
	_tag(tag)
{
	reset();
}

QUDetailItem::QUDetailItem(const QString &tag, const QList<QUSongItem*> &songItems):
	QTableWidgetItem(),
	_tag(tag)
{
	reset();
	setSongItems(songItems);
}

void QUDetailItem::setSongItems(const QList<QUSongItem*> &songItems) {
	bool countChanged = _songItems.isEmpty() || (_songItems.size() == 1 && songItems.size() > 1) || (_songItems.size() > 1 && songItems.size() == 1); // speed hack

	_songItems = songItems;

	if(songItems.size() == 1) {
		this->updateItemForSingleSong();
	} else if(songItems.size() > 1) {
		this->updateItemForMultipleSongs(countChanged);
	} else {
		this->setText("");
	}
}

/*!
 * Setup visual appearance for this item according to its tag.
 */
void QUDetailItem::reset() {
	// default values
	_flagsForSingleSong = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
	_flagsForMultipleSongs = Qt::NoItemFlags;
	_textMask = QString("%1");
	_hasDynamicDefaultData = true;

	if(QString::compare(_tag, ENCODING_TAG) == 0) {
		_flagsForSingleSong = Qt::NoItemFlags;
//	} else if(QString::compare(_tag, TITLE_TAG) == 0) {
	} else if(QString::compare(_tag, ARTIST_TAG) == 0) {
		_flagsForMultipleSongs = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
	} else if(QString::compare(_tag, LANGUAGE_TAG) == 0) {
		_flagsForMultipleSongs = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
		setData(Qt::UserRole, QUSongSupport::availableSongLanguages()); // static default data
		_hasDynamicDefaultData = false;
	} else if(QString::compare(_tag, EDITION_TAG) == 0) {
		_flagsForMultipleSongs = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
		setData(Qt::UserRole, QUSongSupport::availableSongEditions()); // static default data
		_hasDynamicDefaultData = false;
	} else if(QString::compare(_tag, GENRE_TAG) == 0) {
		_flagsForMultipleSongs = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
		setData(Qt::UserRole, QUSongSupport::availableSongGenres()); // static default data
		_hasDynamicDefaultData = false;
	} else if(QString::compare(_tag, YEAR_TAG) == 0) {
		_flagsForMultipleSongs = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
		setData(Qt::UserRole, QUSongSupport::availableSongYears()); // static default data
		_hasDynamicDefaultData = false;
	} else if(QString::compare(_tag, CREATOR_TAG) == 0) {
		_flagsForMultipleSongs = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
//	} else if(QString::compare(_tag, MP3_TAG) == 0) {
//	} else if(QString::compare(_tag, COVER_TAG) == 0) {
//	} else if(QString::compare(_tag, BACKGROUND_TAG) == 0) {
//	} else if(QString::compare(_tag, VIDEO_TAG) == 0) {
	} else if(QString::compare(_tag, VIDEOGAP_TAG) == 0) {
		_textMask = QString(QObject::tr("%1 seconds"));
	} else if(QString::compare(_tag, START_TAG) == 0) {
		_textMask = QString(QObject::tr("%1 seconds"));
	} else if(QString::compare(_tag, END_TAG) == 0) {
		_textMask = QString(QObject::tr("%1 milliseconds"));
	} else if(QString::compare(_tag, RELATIVE_TAG) == 0) {
		_flagsForSingleSong = Qt::NoItemFlags;
	} else if(QString::compare(_tag, PREVIEWSTART_TAG) == 0) {
		_textMask = QString(QObject::tr("%1 seconds"));
	} else if (QString::compare(_tag, CALCMEDLEY_TAG) == 0) {
		_flagsForMultipleSongs = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
		setData(Qt::UserRole, QStringList() << "" << "OFF");
		_hasDynamicDefaultData = false;
//	} else if(QString::compare(_tag, MEDLEYSTARTBEAT_TAG) == 0) {
//	} else if(QString::compare(_tag, MEDLEYENDBEAT_TAG) == 0) {
	} else if(QString::compare(_tag, BPM_TAG) == 0) {
		_flagsForSingleSong = Qt::NoItemFlags;
	} else if(QString::compare(_tag, GAP_TAG) == 0) {
		_textMask = QString(QObject::tr("%1 milliseconds"));
	} else if(QString::compare(_tag, P1_TAG) == 0) {
		_flagsForSingleSong = Qt::NoItemFlags;
		_hasDynamicDefaultData = false;
	} else if(QString::compare(_tag, P2_TAG) == 0) {
		_flagsForSingleSong = Qt::NoItemFlags;
		_hasDynamicDefaultData = false;
	} else if(QString::compare(_tag, ALBUM_TAG) == 0) {
		_flagsForMultipleSongs = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
		_hasDynamicDefaultData = false;
	} else if(QString::compare(_tag, COMMENT_TAG) == 0) {
		_flagsForMultipleSongs = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
		_hasDynamicDefaultData = false;
	} else if(QString::compare(_tag, SOURCE_TAG) == 0) {
		_hasDynamicDefaultData = false;
	} else if(QString::compare(_tag, TITLEONSORTING_TAG) == 0) {
		_hasDynamicDefaultData = false;
	} else if(QString::compare(_tag, ARTISTONSORTING_TAG) == 0) {
		_flagsForMultipleSongs = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
		_hasDynamicDefaultData = false;
	} else if( QUSongSupport::availableCustomTags().contains(_tag, Qt::CaseInsensitive) ) {
		_flagsForMultipleSongs = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
		_hasDynamicDefaultData = false;
	}
}

void QUDetailItem::updateItemForSingleSong() {
	if( (QString::compare(_tag, P1_TAG, Qt::CaseInsensitive) == 0) || (QString::compare(_tag, P2_TAG, Qt::CaseInsensitive) == 0) ) {
		if(songItems().first()->song()->isDuet()) {
			_flagsForSingleSong = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
		} else {
			_flagsForSingleSong = Qt::NoItemFlags;
		}
	}
	setFlags(_flagsForSingleSong);

	QString text = songItems().first()->song()->customTag(_tag);
	setText(text == N_A ? N_A : _textMask.arg(text));

	updateDefaultDataForSingleSong();
}

void QUDetailItem::updateItemForMultipleSongs(bool fullUpdate) {
	if(fullUpdate)
		setFlags(_flagsForMultipleSongs);

	if(!_flagsForMultipleSongs.testFlag(Qt::ItemIsEditable)) {
		setText(QObject::tr("Not editable."));
	} else {
		QString text = songItems().first()->song()->customTag(_tag);

		foreach(QUSongItem *item, songItems()) {
			if(QString::compare(text, item->song()->customTag(_tag), Qt::CaseInsensitive) != 0) {
				text = QObject::tr("Click here to edit.");
				break;
			}
		}

		setText(text);

		if(_hasDynamicDefaultData)
			updateDefaultDataForMultipleSongs();
	}
}

void QUDetailItem::updateDefaultDataForSingleSong() {
	if(_hasDynamicDefaultData)
		setData(Qt::UserRole, defaultData(songItems().first()->song()));
}

void QUDetailItem::updateDefaultDataForMultipleSongs() {
	if (data(Qt::UserRole).toStringList().size() >= 5)
		return;

	QStringList dropDownData;

	int i = 0;
	foreach(QUSongItem *songItem, songItems()) {
		i++;
		dropDownData << defaultData(songItem->song());
		if(i >= 5) {
			dropDownData << "...";
			break;
		}
	}

	this->setData(Qt::UserRole, dropDownData);
}

QStringList QUDetailItem::defaultData(QUSongFile *song) {
	QStringList dropDownData;

	if(!song)
		return dropDownData;

	if(QString::compare(tag(), MP3_TAG) == 0)
		dropDownData = song->songFileInfo().dir().entryList(QUSongSupport::allowedAudioFiles(), QDir::NoDotAndDotDot | QDir::Files, QDir::Name);

	else if(QString::compare(tag(), COVER_TAG) == 0)
		dropDownData = song->songFileInfo().dir().entryList(QUSongSupport::allowedImageFiles(), QDir::NoDotAndDotDot | QDir::Files, QDir::Name);

	else if(QString::compare(tag(), BACKGROUND_TAG) == 0)
		dropDownData = song->songFileInfo().dir().entryList(QUSongSupport::allowedImageFiles(), QDir::NoDotAndDotDot | QDir::Files, QDir::Name);

	else if(QString::compare(tag(), VIDEO_TAG) == 0)
		dropDownData = song->songFileInfo().dir().entryList(QUSongSupport::allowedVideoFiles(), QDir::NoDotAndDotDot | QDir::Files, QDir::Name);

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
	else if(QString::compare(tag(), MEDLEYSTARTBEAT_TAG) == 0) {
		if(song->loadMelody().isEmpty() or song->loadMelody().first()->notes().isEmpty()) {
			return dropDownData;
		}

		foreach(QUSongLineInterface *line, song->loadMelody()) {
			auto notes = line->notes();
			if(!notes.isEmpty())
				dropDownData << QString::number(notes.first()->timestamp());
		}
	}
	else if(QString::compare(tag(), MEDLEYENDBEAT_TAG) == 0) {
		if(song->loadMelody().isEmpty() or song->loadMelody().first()->notes().isEmpty()) {
			return dropDownData;
		}

		foreach(QUSongLineInterface *line, song->loadMelody()) {
			auto notes = line->notes();
			if(!notes.isEmpty())
				dropDownData << QString::number(notes.last()->timestamp() + notes.last()->duration());
		}
	}

	return dropDownData;
}
