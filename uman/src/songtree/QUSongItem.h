#ifndef QUSONGITEM_H_
#define QUSONGITEM_H_

#include <QTreeWidgetItem>
#include "QU.h"
#include "QUSongFile.h"

#define FIXED_COLUMN_COUNT    23

#define FOLDER_COLUMN          0
#define ARTIST_COLUMN          1
#define TITLE_COLUMN           2
#define MP3_COLUMN             3
#define COVER_COLUMN           4
#define BACKGROUND_COLUMN      5
#define VIDEO_COLUMN           6
#define UNUSED_FILES_COLUMN    7
#define MULTIPLE_SONGS_COLUMN  8
#define LENGTH_COLUMN          9
#define LENGTH_DIFF_COLUMN    10
#define LENGTH_MP3_COLUMN     11
#define LENGTH_EFF_COLUMN     12
#define START_COLUMN          13
#define END_COLUMN            14
#define VIDEOGAP_COLUMN       15
#define ARTIST_COLUMN_EX      16
#define TITLE_COLUMN_EX       17
#define LANGUAGE_COLUMN       18
#define EDITION_COLUMN        19
#define GENRE_COLUMN          20
#define YEAR_COLUMN           21
#define CREATOR_COLUMN        22

#define FIRST_CUSTOM_TAG_COLUMN 20

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
	void updateAsUnknown();

	QUSongFile* song() const { return _song; }
	bool isToplevel() const { return _isToplevel; }

	void showUnusedFilesIcon(QString fileName = "");
	void showMultipleSongsIcon(QString fileName = "");

	virtual bool operator< (const QTreeWidgetItem &other) const;

private:
	QUSongFile *_song;
	bool _isToplevel;

	void clearContents();
	void setTick(int column, bool isBlue = false, QString toolTip = "");
	void setCross(int column, bool isBlue = false, QString toolTip = "");

	void updateSpellCheckColumns();
	void updateFileCheckColumns();
	void updateTimeCheckColumns();
	void updateTextColumns();
};

#endif /*QUSONGITEM_H_*/
