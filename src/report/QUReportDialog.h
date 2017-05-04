#ifndef QUREPORTDIALOG_H_
#define QUREPORTDIALOG_H_

#include "QU.h"
#include "QUSongFile.h"
#include "QUPlaylistFile.h"

#include "QUSongTagData.h"
#include "QUBooleanSongData.h"
#include "QUSongFileData.h"
#include "QUReportItem.h"

#include <QDialog>

#include "ui_QUReportDialog.h"

class QUReportDialog: public QDialog, private Ui::QUReportDialog {
	Q_OBJECT

public:
	QUReportDialog(
			const QList<QUSongFile*> &allSongs,
			const QList<QUSongFile*> &visibleSongs,
			const QList<QUPlaylistFile*> &allPlaylists,
			QWidget *parent = 0);
	~QUReportDialog();

private slots:
	void selectLetterFnt();
	void selectTopLevelEntryFnt();
	void selectSubLevelEntryFnt();
	void selectSubSubLevelEntryFnt();
	void selectLetterClr();
	void selectTopLevelEntryClr();
	void selectSubLevelEntryClr();
	void selectSubSubLevelEntryClr();
	void createPdfReport();
	void createHtmlReport();
	void createPlainTextReport();
	void createCSVReport();

	void togglePlaylistSource(bool checked);
	void toggleStyleCombo(bool checked);
	void toggleAppendLyrics(bool checked);

private:
	QList<QUSongFile*> _allSongs;
	QList<QUSongFile*> _visibleSongs;
	QList<QUPlaylistFile*> _allPlaylists;
	QFont _letterFnt;
	QFont _topLevelEntryFnt;
	QFont _subLevelEntryFnt;
	QFont _subSubLevelEntryFnt;
	QColor _letterClr;
	QColor _topLevelEntryClr;
	QColor _subLevelEntryClr;
	QColor _subSubLevelEntryClr;

	QList<QUAbstractReportData*> _data;

	void initReportList();
	void initStyleCombo();
	void initPlaylistCombo();
	void initPaperSizeCombo();
	void initOrientationCombo();
	void initSelectFntBtns();
	void initSelectClrBtns();

	void fetchData();
	QList<QUSongFile*> sortedSongs();

	QU::ReportOptions selectedOptions() const;
	QString currentPlaylistName() const;

	void saveState();
	void loadState();
};

#endif /*QUREPORTDIALOG_H_*/
