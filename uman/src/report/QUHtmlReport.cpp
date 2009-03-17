#include "QUHtmlReport.h"
#include "QUMonty.h"
#include "QUProgressDialog.h"

#include "QUBooleanSongData.h"

#include <QDomNodeList>
#include <QFile>
#include <QMessageBox>

QUHtmlReport::QUHtmlReport(
		const QList<QUSongFile*> &songFiles,
		const QList<QUAbstractReportData*> &reportDataList,
		const QFileInfo &fi,
		QU::ReportOptions options,
		const QVariant &userData,
		const QString &cssFilePath,
		QObject *parent):
	QUAbstractReport(songFiles, reportDataList, fi, options, userData, parent)
{
	_css.setFile(cssFilePath);

	this->createHead();
	this->createBody();

	// copy style sheet to destination
	if(options.testFlag(QU::reportUseStyleSheet))
		QFile::copy(_css.filePath(), QFileInfo(fileInfo().dir(), _css.fileName()).filePath());
}

QString QUHtmlReport::content() const {
	return _report.toString();
}

QDomElement QUHtmlReport::html() {
	if(_report.firstChildElement("html").isNull())
		_report.appendChild(_report.createElement("html"));

	return _report.firstChildElement("html");
}

void QUHtmlReport::createHead() {
	QDomElement  head = _report.createElement("head");
	QDomElement  link = _report.createElement("link");
	QDomElement title = _report.createElement("title");

	title.appendChild(_report.createTextNode(tr("UltraStar Manager - Song Report")));

	link.setAttribute("href", _css.fileName());
	link.setAttribute("txpe", "text/css");
	link.setAttribute("rel", "stylesheet");

	head.appendChild(title);

	if(options().testFlag(QU::reportUseStyleSheet))
		head.appendChild(link);

	this->html().appendChild(head);
}

void QUHtmlReport::createBody() {
	QDomElement body = _report.createElement("body");

	appendBasePath(body);
	appendUserData(body);
	appendSongsTable(body);
	appendLyrics(body);

	this->html().appendChild(body);
}

void QUHtmlReport::appendBasePath(QDomNode &parent) {
	QDomElement div = _report.createElement("div");
	div.appendChild(_report.createTextNode(QString(tr("Songs Path: \"%1\"")).arg(QU::BaseDir.path())));
	div.setAttribute("class", "path");

	if(options().testFlag(QU::reportPrependCurrentPath))
		parent.appendChild(div);
}

void QUHtmlReport::appendUserData(QDomNode &parent) {
	QDomElement div = _report.createElement("div");
	div.appendChild(_report.createTextNode(QString("\"%1\"").arg(userData().toString())));
	div.setAttribute("class", "userdata");

	if(options().testFlag(QU::reportPrependUserData))
		parent.appendChild(div);
}

void QUHtmlReport::appendSongsTable(QDomNode &parent) {
	QDomElement table = _report.createElement("table");
	table.setAttribute("class", "list");

	appendSongsTableHead(table);
	appendSongsTableBody(table);

	parent.appendChild(table);
}

void QUHtmlReport::appendSongsTableHead(QDomNode &parent) {
	QDomElement thead = _report.createElement("thead");
	QDomElement tr    = _report.createElement("tr");

	/* running number */
	tr.appendChild(_report.createElement("th"));

	/* selected columns */
	foreach(QUAbstractReportData *rd, reportDataList()) {
		QDomElement th = _report.createElement("th");

		if(!rd->headerIconData().isEmpty()) {
			QDomElement img = _report.createElement("img");
			QDomAttr src = _report.createAttribute("src");
			QDomAttr alt = _report.createAttribute("alt");
			QDomAttr title = _report.createAttribute("title");

			src.setNodeValue(monty->useImageFromResource(rd->headerIconData(), fileInfo().dir()));
			alt.setNodeValue(rd->headerTextData());
			title.setNodeValue(rd->headerTextData());

			img.setAttributeNode(src);
			img.setAttributeNode(alt);
			img.setAttributeNode(title);

			th.appendChild(img);
		} else if(!rd->headerTextData().isEmpty())
			th.appendChild(_report.createTextNode(rd->headerTextData()));

		tr.appendChild(th);
	}

	thead.appendChild(tr);
	parent.appendChild(thead);
}

