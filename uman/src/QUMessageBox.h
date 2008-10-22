#ifndef QUMESSAGEBOX_H_
#define QUMESSAGEBOX_H_

#include <QDialog>
#include <QCloseEvent>

#include "QU.h"

#include "ui_QUMessageBox.h"

class QUMessageBox: public QDialog, private Ui::QUMessageBox {
	Q_OBJECT

public:
	enum Result {
		none,
		first,
		second,
		third
	};
	Q_DECLARE_FLAGS(Results, Result);

	static QUMessageBox::Results ask(
			QWidget *parent,
			const QString &title,
			const QString &message,
			const QString &icon1, const QString &text1,
			const QString &icon2 = "", const QString &text2 = "",
			const QString &icon3 = "", const QString &text3 = "",
			int widthChange = 0,
			QU::EventMessageTypes type = QU::information);

public slots:
	virtual void reject();

protected:
	QUMessageBox(QWidget *parent = 0);

	QUMessageBox::Results result() const { return _result; }

	virtual void closeEvent(QCloseEvent *event);

private slots:
	void firstClicked() { _result = QUMessageBox::first; this->accept(); }
	void secondClicked() { _result = QUMessageBox::second; this->accept(); }
	void thirdClicked() { _result = QUMessageBox::third; this->accept(); }

private:
	QUMessageBox::Results _result;
};

#endif /*QUMESSAGEBOX_H_*/
