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
	QURenameTaskDialog(QWidget *parent = 0);

private slots:
	void fillIconCombo(const QString &resourcePath);
	void controlGroupSpin(int exclusiveState);

	void addData();
	void removeData();

	void saveRenameTask();
	bool saveRenameTask(const QString &filePath);
	void saveRenameTaskAs();

	void updateMoveButtons(int row, int column);

signals:
	void finished(const QString &message, QU::EventMessageTypes type);

private:
	QString _fileName;

	void initDialog();
};

#endif /* QURENAMETASKDIALOG_H_ */
