#ifndef QUDETAILITEM_H_
#define QUDETAILITEM_H_

#include <QString>
#include <QTableWidgetItem>
#include "QUSongFile.h"

/*!
 * This class encapsulates a pointer to a QUSongFile object for a QTableWidget
 * instance which is used to present details of a song file to the user.
 */
class QUDetailItem: public QTableWidgetItem {	
public:
	QUDetailItem(const QString &text, const QString &tag, QUSongFile *song = 0);
	~QUDetailItem();
	
	QString tag() const {return _tag;}
	QUSongFile* song() const {return _song;}
	void setFontColor(const QColor &color);
	
private:
	QString _tag;
	QUSongFile *_song;
	
};

#endif /*QUDETAILITEM_H_*/
