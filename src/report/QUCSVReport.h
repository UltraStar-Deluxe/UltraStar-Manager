#ifndef QUCSVREPORT_H_
#define QUCSVREPORT_H_

#include "QUAbstractReport.h"

#include <QTextStream>

class QUCSVReport: public QUAbstractReport {
	Q_OBJECT

public:
	QUCSVReport(
			const QList<QUSongFile*> &songFiles,
			const QList<QUAbstractReportData*> &reportDataList,
			const QFileInfo &fi,
			QU::ReportOptions options,
			const QVariant &userData = QVariant(),
			QObject *parent = nullptr);

	virtual QString content() const;

private:
	QString _content;

	void createContent();

	void printHeading(QTextStream &out);
	void printSongTable(QTextStream &out);
};

#endif /*QUCSVREPORT_H_*/
