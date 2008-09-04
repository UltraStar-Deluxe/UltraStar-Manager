#ifndef QUPLAINTEXTREPORT_H_
#define QUPLAINTEXTREPORT_H_

#include "QUAbstractReport.h"

#include <QTextStream>

class QUPlainTextReport: public QUAbstractReport {
	Q_OBJECT

public:
	QUPlainTextReport(
			const QList<QUSongFile*> &songFiles,
			const QList<QUAbstractReportData*> &reportDataList,
			const QFileInfo &fi,
			QU::ReportOptions options = 0,
			const QVariant &userData = QVariant(),
			QObject *parent = 0);

	virtual QString content() const;

private:
	QString    _content;
	QList<int> _paddings;

	void computePaddings();
	void createContent();

	void printHeading(QTextStream &out);
	void printSongTable(QTextStream &out);
	void printLyrics(QTextStream &out);
};

#endif /*QUPLAINTEXTREPORT_H_*/
