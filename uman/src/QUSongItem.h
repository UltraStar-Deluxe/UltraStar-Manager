#ifndef QUSONGITEM_H_
#define QUSONGITEM_H_

#include <QTreeWidgetItem>
#include "QUSongFile.h"

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
	
	void autoSetFiles();
	
	QUSongFile* song() const {return _song;}
	bool isToplevel() const { return _isToplevel; }
	
private:
	QUSongFile *_song;
	bool _isToplevel;

	void clearContents();
};

#endif /*QUSONGITEM_H_*/
