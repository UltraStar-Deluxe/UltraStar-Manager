#include "QUAbstractReport.h"

#include <QFile>
#include <QTextStream>

QUAbstractReport::QUAbstractReport(
		const QList<QUSongFile*> &songFiles,
		const QList<QUAbstractReportData*> &reportDataList,
		const QFileInfo &fi,
		QU::ReportOptions options,
		const QVariant &userData,
		QObject *parent):
	QObject(parent),
	_reportDataList(reportDataList),
	_songFiles(songFiles),
	_fi(fi),
	_options(options),
	_userData(userData)
{
}

bool QUAbstractReport::save() {
	QFile file(_fi.filePath());

	if(file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
		QTextStream out(&file);
		out << this->content();
		file.close();
		return true;
	}
	return false;
}
