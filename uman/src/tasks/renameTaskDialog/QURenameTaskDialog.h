#ifndef QURENAMETASKDIALOG_H_
#define QURENAMETASKDIALOG_H_

#include "QU.h"
#include "QURenameTask.h"
#include <QDialog>

#include "ui_QURenameTaskDialog.h"

class QURenameTaskDialog: public QDialog, private Ui::QURenameTaskDialog {
	Q_OBJECT

public:
	QURenameTaskDialog(QURenameTask *task, QWidget *parent = 0);

private slots:
	void fillIconCombo(const QString &resourcePath);
	void controlGroupSpin(int exclusiveState);

	void addData();
	void removeData();

	void saveRenameTask();
	void saveRenameTask(const QString &filePath);
	void saveRenameTaskAs();

private:
	QString _fileName;
};

#endif /* QURENAMETASKDIALOG_H_ */
