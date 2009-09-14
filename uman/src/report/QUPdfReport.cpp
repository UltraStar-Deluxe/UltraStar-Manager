#include "QUPdfReport.h"

#include "QULogService.h"

#include <QPrinter>
#include <QPainter>

#include <QFont>
#include <QFontMetrics>

QUPdfReport::QUPdfReport(
		const QList<QUSongFile*> &songFiles,
		const QList<QUAbstractReportData*> &reportDataList,
		const QFileInfo &fi,
		QU::ReportOptions options,
		const QVariant &userData,
		QObject *parent): QUAbstractReport(songFiles, reportDataList, fi, options, userData, parent)
{
}

bool QUPdfReport::save() {

	QPrinter printer(QPrinter::HighResolution);
	printer.setOutputFormat(QPrinter::PdfFormat);
	printer.setOutputFileName(fileInfo().filePath());
	printer.setPaperSize(QPrinter::A4);
	printer.setPageMargins(10, 10, 10, 10, QPrinter::Millimeter);

	QPainter painter;
	painter.begin(&printer);

	QFont f("Verdana", 12, QFont::Bold, false);

	painter.setFont(f);
	painter.drawText(printer.pageRect(), "Hello, World!", Qt::AlignTop | Qt::AlignLeft);

//	printer.newPage();

	painter.end();


	logSrv->add(tr("Adding PDF report...done."), QU::Information);
	return true;
}
