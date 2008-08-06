#ifndef QUPREVIEWTREE_H_
#define QUPREVIEWTREE_H_

#define SONG_COUNT_CHILD_INDEX          0
#define SELECTED_SONG_COUNT_CHILD_INDEX 1
#define HIDDEN_SONG_COUNT_CHILD_INDEX   2
#define VISIBLE_SONG_COUNT_CHILD_INDEX  3

#include "QU.h"

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QFileInfo>

class QUPreviewTree: public QTreeWidget {
public:
	QUPreviewTree(QWidget *parent = 0);

public slots:
	void setSongCount(int count);
	void setSelectedSongCount(int count);
	void setHiddenSongCount(int count);
	void setVisibleSongCount(int count);
	void showFileInformation(const QFileInfo &fi);

private:
	QTreeWidgetItem *general;
	QTreeWidgetItem *current;
	QTreeWidgetItem *extra;

	QTreeWidgetItem* createInfoItem(const QString &tag, const QString &value);

	void showAudioFileInformation(const QFileInfo &fi);
	void showPictureFileInformation(const QFileInfo &fi);
	void showVideoFileInformation(const QFileInfo &fi);
};

#endif /* QUPREVIEWTREE_H_ */
