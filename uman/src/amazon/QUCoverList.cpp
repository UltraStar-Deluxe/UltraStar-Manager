#include "QUCoverList.h"

#include <QVariant>
#include <QMessageBox>

QUCoverList::QUCoverList(QWidget *parent): QListView(parent) {
	QUCoverModel *model = new QUCoverModel(this);

	setModel(model);
	setItemDelegate(new QUCoverItemDelegate(this));

	connect(this, SIGNAL(activated(const QModelIndex&)), this, SLOT(passActivation(const QModelIndex&)));
}

QString QUCoverList::currentFilePath() {
	if(!currentIndex().isValid())
		return QString();

	return model()->data(currentIndex(), Qt::UserRole).toString();
}

QUCoverModel* QUCoverList::model() const {
	return dynamic_cast<QUCoverModel*>(QListView::model());
}

void QUCoverList::passActivation(const QModelIndex &index) {
	emit coverActivated(model()->data(index, Qt::UserRole).toString());
}
