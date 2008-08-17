#include "QUHtmlReport.h"
#include "QUMonty.h"
#include "QUProgressDialog.h"
#include "QUMainWindow.h"

#include <QDomNodeList>

QUHtmlReport::QUHtmlReport(const QList<QUSongFile*> &songFiles, const QList<QUAbstractReportData*> &reportDataList, const QFileInfo &fi, bool showBaseDir, QObject *parent):
	QUAbstractReport(songFiles, reportDataList, fi, showBaseDir, parent)
{
	QDomElement html = _report.createElement("html");
	QDomElement head = _report.createElement("head");
	QDomElement title = _report.createElement("title");
	QDomElement body = _report.createElement("body");
	QDomElement div = _report.createElement("div");
	QDomElement table = _report.createElement("table");

	_report.appendChild(html);

	html.appendChild(head);
	html.appendChild(body);
	head.appendChild(title);
	if(showBaseDir) body.appendChild(div);
	body.appendChild(table);

	title.appendChild(_report.createTextNode(tr("UltraStar Manager - Song Report")));

	// header
	div.appendChild(_report.createTextNode(QString(tr("Songs Path: \"%1\"")).arg(QUMainWindow::BaseDir.path())));

	table.appendChild(_report.createElement("tr"));

	foreach(QUAbstractReportData *rd, reportDataList) {
		QDomElement th = _report.createElement("th");

		if(!rd->headerIconData().isEmpty()) {
			QDomElement img = _report.createElement("img");
			QDomAttr src = _report.createAttribute("src");
			QDomAttr alt = _report.createAttribute("alt");
			QDomAttr title = _report.createAttribute("title");

			src.setNodeValue(monty->useImageFromResource(rd->headerIconData(), _fi.dir()));
			alt.setNodeValue(rd->headerTextData());
			title.setNodeValue(rd->headerTextData());

			img.setAttributeNode(src);
			img.setAttributeNode(alt);
			img.setAttributeNode(title);

			th.appendChild(img);
		} else if(!rd->headerTextData().isEmpty())
			th.appendChild(_report.createTextNode(rd->headerTextData()));

		table.childNodes().at(0).appendChild(th);
	}

	// content
	QUProgressDialog pDlg(tr("Creating html report..."), songFiles.size());
	pDlg.setPixmap(":/types/folder.png");
	pDlg.show();

	foreach(QUSongFile *song, songFiles) {
		pDlg.update(QString("%1 - %2").arg(song->artist()).arg(song->title()));
		if(pDlg.cancelled()) break;

		QDomElement tr = _report.createElement("tr");

		foreach(QUAbstractReportData *rd, reportDataList) {
			QDomElement td = _report.createElement("td");

			if(!rd->iconData(song).isEmpty()) {
				QDomElement img = _report.createElement("img");
				QDomAttr src = _report.createAttribute("src");
				QDomAttr alt = _report.createAttribute("alt");
				QDomAttr title = _report.createAttribute("title");

				src.setNodeValue(monty->useImageFromResource(rd->iconData(song), _fi.dir()));
				alt.setNodeValue(rd->textData(song));
				title.setNodeValue(rd->textData(song));

				img.setAttributeNode(src);
				img.setAttributeNode(alt);
				img.setAttributeNode(title);

				td.appendChild(img);
			} else if(!rd->textData(song).isEmpty()) {
				td.appendChild(_report.createTextNode(rd->textData(song)));
			}

			tr.appendChild(td);
		}

		table.appendChild(tr);
	}
}

QString QUHtmlReport::content() const {
	return _report.toString();
}
