#ifndef QUABSTRACTREPORTDATA_H_
#define QUABSTRACTREPORTDATA_H_

#include <QObject>
#include <QIcon>

#include "QU.h"
#include "QUSongFile.h"

#define ARTIST_COL				0
#define TITLE_COL				1
#define LANGUAGE_COL			2
#define EDITION_COL				3
#define GENRE_COL				4
#define YEAR_COL				5
#define CREATOR_COL				6
#define AUDIO_EXISTS_COL		7
#define COVER_EXISTS_COL		8
#define BACKGROUND_EXISTS_COL	9
#define VIDEO_EXISTS_COL		10
#define MEDLEY_EXISTS_COL		11
#define GOLDEN_NOTES_EXIST_COL	12
#define SONG_PATH_COL			13
#define SONG_FILE_PATH_COL		14
#define REL_SONG_FILE_PATH_COL	15
#define LENGTH_COL				16
#define SPEED_COL				17
#define CUSTOM_TAG_COL			18

class QUAbstractReportData: public QObject {
	Q_OBJECT
	
public:
	QUAbstractReportData(QObject *parent = 0);
	
	virtual QString textData(QUSongFile *song) = 0;
	virtual QString iconData(QUSongFile *song) = 0;
	
	virtual QString headerTextData() = 0;
	virtual QString headerIconData() = 0;
	
	const QIcon& icon() const { return _icon; }
	const QString& description() const { return _description; }
	const QString& toolTip() const { return _toolTip; }
	int id() const { return _id; }
	
	virtual void sort(QList<QUSongFile*> &songs) = 0;
	
	QUAbstractReportData* next() const { return _next; }
	void setNext(QUAbstractReportData *next) { _next = next; }
	
private:
	QIcon   _icon;
	QString _description;
	QString _toolTip;
	int	 _id;
	QUAbstractReportData *_next;

protected:
	void setIcon(const QIcon &icon) { _icon = icon; }
	void setDescription(const QString &description) { _description = description; }
	void setToolTip(const QString &toolTip) { _toolTip = toolTip; }
	void setID(const int id) { _id = id; }
};

#endif /*QUABSTRACTREPORTDATA_H_*/
