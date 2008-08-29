#include "QUCoverList.h"

#include <QCoreApplication>
#include <QScrollBar>

QUCoverList::QUCoverList(QWidget *parent): QListWidget(parent) {
}

void QUCoverList::addItem(QListWidgetItem *item) {
	QListWidget::addItem(item);
}

/*!
 * Used to show almost all covers.
 */
void QUCoverList::adjustMinimumHeight() {
	QCoreApplication::processEvents(QEventLoop::AllEvents | QEventLoop::ExcludeUserInputEvents);
	setMinimumHeight((contentsSize().width() * contentsSize().height()) / qMax(1, viewport()->width()) + iconSize().height());
	QCoreApplication::processEvents(QEventLoop::AllEvents | QEventLoop::ExcludeUserInputEvents);
	setMinimumHeight(maximumViewportSize().height() + verticalScrollBar()->maximum());
}
