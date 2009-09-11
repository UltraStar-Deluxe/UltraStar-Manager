#ifndef QUREMOTEIMAGEDIALOG_H
#define QUREMOTEIMAGEDIALOG_H

#include <QDialog>
#include <QList>

#include "ui_QURemoteImageDialog.h"

class QUSongItem;
class QUCoverGroup;

class QURemoteImageDialog: public QDialog, private Ui::QURemoteImageDialog {
	Q_OBJECT

public:
	QURemoteImageDialog(const QList<QUSongItem*> &items, QWidget *parent = 0);

private slots:
	void copyAndSetCovers();
	void checkAllGroups();
	void uncheckAllGroups();
	void getCovers();

private:
	QList<QUCoverGroup*> _groups;

	void initResultsPage(const QList<QUSongItem*> &items);
};

#endif // QUREMOTEIMAGEDIALOG_H
