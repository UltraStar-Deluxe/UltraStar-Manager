#ifndef QUPREVIEWTREE_H_
#define QUPREVIEWTREE_H_

#define SONG_COUNT_CHILD_INDEX           0
#define HIDDEN_SONG_COUNT_CHILD_INDEX    1
#define VISIBLE_SONG_COUNT_CHILD_INDEX   2
#define SELECTED_SONG_COUNT_CHILD_INDEX  3

#define SELECTED_SONG_LENGTH_INDEX  0
#define SELECTED_AUDIO_LENGTH_INDEX 1
#define SELECTED_TOTAL_LENGTH_INDEX 2

#include "QU.h"

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QFileInfo>

class QUPreviewTree: public QTreeWidget {
	Q_OBJECT

public:
	QUPreviewTree(QWidget *parent = 0);

public slots:
	void setSongCount(int songCount, int friendCount);
	void setSelectedSongCount(int songCount, int friendCount);
	void setHiddenSongCount(int songCount, int friendCount);
	void setVisibleSongCount(int songCount, int friendCount);

	void setSelectedSongLength(int seconds = -1);
	void setSelectedAudioLength(int seconds = -1);
	void setSelectedTotalLength(int seconds = -1);

	void showFileInformation(const QFileInfo &fi);

private:
	QTreeWidgetItem *general;
	QTreeWidgetItem *generalSelected;
	QTreeWidgetItem *types;
	QTreeWidgetItem *current;
	QTreeWidgetItem *extra;

	QTreeWidgetItem* createInfoItem(const QString &tag, const QString &value);
	QTreeWidgetItem* createInfoItem(const QIcon &icon, const QString &tag, const QString &value);

	void showAudioFileInformation(const QFileInfo &fi);
	void showPictureFileInformation(const QFileInfo &fi);
	void showVideoFileInformation(const QFileInfo &fi);

	void showSelectedLength(QTreeWidgetItem *child, int seconds);
};

#endif /* QUPREVIEWTREE_H_ */
