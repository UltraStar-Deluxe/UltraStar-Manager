#ifndef QURENAMETASKDIALOG_H_
#define QURENAMETASKDIALOG_H_

#include "QU.h"
#include "QURenameTask.h"
#include "QUTaskDialog.h"

class QURenameTaskDialog: public QUTaskDialog {
	Q_OBJECT

public:
	QURenameTaskDialog(QURenameTask *task = 0, QWidget *parent = 0);

protected:
	virtual QDir configurationDirectory() const;
	virtual bool saveTask(const QString &filePath);

private:
	void init(QUScriptableTask *task = 0);
};

#endif /* QURENAMETASKDIALOG_H_ */
