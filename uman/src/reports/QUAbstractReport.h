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
	QUAbstractReport(
			const QList<QUSongFile*> &songFiles,
			const QList<QUAbstractReportData*> &reportDataList,
			const QFileInfo &fi,
			bool showBaseDir = false,
			QObject *parent = 0);

	virtual QString content() const = 0;
	void save();

protected:
	QList<QUAbstractReportData*> reportDataList() const { return _reportDataList; }
	QList<QUSongFile*>           songs() const { return _songFiles; }
	QFileInfo                    fileInfo() const { return _fi; }
	bool                         showBasePath() const { return _showBaseDir; }

private:
	QList<QUAbstractReportData*> _reportDataList;
	QList<QUSongFile*>           _songFiles;
	QFileInfo                    _fi;
	bool                         _showBaseDir;
};

#endif /*QUABSTRACTREPORT_H_*/
