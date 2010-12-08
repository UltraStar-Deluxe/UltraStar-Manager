#ifndef QUREMOTEIMAGEDIALOG_H
#define QUREMOTEIMAGEDIALOG_H

#include <QDialog>
#include <QList>

#include "ui_QURemoteImageDialog.h"

class QUSongItem;
class QUCoverGroup;
class QURemoteImageSource;

class QURemoteImageDialog: public QDialog, private Ui::QURemoteImageDialog {
	Q_OBJECT

public:
	QURemoteImageDialog(const QList<QUSongItem*> &items, QWidget *parent = 0);

private slots:
	void copyAndSetCovers();
	void checkAllGroups();
	void uncheckAllGroups();
	void getCovers();
	void updateResultsPage();
	void toggleConfigurationPage(bool);

private:
	QList<QUCoverGroup*> _groups;

	void initImageSources();
	void initResultsPage(const QList<QUSongItem*> &items);

	QURemoteImageSource* currentImageSource() const;

	QWidget* configurationPage(QURemoteImageSource *src);
};

#endif // QUREMOTEIMAGEDIALOG_H
