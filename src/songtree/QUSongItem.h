#ifndef QUSONGITEM_H_
#define QUSONGITEM_H_

#include <QTreeWidgetItem>
#include "QU.h"
#include "QUSongFile.h"

#define FIXED_COLUMN_COUNT		34

#define FOLDER_COLUMN			0

#define ARTIST_COLUMN			1
#define TITLE_COLUMN			2

#define AUDIO_COLUMN	    	3
#define INSTRUMENTAL_COLUMN     4
#define VOCALS_COLUMN           5
#define COVER_COLUMN			6
#define BACKGROUND_COLUMN		7
#define VIDEO_COLUMN			8
#define REPLAYGAIN_COLUMN       9

#define TYPE_KARAOKE_COLUMN		10
#define TYPE_DUET_COLUMN	    11
#define MEDLEY_COLUMN			12
#define GOLDEN_NOTES_COLUMN		13
#define RAP_NOTES_COLUMN		14
#define UNUSED_FILES_COLUMN		15
#define MULTIPLE_SONGS_COLUMN	16
#define SCORE_COLUMN			17
#define SYNC_COLUMN				18

#define LENGTH_COLUMN			19
#define LENGTH_DIFF_COLUMN		20
#define LENGTH_AUDIO_COLUMN		21
#define LENGTH_EFF_COLUMN		22
#define SPEED_COLUMN			23

#define START_COLUMN			24
#define END_COLUMN				25
#define VIDEOGAP_COLUMN			26

#define ARTIST_COLUMN_EX		27
#define TITLE_COLUMN_EX			28
#define VERSION_COLUMN          29
#define LANGUAGE_COLUMN			30
#define EDITION_COLUMN			31
#define GENRE_COLUMN			32
#define YEAR_COLUMN				33
#define CREATOR_COLUMN			34
#define TAGS_COLUMN				35
#define LINE_ENDING_COLUMN      36
#define AUDIO_TAG_COLUMN        37

#define RELATIVE_COLUMN			38
#define BPM_COLUMN				39
#define GAP_COLUMN				40

#define DUPLICATE_ID_COLUMN		41

#define FIRST_CUSTOM_TAG_COLUMN	42

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
	void updateAsAudio();
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
