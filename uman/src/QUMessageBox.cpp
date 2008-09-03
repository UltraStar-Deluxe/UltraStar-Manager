#include "QUMessageBox.h"

#include <QIcon>

QUMessageBox::QUMessageBox(QWidget *parent): QDialog(parent) {
	setupUi(this);

	this->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

	_result = QUMessageBox::none;

	connect(one, SIGNAL(clicked()), this, SLOT(firstClicked()));
	connect(two, SIGNAL(clicked()), this, SLOT(secondClicked()));
	connect(three, SIGNAL(clicked()), this, SLOT(thirdClicked()));
}

void QUMessageBox::reject() {
	// cannot reject
}

void QUMessageBox::closeEvent(QCloseEvent *event) {
	if(_result == QUMessageBox::none) {
		event->ignore();
	} else {
		event->accept();
	}
}

QUMessageBox::Results QUMessageBox::ask(
		QWidget *parent,
		const QString &title,
		const QString &message,
		const QString &icon1, const QString &text1,
		const QString &icon2, const QString &text2,
		const QString &icon3, const QString &text3,
		int widthChange)
{
	QUMessageBox *dlg = new QUMessageBox(parent);
	dlg->resize(dlg->width() + widthChange, dlg->height());

	dlg->setWindowTitle(title);
	dlg->message->setText(message);

	dlg->one->setIcon(QIcon(icon1));
	dlg->one->setText(text1);
	dlg->one->setFocus();

	if(text2.isEmpty())
		dlg->two->hide();
	else {
		dlg->two->setIcon(QIcon(icon2));
		dlg->two->setText(text2);
		dlg->two->setFocus();
	}

	if(text3.isEmpty())
		dlg->three->hide();
	else {
		dlg->three->setIcon(QIcon(icon3));
		dlg->three->setText(text3);
		dlg->three->setFocus();
	}

	dlg->exec();

	QUMessageBox::Results r = dlg->result();

	delete dlg;

	return r;
}
