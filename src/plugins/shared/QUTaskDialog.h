#ifndef QUTASKDIALOG_H_
#define QUTASKDIALOG_H_

#include "QU.h"
#include "QUScriptableTask.h"

#include <QDialog>
#include <QString>
#include <QDomDocument>
#include <QDomElement>
#include <QDir>

#include "ui_QUTaskDialog.h"

class QUTaskDialog: public QDialog, protected Ui::QUTaskDialog {
	Q_OBJECT

public:
	QUTaskDialog(QUScriptableTask *task = 0, QWidget *parent = 0);

protected slots:
	void fillIconCombo(const QString &resourcePath);
	void controlGroupSpin(int exclusiveState);

	void addData();
	void removeData();

	void saveTask();
	void saveTaskAs();

	void updateMoveButtons(int row, int column);

protected:
	QString _fileName;
	QDomDocument _doc;

	void initForTask();
	void initForTask(QUScriptableTask *task);

	virtual QDir configurationDirectory() const = 0;
	virtual bool saveTask(const QString &filePath) = 0;
	bool saveDocument(const QString &filePath);

	void appendGeneral(QDomElement &parent, QUScriptableTask::TaskTypes type);
};

#endif /* QUTASKDIALOG_H_ */
