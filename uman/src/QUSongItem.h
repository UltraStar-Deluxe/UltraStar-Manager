#ifndef QUSONGITEM_H_
#define QUSONGITEM_H_

#include <QString>
#include <QTreeWidgetItem>
#include "QUSongFile.h"

/*!
 * This class encapsulates a pointer to a QUSongFile object to be able
 * to retrieve a song by an item in a QTreeWidget.
 */
class QUSongItem: public QTreeWidgetItem {	
public:
	QUSongItem(QUSongFile *song = 0);
	~QUSongItem();
	
	QUSongFile* song() const {return _song;}
	void setFontColor(const QColor &color);
	
private:
	QUSongFile *_song;
};

#endif /*QUSONGITEM_H_*/
