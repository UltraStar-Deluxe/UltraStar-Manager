#ifndef QUCOVERLIST_H_
#define QUCOVERLIST_H_

#include "QU.h"
#include "QUCoverModel.h"
#include "QUCoverItemDelegate.h"

#include <QListView>
#include <QString>

class QUCoverList: public QListView {
	Q_OBJECT

public:
	QUCoverList(QWidget *parent = 0);

	QString currentFilePath();
	QUCoverModel* model() const;

private slots:
	void previewCover(const QModelIndex &index);
};

#endif /* QUCOVERLIST_H_ */
