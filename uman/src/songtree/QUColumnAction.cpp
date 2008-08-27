#include "QUColumnAction.h"

QUColumnAction::QUColumnAction(const QString &text, const QVariant &userData, QObject *parent):
	QAction(text, parent)
{
	this->setData(userData);
	this->setCheckable(true);

	if(userData.toInt() == LENGTH_DIFF_COLUMN)
		this->setText(tr("Warnings"));

	connect(this, SIGNAL(toggled(bool)), SLOT(toggleColumn(bool)));
}

void QUColumnAction::toggleColumn(bool checked) {
	int index = this->data().toInt();

	emit columnToggled(checked, index);
}
