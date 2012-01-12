#include "QUCSVReport.h"
#include "QUProgressDialog.h"

#include <QTextStream>

QUCSVReport::QUCSVReport(
		const QList<QUSongFile*> &songFiles,
		const QList<QUAbstractReportData*> &reportDataList,
		const QFileInfo &fi,
		QU::ReportOptions options,
		const QVariant &userData,
		QObject *parent):
	QUAbstractReport(songFiles, reportDataList, fi, options, userData, parent)
{

	QTextStream out(&_content);
	out.setFieldAlignment(QTextStream::AlignLeft);
	out.setPadChar(QChar::Nbsp);

	printHeading(out);
	printSongTable(out);
}

QString QUCSVReport::content() const {
	return _content;
}

void QUCSVReport::printHeading(QTextStream &out) {
	if(options().testFlag(QU::reportPrependCurrentPath))
		out << QString(tr("Songs Path: \"%1\"")).arg(QDir::toNativeSeparators(QU::BaseDir.path())) << endl << endl;

	if(options().testFlag(QU::reportPrependUserData))
		out << QString("\"%1\"").arg(userData().toString()) << endl << endl;

	/* selected columns */
	for(int i = 0; i < reportDataList().size(); i++) {
		out.setFieldWidth(0);
		out << ";";
		out << reportDataList().at(i)->headerTextData();
	}
	out.setFieldWidth(0);
	out << endl;
}

void QUCSVReport::printSongTable(QTextStream &out) {
	QUProgressDialog pDlg(tr("Creating CSV report..."), songs().size());
	pDlg.setPixmap(":/types/folder.png");
	pDlg.show();

	int rn = 1, max = QVariant(songs().size()).toString().length();
	foreach(QUSongFile *song, songs()) {
		pDlg.update(QString("%1 - %2").arg(song->artist()).arg(song->title()));
		if(pDlg.cancelled()) break;

		/* running number */
		out.setFieldWidth(0);
		out << QString("%1").arg(rn++, max, 10, QChar('0'));

		for(int i = 0; i < reportDataList().size(); i++) {
			out.setFieldWidth(0);
			out << ";";
			out << reportDataList().at(i)->textData(song);
		}
		out.setFieldWidth(0);
		out << endl;
	}
}
