#ifndef QUPDFREPORT_H
#define QUPDFREPORT_H

#include "QUAbstractReport.h"

class QUPdfReport: public QUAbstractReport {
	Q_OBJECT

public:
	QUPdfReport(
			const QList<QUSongFile*> &songFiles,
			const QList<QUAbstractReportData*> &reportDataList,
			const QFileInfo &fi,
			QU::ReportOptions options = 0,
			const QVariant &userData = QVariant(),
			QObject *parent = 0);

	virtual QString content() const { return tr("PDF Report"); }
	virtual bool save();
};

#endif // QUPDFREPORT_H
