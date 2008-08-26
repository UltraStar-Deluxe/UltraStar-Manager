#ifndef QUAUDIOTAGTASKDIALOG_H_
#define QUAUDIOTAGTASKDIALOG_H_

#include "QU.h"
#include "QUAudioTagTask.h"
#include "QUTaskDialog.h"

class QUAudioTagTaskDialog: public QUTaskDialog {
	Q_OBJECT

public:
	QUAudioTagTaskDialog(QUAudioTagTask *task, QWidget *parent = 0);
	QUAudioTagTaskDialog(QWidget *parent = 0);

protected:
	virtual bool saveTask(const QString &filePath);

private:
	void init(QUScriptableTask *task = 0);
};

#endif /* QUAUDIOTAGTASKDIALOG_H_ */
