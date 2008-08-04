#include "QUPlainTextReport.h"
#include "QUProgressDialog.h"

#include <QTextStream>

QUPlainTextReport::QUPlainTextReport(const QList<QUSongFile*> &songFiles, const QList<QUAbstractReportData*> &reportDataList, const QFileInfo &fi, QObject *parent):
	QUAbstractReport(songFiles, reportDataList, fi, parent)
{
	QTextStream out(&_content);
	out.setFieldAlignment(QTextStream::AlignLeft);
	out.setPadChar(QChar::Nbsp);

	QList<int> paddings;

	// compute max padding per column
	for(int i = 0; i < reportDataList.size(); i++) {
		paddings.append(reportDataList.at(i)->headerTextData().length());

		foreach(QUSongFile *song, songFiles) {
			paddings[i] = qMax(paddings[i], reportDataList.at(i)->textData(song).length());
		}
	}

	// heading
	int lineSize = 0;
	for(int i = 0; i < reportDataList.size(); i++) {
		out.setFieldWidth(0);
		out << " | ";                                     lineSize += 3;
		out.setFieldWidth(paddings[i]);                   lineSize += paddings[i];
		out << reportDataList[i]->headerTextData();
	}
	out.setFieldWidth(0);
	out << " | " << endl;                                 lineSize += 3;

	out << QString(lineSize, '=') << endl;

	// content
	QUProgressDialog pDlg(tr("Creating plain text report..."), songFiles.size());
	pDlg.setPixmap(":/types/folder.png");
	pDlg.show();

	foreach(QUSongFile *song, songFiles) {
		pDlg.update(QString("%1 - %2").arg(song->artist()).arg(song->title()));
		if(pDlg.cancelled()) break;

		for(int i = 0; i < reportDataList.size(); i++) {
			out.setFieldWidth(0);
			out << " | ";
			out.setFieldWidth(paddings[i]);
			out << reportDataList[i]->textData(song);
		}
		out.setFieldWidth(0);
		out << " | " << endl;
	}
}

QString QUPlainTextReport::content() const {
	return _content;
}
