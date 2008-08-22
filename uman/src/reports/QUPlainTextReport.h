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
			bool showBaseDir = false,
			QObject *parent = 0);

	virtual QString content() const;

private:
	QString    _content;
	QList<int> _paddings;

	void computePaddings();
	void createContent();

	void printHeading(QTextStream &out);
	void printSongTable(QTextStream &out);
};

#endif /*QUPLAINTEXTREPORT_H_*/
