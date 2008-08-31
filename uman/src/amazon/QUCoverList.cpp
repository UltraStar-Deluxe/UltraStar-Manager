#include "QUCoverList.h"

QUCoverList::QUCoverList(QWidget *parent): QListView(parent) {
	setModel(new QUCoverModel(this));
}

void QUCoverList::addItem(const QString &filePath) {
//	QListWidget::addItem(item);
}

void QUCoverList::clear() {

}

QString QUCoverList::currentFilePath() {
	return "";
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
