#ifndef QUPICTUREDIALOG_H_
#define QUPICTUREDIALOG_H_

#define EXTRA_HEIGHT  75

#include <QDialog>
#include <QCloseEvent>
#include "ui_QUPictureDialog.h"

class QUPictureDialog: public QDialog, private Ui::QUPictureDialog {
	Q_OBJECT

public:
	QUPictureDialog(const QString &filePath, QWidget *parent = 0);

private slots:
	void fitPicture();
	void fullPicture();

public slots:
	virtual void accept();
	virtual void reject();

protected:
	int initialWidth() const;
	void saveInitialWidth();

private:
	QString  _filePath;
};

#endif /*QUPICTUREDIALOG_H_*/
