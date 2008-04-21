#ifndef QUABSTRACTREPORT_H_
#define QUABSTRACTREPORT_H_

#include "QUAbstractReportData.h"
#include "QUSongFile.h"

#include <QObject>
#include <QString>
#include <QList>
#include <QFileInfo>

class QUAbstractReport: public QObject {
	Q_OBJECT
	
public:
	QUAbstractReport(const QList<QUSongFile*> &songFiles, const QList<QUAbstractReportData*> &reportDataList, const QFileInfo &fi, QObject *parent = 0);
	
	virtual QString content() const = 0;
	void save();
	
protected:
	QList<QUAbstractReportData*> _reportDataList;
	QList<QUSongFile*> _songFiles;
	QFileInfo _fi;
};

#endif /*QUABSTRACTREPORT_H_*/
