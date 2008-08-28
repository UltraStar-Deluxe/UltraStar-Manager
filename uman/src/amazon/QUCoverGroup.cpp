#include "QUCoverGroup.h"

#include <QVariant>

QUCoverGroup::QUCoverGroup(const QString &name, QWidget *parent): QWidget(parent) {
	setupUi(this);

	list->setIconSize(QSize(COVER_ICON_WIDTH, COVER_ICON_HEIGHT));
	group->setTitle(name);

	connect(list, SIGNAL(itemActivated (QListWidgetItem*)), this, SLOT(previewActivePicture(QListWidgetItem*)));
}

void QUCoverGroup::previewActivePicture(QListWidgetItem *item) {
	QUPictureDialog *dlg = new QUPictureDialog(item->data(Qt::UserRole).toString(), this);
	dlg->exec();
	delete dlg;
}
