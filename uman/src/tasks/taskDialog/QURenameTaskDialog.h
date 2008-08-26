#ifndef QURENAMETASKDIALOG_H_
#define QURENAMETASKDIALOG_H_

#include "QU.h"
#include "QURenameTask.h"

#include "QUTaskDialog.h"

class QURenameTaskDialog: public QUTaskDialog {
	Q_OBJECT

public:
	QURenameTaskDialog(QURenameTask *task, QWidget *parent = 0);
	QURenameTaskDialog(QWidget *parent = 0);

protected:
	virtual bool saveTask(const QString &filePath);

};

#endif /* QURENAMETASKDIALOG_H_ */
