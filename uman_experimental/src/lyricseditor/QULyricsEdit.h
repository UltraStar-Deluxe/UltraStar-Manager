#ifndef QULYRICSEDIT_H
#define QULYRICSEDIT_H

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QKeyEvent>

#include "QUSongFile.h"
#include "QUSongLineDelegate.h"

class QULyricsEdit: public QTableWidget {
	Q_OBJECT

public:
	QULyricsEdit(QWidget *parent = 0);
	void setSong(QUSongFile *song);
	QUSongFile* song() const {return _song;}

public slots:
	void lineModified(QTableWidgetItem *item);
	void setShowWhitespace(bool enabled);
	void skipReadOnlyCells(int row, int col, int, int);

protected:
	virtual void keyPressEvent (QKeyEvent *event);

private:
	QUSongFile *_song;
	QUSongLineDelegate *_delegate;
	bool _showWhitespace;

	void initLyrics();
};

#endif // QULYRICSEDIT_H
