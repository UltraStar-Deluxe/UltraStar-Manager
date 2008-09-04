#ifndef QUHTMLREPORT_H_
#define QUHTMLREPORT_H_

#include "QUAbstractReport.h"

#include <QDomDocument>

class QUHtmlReport: public QUAbstractReport {
	Q_OBJECT

public:
	QUHtmlReport(
			const QList<QUSongFile*> &songFiles,
			const QList<QUAbstractReportData*> &reportDataList,
			const QFileInfo &fi,
			QU::ReportOptions options = 0,
			const QVariant &userData = QVariant(),
			const QString &cssFilePath = QString(),
			QObject *parent = 0);

	virtual QString content() const;

private:
	QDomDocument _report;
	QFileInfo    _css;

	QDomElement html();

	void createHead();
	void createBody();

	void appendBasePath(QDomNode &parent);
	void appendUserData(QDomNode &parent);

	void appendSongsTable(QDomNode &parent);
	void appendSongsTableHead(QDomNode &parent);
	void appendSongsTableBody(QDomNode &parent);

	void appendLyrics(QDomNode &parent);
};

#endif /*QUHTMLREPORT_H_*/
