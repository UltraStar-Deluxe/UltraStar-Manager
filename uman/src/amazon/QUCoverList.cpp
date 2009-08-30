#include "QUCoverList.h"
#include "QUPictureDialog.h"

#include <QVariant>
#include <QMessageBox>

QUCoverList::QUCoverList(QWidget *parent): QListView(parent) {
	QUCoverModel *model = new QUCoverModel(this);

	setModel(model);
	setItemDelegate(new QUCoverItemDelegate(this));

	connect(this, SIGNAL(activated(const QModelIndex&)), this, SLOT(previewCover(const QModelIndex&)));
}

QString QUCoverList::currentFilePath() {
	if(!currentIndex().isValid())
		return QString();

	return model()->data(currentIndex(), Qt::UserRole).toString();
}

QUCoverModel* QUCoverList::model() const {
	return dynamic_cast<QUCoverModel*>(QListView::model());
}

void QUCoverList::previewCover(const QModelIndex &index) {
	QUPictureDialog *dlg = new QUPictureDialog(model()->data(index, Qt::UserRole).toString(), this);
	dlg->exec();
	delete dlg;
}
