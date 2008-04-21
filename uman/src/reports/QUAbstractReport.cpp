#include "QUAbstractReport.h"

#include <QFile>
#include <QTextStream>

QUAbstractReport::QUAbstractReport(
		const QList<QUSongFile*> &songFiles, 
		const QList<QUAbstractReportData*> &reportDataList,
		const QFileInfo &fi,
		QObject *parent): 
	QObject(parent),
	_reportDataList(reportDataList),
	_songFiles(songFiles),
	_fi(fi)
{
}
		
void QUAbstractReport::save() {
	QFile file(_fi.filePath());
	
	if(file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
		QTextStream out(&file);
		out << this->content();
		file.close();
	}
}
