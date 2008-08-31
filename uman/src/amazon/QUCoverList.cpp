#include "QUCoverList.h"

#include <QVariant>

QUCoverList::QUCoverList(QWidget *parent): QListView(parent) {
	QUCoverModel *model = new QUCoverModel(this);

	setModel(model);

	connect(this, SIGNAL(activated(const QModelIndex&)), this, SLOT(passActivation(const QModelIndex&)));
}

QString QUCoverList::currentFilePath() {
	if(!currentIndex().isValid())
		return QString();

	return model()->data(currentIndex(), Qt::UserRole).toString();
}

QUCoverModel* QUCoverList::model() {
	return dynamic_cast<QUCoverModel*>(QListView::model());
}

void QUCoverList::passActivation(const QModelIndex &index) {
	emit coverActivated(model()->data(index, Qt::UserRole).toString());
}

/*!
 * Used to show almost all covers.
 */
//void QUCoverList::adjustMinimumHeight() {
//	QCoreApplication::processEvents(QEventLoop::AllEvents | QEventLoop::ExcludeUserInputEvents);
//	setMinimumHeight((contentsSize().width() * contentsSize().height()) / qMax(1, viewport()->width()) + iconSize().height());
//	QCoreApplication::processEvents(QEventLoop::AllEvents | QEventLoop::ExcludeUserInputEvents);
//	setMinimumHeight(maximumViewportSize().height() + verticalScrollBar()->maximum());
//}
