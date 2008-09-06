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

	void clear();

	QList<QUSongFile*> selectedSongs();
	QList<QUSongItem*> selectedSongItems();
	QList<QUSongFile*> visibleSongs();
	QList<QUSongItem*> visibleSongItems();
	QList<QUSongItem*> allSongItems();

	void fill(QList<QUSongFile*> songs);

	int hiddenItemsCount() const { return _hiddenItems.size(); }

	void restoreSelection(const QList<QUSongItem*> &selectedItems);

public slots:
	void refreshSelectedItems();
	void resizeToContents();
	void saveSelectedSongs();
	void saveUnsavedChanges();

	void filterItems(const QString &regexp, QU::FilterModes mode = QU::informationTags);
	void filterDuplicates();

signals:
	void finished(const QString &message, QU::EventMessageTypes type);
	void songCreated(QUSongFile *song);
	void songToPlaylistRequested(QUSongFile *song);
	void showLyricsRequested(QUSongFile *song);
	void coversFromAmazonRequested(QList<QUSongItem*> items);
	void deleteSongRequested(QUSongFile *song);

private slots:
	void deleteCurrentItem();
	void sendSelectedSongsToPlaylist();
	void requestLyrics();
	void requestCoversFromAmazon();

	void showItemMenu(const QPoint &point);
	void showHeaderMenu(const QPoint &point);

	void toggleColumn(bool show, int index);
	void showAllColumns();
	void showDefaultColumns();
	void showMinimalColumns();
	void showTimeColumns();
	void showCheckColumns();

	void openCurrentFile();

	void hideAll();
	void hideSelected();
	void hideSelectedOnly();
	void hideAllButSelected();

	void requestDeleteSelectedSongs();

	void mergeSelectedSongs();

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
