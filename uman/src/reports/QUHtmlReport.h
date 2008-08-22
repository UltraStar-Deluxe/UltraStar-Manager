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
			bool showBaseDir = false,
			const QString &cssFilePath = QString(),
			QObject *parent = 0);

	virtual QString content() const;

private:
	QDomDocument _report;
};

#endif /*QUHTMLREPORT_H_*/
