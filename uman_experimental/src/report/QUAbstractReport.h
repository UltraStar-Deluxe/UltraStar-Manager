#ifndef QUABSTRACTREPORT_H_
#define QUABSTRACTREPORT_H_

#include "QU.h"
#include "QUSongFile.h"
#include "QUAbstractReportData.h"

#include <QObject>
#include <QString>
#include <QList>
#include <QFileInfo>
#include <QVariant>

class QUAbstractReport: public QObject {
	Q_OBJECT

public:
	QUAbstractReport(
			const QList<QUSongFile*> &songFiles,
			const QList<QUAbstractReportData*> &reportDataList,
			const QFileInfo &fi,
			QU::ReportOptions options = 0,
			const QVariant &userData = QVariant(),
			QObject *parent = 0);

	virtual QString content() const = 0;
	virtual bool save();

protected:
	QList<QUAbstractReportData*> reportDataList() const { return _reportDataList; }
	QList<QUSongFile*>           songs() const { return _songFiles; }
	QFileInfo                    fileInfo() const { return _fi; }
	QU::ReportOptions            options() const { return _options; }
	QVariant                     userData() const { return _userData; }

private:
	QList<QUAbstractReportData*> _reportDataList;
	QList<QUSongFile*>           _songFiles;
	QFileInfo                    _fi;
	QU::ReportOptions            _options;
	QVariant                     _userData;
};

#endif /*QUABSTRACTREPORT_H_*/
