#ifndef QUTASKDIALOG_H_
#define QUTASKDIALOG_H_

#include "QU.h"
#include "QUScriptableTask.h"

#include <QDialog>

#include "ui_QUTaskDialog.h"

class QUTaskDialog: public QDialog, protected Ui::QUTaskDialog {
	Q_OBJECT

public:
	QUTaskDialog(QUScriptableTask *task, QWidget *parent = 0);
	QUTaskDialog(QWidget *parent = 0);

protected slots:
	void fillIconCombo(const QString &resourcePath);
	void controlGroupSpin(int exclusiveState);

	void addData();
	void removeData();

	void saveTask();
	void saveTaskAs();

	void updateMoveButtons(int row, int column);

signals:
	void finished(const QString &message, QU::EventMessageTypes type);

protected:
	QString _fileName;

	void initDialog();

	virtual bool saveTask(const QString &filePath) = 0;
};

#endif /* QUTASKDIALOG_H_ */
