#include "QUHelpButton.h"
#include "QUMessageBox.h"

#include <QIcon>

QUHelpButton::QUHelpButton(QWidget *parent): QToolButton(parent) {
	setAutoRaise(true);
	setIcon(QIcon(":/marks/help.png"));
	connect(this, SIGNAL(clicked()), this, SLOT(showHelp()));
}

void QUHelpButton::showHelp() {
	QUMessageBox::question(qobject_cast<QWidget*>(parent()), topic(), text(),
			QStringList() << ":/marks/accept.png" << "OK",
			330);
}