void QUHtmlReport::appendSongsTableBody(QDomNode &parent) {
	QDomElement tbody = _report.createElement("tbody");

	QUProgressDialog pDlg(tr("Creating HTML report..."), songs().size());
	pDlg.setPixmap(":/types/folder.png");
	pDlg.show();
	bool odd = true;

	int rn = 1;
	foreach(QUSongFile *song, songs()) {
		pDlg.update(QString("%1 - %2").arg(song->artist()).arg(song->title()));
		if(pDlg.cancelled()) break;

		QDomElement tr = _report.createElement("tr");
		tr.setAttribute("class", odd ? "odd" : "even"); odd = !odd;

		/* running number */
		QDomElement rtd = _report.createElement("td");
		rtd.appendChild(_report.createTextNode(QString("%1").arg(rn)));
		rtd.setAttribute("class", "rn");
		tr.appendChild(rtd);

		/* selected columns */
		foreach(QUAbstractReportData *rd, reportDataList()) {
			QDomElement td = _report.createElement("td");

			QDomElement a  = _report.createElement("a");
			a.setAttribute("href", QString("#%1").arg(rn));

			QUBooleanSongData *bData = dynamic_cast<QUBooleanSongData*>(rd);
			if(bData and !bData->iconData(song).isEmpty()) {
				QDomElement img = _report.createElement("img");
				QDomAttr src = _report.createAttribute("src");
				QDomAttr alt = _report.createAttribute("alt");
				QDomAttr title = _report.createAttribute("title");

				src.setNodeValue(monty->useImageFromResource(rd->iconData(song), fileInfo().dir()));
				alt.setNodeValue(rd->textData(song));
				title.setNodeValue(rd->textData(song));

				img.setAttributeNode(src);
				img.setAttributeNode(alt);
				img.setAttributeNode(title);

				if(options().testFlag(QU::reportLinkLyrics)) {
					a.appendChild(img);
					td.appendChild(a);
				} else {
					td.appendChild(img);
				}
			} else if(!bData and !rd->textData(song).isEmpty()) {
				if(options().testFlag(QU::reportLinkLyrics)) {
					a.appendChild(_report.createTextNode(rd->textData(song)));
					td.appendChild(a);
				} else {
					td.appendChild(_report.createTextNode(rd->textData(song)));
				}
			}

			tr.appendChild(td);
		}

		tbody.appendChild(tr);
		rn++;
	}

	parent.appendChild(tbody);
}

void QUHtmlReport::appendLyrics(QDomNode &parent) {
	if(!options().testFlag(QU::reportAppendLyrics))
		return;

	QUProgressDialog pDlg(tr("Appending lyrics..."), songs().size());
	pDlg.setPixmap(":/types/text.png");
	pDlg.show();

	QDomElement table = _report.createElement("table"); table.setAttribute("class", "lyrics");
	QDomElement tr = _report.createElement("tr"); table.appendChild(tr);
	QDomElement td = _report.createElement("td"); tr.appendChild(td);

	int rn = 1;
	foreach(QUSongFile *song, songs()) {
		pDlg.update(QString("%1 - %2").arg(song->artist()).arg(song->title()));
		if(pDlg.cancelled()) break;

		QDomElement div = _report.createElement("div");
		div.setAttribute("class", "song");
		if(options().testFlag(QU::reportLinkLyrics))
			div.setAttribute("id", QVariant(rn).toString());

		QDomElement h1 = _report.createElement("h1");
		h1.appendChild(_report.createTextNode(QString("%3. %1 - %2").arg(song->artist()).arg(song->title()).arg(rn)));
		div.appendChild(h1);

		QString lyrics = "<root><p>";
		lyrics += song->lyrics().join("<br/>").replace("<br/><br/>", "</p><br/><p>");
		lyrics.append("</p></root>");

		QDomDocument tmp; tmp.setContent(lyrics);
		QDomNodeList nodes = tmp.firstChildElement("root").childNodes();

		for(int i = 0; i < nodes.count(); i++)
			div.appendChild(nodes.at(i));

		td.appendChild(div);
		rn++;
	}

	parent.appendChild(table);
}
