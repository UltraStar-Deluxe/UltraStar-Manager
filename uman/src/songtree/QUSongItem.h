#ifndef QUSONGITEM_H_
#define QUSONGITEM_H_

#include <QTreeWidgetItem>
#include "QU.h"
#include "QUSongFile.h"

#define FIXED_COLUMN_COUNT    27

#define FOLDER_COLUMN          0

#define ARTIST_COLUMN          1
#define TITLE_COLUMN           2

#define MP3_COLUMN             3
#define COVER_COLUMN           4
#define BACKGROUND_COLUMN      5
#define VIDEO_COLUMN           6

#define TYPE_KARAOKE_COLUMN    7
#define TYPE_DUET_COLUMN       8
#define UNUSED_FILES_COLUMN    9
#define MULTIPLE_SONGS_COLUMN 10

#define LENGTH_COLUMN         11
#define LENGTH_DIFF_COLUMN    12
#define LENGTH_MP3_COLUMN     13
#define LENGTH_EFF_COLUMN     14
#define SPEED_COLUMN          15

#define START_COLUMN          16
#define END_COLUMN            17
#define VIDEOGAP_COLUMN       18

#define ARTIST_COLUMN_EX      19
#define TITLE_COLUMN_EX       20
#define LANGUAGE_COLUMN       21
#define EDITION_COLUMN        22
#define GENRE_COLUMN          23
#define YEAR_COLUMN           24
#define CREATOR_COLUMN        25

#define DUPLICATE_ID_COLUMN   26

#define FIRST_CUSTOM_TAG_COLUMN 27

/*!
 * This class encapsulates a pointer to a QUSongFile object to be able
 * to retrieve a song by an item in a QTreeWidget.
 */
class QUSongItem: public QTreeWidgetItem {
public:
	QUSongItem(QUSongFile *song = 0, bool isToplevel = false);

	void update();
	void updateAsDirectory(bool showRelativePath = false);
	void updateAsTxt();
	void updateAsMp3();
	void updateAsPicture();
	void updateAsVideo();
	void updateAsLicense();
	void updateAsMidi();
	void updateAsKaraoke();
	void updateAsScore();
	void updateAsUnknown();

	void updateSpellFileCheckColumns();

	QUSongFile* song() const { return _song; }
	bool isToplevel() const { return _isToplevel; }

	void showUnusedFilesIcon(QString fileName = "");
	void showMultipleSongsIcon(QString fileName = "");

	virtual bool operator< (const QTreeWidgetItem &other) const;

	static bool altViewEnabled();
	static void setAltViewEnabled(bool enabled);

private:
	QUSongFile *_song;
	bool _isToplevel;

	void clearContents();
	void setTick(int column);
	void setCross(int column, bool isWarning = false, QString toolTip = "");
	void setSmiley(int column, QU::SpellStates state = QU::spellingOk, QString toolTip = "");

	void updateSpellCheckColumns();
	void updateFileCheckColumns();
	void updateTypeColumns();
	void updateTimeCheckColumns();
	void updateTextColumns();

	void updateBackground();
};

#endif /*QUSONGITEM_H_*/
