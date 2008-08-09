#ifndef QUPLAYLISTITEM_H_
#define QUPLAYLISTITEM_H_

#include "QU.h"
#include "QUSongFile.h"

#include <QListWidget>
#include <QListWidgetItem>

class QUPlayListItem: public QListWidgetItem {
public:
	QUPlayListItem(QUSongFile *song, QListWidget *parent = 0);

	void connectSong(QUSongFile *song) { _song = song; }
	void disconnectSong() { _song = 0; }
	bool hasSongConnected() const { return (_song == 0); }

	void updateData();

private:
	QUSongFile *_song;
	QString _textBuffer; // used if _song becomes invalid
};

#endif /* QUPLAYLISTITEM_H_ */
