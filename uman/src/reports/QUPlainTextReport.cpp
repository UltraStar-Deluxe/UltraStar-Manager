#include "QUPlainTextReport.h"
#include "QUProgressDialog.h"
#include "QUMainWindow.h"

#include <QTextStream>

QUPlainTextReport::QUPlainTextReport(const QList<QUSongFile*> &songFiles, const QList<QUAbstractReportData*> &reportDataList, const QFileInfo &fi, bool showBaseDir, QObject *parent):
	QUAbstractReport(songFiles, reportDataList, fi, showBaseDir, parent)
{
	computePaddings();

	QTextStream out(&_content);
	out.setFieldAlignment(QTextStream::AlignLeft);
	out.setPadChar(QChar::Nbsp);

	printHeading(out);
	printSongTable(out);
}

QString QUPlainTextReport::content() const {
	return _content;
}

void QUPlainTextReport::computePaddings() {
	for(int i = 0; i < reportDataList().size(); i++) {
		_paddings.append(reportDataList().at(i)->headerTextData().length());

		foreach(QUSongFile *song, songs()) {
			_paddings[i] = qMax(_paddings[i], reportDataList().at(i)->textData(song).length());
		}
	}
}

void QUPlainTextReport::printHeading(QTextStream &out) {
	if(showBasePath())
		out << QString(tr("Songs Path: \"%1\"")).arg(QUMainWindow::BaseDir.path()) << endl << endl;

	int lineSize = 0;
	for(int i = 0; i < reportDataList().size(); i++) {
		out.setFieldWidth(0);
		out << " | ";                                     lineSize += 3;
		out.setFieldWidth(_paddings[i]);                  lineSize += _paddings[i];
		out << reportDataList().at(i)->headerTextData();
	}
	out.setFieldWidth(0);
	out << " | " << endl;                                 lineSize += 3;

	out << QString(lineSize, '=') << endl;
}

void QUPlainTextReport::printSongTable(QTextStream &out) {
	QUProgressDialog pDlg(tr("Creating plain text report..."), songs().size());
	pDlg.setPixmap(":/types/folder.png");
	pDlg.show();

	foreach(QUSongFile *song, songs()) {
		pDlg.update(QString("%1 - %2").arg(song->artist()).arg(song->title()));
		if(pDlg.cancelled()) break;

		for(int i = 0; i < reportDataList().size(); i++) {
			out.setFieldWidth(0);
			out << " | ";
			out.setFieldWidth(_paddings[i]);
			out << reportDataList().at(i)->textData(song);
		}
		out.setFieldWidth(0);
		out << " | " << endl;
	}
}
