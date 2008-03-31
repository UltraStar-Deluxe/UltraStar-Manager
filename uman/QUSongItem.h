#ifndef QUSONGITEM_H_
#define QUSONGITEM_H_

#include <QString>
#include <QTreeWidgetItem>
#include "QUSongFile.h"

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
