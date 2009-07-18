#ifndef QUSONGTREE_H_
#define QUSONGTREE_H_

#include <QTreeWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QPoint>
#include <QList>
#include <QMenu>
#include <QKeyEvent>

#include "QU.h"
#include "QUSongItem.h"

class QUMainWindow;

class QUSongTree: public QTreeWidget {
	Q_OBJECT

public:
	QUSongTree(QWidget *parent = 0);
	QUMainWindow* parentWindow() const;

	void initHorizontalHeader();

	bool hasUnsavedChanges() const;
	bool hasHiddenItems() const { return !_hiddenItems.isEmpty(); }

	QList<QUSongFile*> selectedSongs();
	QList<QUSongItem*> selectedSongItems();
	QList<QUSongFile*> visibleSongs();
	QList<QUSongItem*> visibleSongItems() const;
	QList<QUSongItem*> allSongItems();

	int hiddenItemsCount() const { return _hiddenItems.size(); }

	void restoreSelection(const QList<QUSongItem*> &selectedItems);

public slots:
	void clear();
	void addItem(QUSongFile *song);
	void updateItem(QUSongFile *song);
	void resizeAndSort();

//	void refreshSelectedItems();
	void resizeToContents();
	void saveSelectedSongs();
	void saveUnsavedChanges();

	void filterItems(const QString &regexp, QU::FilterModes mode = QU::informationTags);
	void filterDuplicates();

signals:
	void songToPlaylistRequested(QUSongFile *song);
	void coversFromAmazonRequested(QList<QUSongItem*> items);
	void coverFlowRequested(QList<QUSongItem*> items);
	void backgroundFlowRequested(QList<QUSongItem*> items);

	void showLyricsRequested(QUSongFile *song);
	void editLyricsRequested(QUSongFile *song);

private slots:
	void deleteCurrentItem();
	void sendSelectedSongsToPlaylist();
	void requestLyrics();
	void requestEditLyrics();
	void requestCoversFromAmazon();
	void requestCoverFlow();
	void requestBackgroundFlow();

	void showItemMenu(const QPoint &point);
	void showHeaderMenu(const QPoint &point);

	void toggleColumn(bool show, int index);
	void showAllColumns();
	void showDefaultColumns();
	void showMinimalColumns();
	void showTimeColumns();
	void showCheckColumns();

	void openCurrentFile();
	void openCurrentFolder();

	void hideAll();
	void hideSelected();
	void hideSelectedOnly();
	void hideAllButSelected();

	void showMoreCurrentArtist();

	void requestDeleteSelectedSongs();

	void mergeSelectedSongs();

	void calculateSpeed();

protected:
	virtual void keyPressEvent(QKeyEvent *event);

private:
	QList<QUSongItem*> _hiddenItems;

	virtual bool dropMimeData(QTreeWidgetItem *parent, int index, const QMimeData *data, Qt::DropAction action);
	virtual QStringList mimeTypes() const;

	bool copyFilesToSong(const QList<QUrl> &files, QUSongItem *item);
	void dropSongFiles(const QList<QUrl> &urls);
	void createSongFolder(QUSongFile *song);

	void removeFilter();
};

#endif /*QUSONGTREE_H_*/
