#ifndef QUCUSTOMTAGSDIALOG_H_
#define QUCUSTOMTAGSDIALOG_H_

#include "QU.h"
#include "QUSongFile.h"

#include <QDialog>

#include "ui_QUCustomTagsDialog.h"

class QUCustomTagsDialog: public QDialog, private Ui::QUCustomTagsDialog {
	Q_OBJECT

public:
	QUCustomTagsDialog(QWidget *parent = 0);

private slots:
	void save();

	void addTag();
	void removeCurrentTag();
};

#endif /* QUCUSTOMTAGSDIALOG_H_ */
