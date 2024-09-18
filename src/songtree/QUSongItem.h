#ifndef QUSONGITEM_H_
#define QUSONGITEM_H_

#include <QTreeWidgetItem>
#include "QU.h"
#include "QUSongFile.h"

#define FIXED_COLUMN_COUNT		34

#define FOLDER_COLUMN			0

#define ARTIST_COLUMN			1
#define TITLE_COLUMN			2

#define MP3_COLUMN				3
#define COVER_COLUMN			4
#define BACKGROUND_COLUMN		5
#define VIDEO_COLUMN			6

#define TYPE_KARAOKE_COLUMN		7
#define TYPE_DUET_COLUMN		8
#define MEDLEY_COLUMN			9
#define GOLDEN_NOTES_COLUMN		10
#define RAP_NOTES_COLUMN		11
#define UNUSED_FILES_COLUMN		12
#define MULTIPLE_SONGS_COLUMN	13
#define SCORE_COLUMN			14
#define SYNC_COLUMN				15

#define LENGTH_COLUMN			16
#define LENGTH_DIFF_COLUMN		17
#define LENGTH_MP3_COLUMN		18
#define LENGTH_EFF_COLUMN		19
#define SPEED_COLUMN			20

#define START_COLUMN			21
#define END_COLUMN				22
#define VIDEOGAP_COLUMN			23

#define ARTIST_COLUMN_EX		24
#define TITLE_COLUMN_EX			25
#define LANGUAGE_COLUMN			26
#define EDITION_COLUMN			27
#define GENRE_COLUMN			28
#define YEAR_COLUMN				29
#define CREATOR_COLUMN			30
#define TAGS_COLUMN				31

#define RELATIVE_COLUMN			32
#define BPM_COLUMN				33
#define GAP_COLUMN				34

#define DUPLICATE_ID_COLUMN		35

#define FIRST_CUSTOM_TAG_COLUMN	36

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
	void updateAsImage();
	void updateAsVideo();
	void updateAsLicense();
	void updateAsMidi();
	void updateAsKaraoke();
	void updateAsScore();
	void updateAsSync();
	void updateAsUnknown();

	void updateSpellFileCheckColumns();

	void setSong(QUSongFile *song) { _song = song; }
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
	void updateControlColumns();
	void updateBackground();
};

#endif /*QUSONGITEM_H_*/
