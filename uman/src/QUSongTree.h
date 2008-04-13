#ifndef QUSONGTREE_H_
#define QUSONGTREE_H_

#include <QTreeWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QPoint>

class QUSongTree: public QTreeWidget {
	Q_OBJECT
	
public:
	QUSongTree(QWidget *parent = 0);
	void initHorizontalHeader();

public slots:
	void showContextMenu(const QPoint &point);
	
private:
	
	virtual bool dropMimeData(QTreeWidgetItem *parent, int index, const QMimeData *data, Qt::DropAction action);
	virtual QStringList mimeTypes() const;
	
};

#endif /*QUSONGTREE_H_*/
