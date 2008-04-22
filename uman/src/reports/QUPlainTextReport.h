#ifndef QUPLAINTEXTREPORT_H_
#define QUPLAINTEXTREPORT_H_

#include "QUAbstractReport.h"

class QUPlainTextReport: public QUAbstractReport {
	Q_OBJECT
	
public:
	QUPlainTextReport(const QList<QUSongFile*> &songFiles, const QList<QUAbstractReportData*> &reportDataList, const QFileInfo &fi, QObject *parent = 0);
	
	virtual QString content() const;
	
private:
	QString _content;
};

#endif /*QUPLAINTEXTREPORT_H_*/
