#include "QUHtmlReport.h"
#include "QUMonty.h"

#include <QDomNodeList>

QUHtmlReport::QUHtmlReport(const QList<QUSongFile*> &songFiles, const QList<QUAbstractReportData*> &reportDataList, const QFileInfo &fi, QObject *parent): 
	QUAbstractReport(songFiles, reportDataList, fi, parent) 
{
	QDomElement html = _report.createElement("html");
	QDomElement head = _report.createElement("head");
	QDomElement title = _report.createElement("title");
	QDomElement body = _report.createElement("body");
	QDomElement table = _report.createElement("table");
	
	_report.appendChild(html);
	
	html.appendChild(head);
	html.appendChild(body);
	head.appendChild(title);
	body.appendChild(table);
	
	title.appendChild(_report.createTextNode(QObject::tr("UltraStar Manager - Song Report")));
	
	// header
	table.appendChild(_report.createElement("tr"));

	foreach(QUAbstractReportData *rd, reportDataList) {
		QDomElement th = _report.createElement("th");
		th.appendChild(_report.createTextNode(rd->headerData()));
		
		table.childNodes().at(0).appendChild(th);
	}
	
	// content
	foreach(QUSongFile *song, songFiles) {
		QDomElement tr = _report.createElement("tr");
		
		foreach(QUAbstractReportData *rd, reportDataList) {
			QDomElement td = _report.createElement("td");
			
			if(rd->type() == QU::text)
				td.appendChild(_report.createTextNode(rd->data(song)));
			else if(rd->type() == QU::icon) {
				QDomElement img = _report.createElement("img");
				QDomAttr src = _report.createAttribute("src");
				
				src.setNodeValue(monty->useImageFromResource(rd->data(song), _fi.dir()));
				img.setAttributeNode(src);
				
				td.appendChild(img);
			}
			
			tr.appendChild(td);			
		}
		
		table.appendChild(tr);
	}
}

QString QUHtmlReport::content() const {
	return _report.toString();
}
