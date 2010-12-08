#include "QUPlainTextReport.h"
#include "QUProgressDialog.h"

#include <QTextStream>

QUPlainTextReport::QUPlainTextReport(
		const QList<QUSongFile*> &songFiles,
		const QList<QUAbstractReportData*> &reportDataList,
		const QFileInfo &fi,
		QU::ReportOptions options,
		const QVariant &userData,
		QObject *parent):
	QUAbstractReport(songFiles, reportDataList, fi, options, userData, parent)
{
	computePaddings();

	QTextStream out(&_content);
	out.setFieldAlignment(QTextStream::AlignLeft);
	out.setPadChar(QChar::Nbsp);

	printHeading(out);
	printSongTable(out);
	printLyrics(out);
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
	if(options().testFlag(QU::reportPrependCurrentPath))
		out << QString(tr("Songs Path: \"%1\"")).arg(QU::BaseDir.path()) << endl << endl;

	if(options().testFlag(QU::reportPrependUserData))
		out << QString("\"%1\"").arg(userData().toString()) << endl << endl;

	/* running number */
	int rnWidth = QVariant(songs().size()).toString().length();
	out.setFieldWidth(0);
	out << " " << QString(rnWidth, QChar(' '));

	/* selected columns */
	int lineSize = rnWidth;
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

	int rn = 1, max = QVariant(songs().size()).toString().length();
	foreach(QUSongFile *song, songs()) {
		pDlg.update(QString("%1 - %2").arg(song->artist()).arg(song->title()));
		if(pDlg.cancelled()) break;

		/* running number */
		out.setFieldWidth(0);
		out << " " << QString("%1").arg(rn++, max, 10, QChar('0'));

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

void QUPlainTextReport::printLyrics(QTextStream &out) {
	if(!options().testFlag(QU::reportAppendLyrics))
		return;

	QUProgressDialog pDlg(tr("Appending lyrics..."), songs().size());
	pDlg.setPixmap(":/types/text.png");
	pDlg.show();

	out << endl << endl;

	int i = 1;
	foreach(QUSongFile *song, songs()) {
		pDlg.update(QString("%1 - %2").arg(song->artist()).arg(song->title()));
		if(pDlg.cancelled()) break;

		QString subheader = QString("%3. %1 - %2").arg(song->artist()).arg(song->title()).arg(i++, QVariant(songs().size()).toString().length(), 10, QChar('0'));

		out << QString(subheader.length(), '-') << endl;
		out << subheader << endl;
		out << QString(subheader.length(), '-') << endl;

		out << song->lyrics().join("\n").remove(QRegExp("<b>|</b>|<i>|</i>")) << endl << endl;
	}
}
