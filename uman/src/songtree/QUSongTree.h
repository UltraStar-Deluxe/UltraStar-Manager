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

class QUSongTree: public QTreeWidget {
	Q_OBJECT

public:
	QUSongTree(QWidget *parent = 0);
	void initHorizontalHeader();

	bool hasUnsavedChanges() const;
	bool hasHiddenItems() const { return !_hiddenItems.isEmpty(); }

	void clear();

	QList<QUSongFile*> selectedSongs();
	void fill(QList<QUSongFile*> songs);

public slots:
	void showContextMenu(const QPoint &point);
	void refreshSelectedItems();
	void resizeToContents();
	void saveSelectedSongs();
	void saveUnsavedChanges();

	void filterItems(const QString &regexp, QU::FilterModes mode = QU::informationTags);

signals:
	void finished(const QString &message, QU::EventMessageTypes type);
	void songCreated(QUSongFile *song);

private slots:
	void deleteCurrentItem();

protected:
	virtual void keyPressEvent(QKeyEvent *event);

private:
	QList<QTreeWidgetItem*> _hiddenItems;

	virtual bool dropMimeData(QTreeWidgetItem *parent, int index, const QMimeData *data, Qt::DropAction action);
	virtual QStringList mimeTypes() const;

	bool copyFilesToSong(const QList<QUrl> &files, QUSongItem *item);
	bool dropSongFiles(const QList<QUrl> &urls);

	void fillContextMenu(QMenu &menu, const QPoint &point);
};

#endif /*QUSONGTREE_H_*/
