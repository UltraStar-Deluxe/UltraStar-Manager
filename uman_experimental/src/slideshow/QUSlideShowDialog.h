#ifndef QUSLIDESHOWDIALOG_H
#define QUSLIDESHOWDIALOG_H

#include <QDialog>
#include <QKeyEvent>

#include "QUSongItem.h"
#include "QUSongFile.h"

#include "ui_QUSlideShowDialog.h"

class QUSlideShowDialog: public QDialog, private Ui::QUSlideShowDialog {
	Q_OBJECT

public:
	enum Mode {
		coverflow,
		backgroundflow
	};
	Q_DECLARE_FLAGS(Modes, Mode)

	QUSlideShowDialog(QList<QUSongItem*> items, Modes m = coverflow, QWidget *parent = 0);

public slots:
	void changeSelection(int index);
	void changeUnlinkFlag(bool unlink);
	void accept();
	void reject();

protected:
	virtual void keyPressEvent(QKeyEvent* event);

private:
	QList<QUSongItem*> _items;
	Modes              _mode;

	void preparePictures();
};

#endif // QUSLIDESHOWDIALOG_H
