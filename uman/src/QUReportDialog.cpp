#include "QUReportDialog.h"

#include <QFileDialog>
#include <QSettings>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QPixmap>

#include <QUrl>
#include <QDesktopServices>

QUReportDialog::QUReportDialog(QUSongTree *songTree, QWidget *parent): QDialog(parent), _songTree(songTree) {
	setupUi(this);
	
	if(songTree->hasHiddenItems())
		infoTextLbl->setText(tr("You applied a filter to your songs. The report will only be created for the songs that match the filter."));
	else
		infoTextLbl->setText(tr("Select the columns you want to see in the report. Drag & drop them to change their order. Songs will be sorted after the first column."));

	if(songTree->topLevelItemCount() == 0) {
		infoTextLbl->setText(tr("The report will be empty because no song matches the filter."));
		infoIconLbl->setPixmap(QPixmap(":/marks/error.png"));
	}
	
	connect(doneBtn, SIGNAL(clicked()), this, SLOT(accept()));
	connect(createHtmlBtn, SIGNAL(clicked()), this, SLOT(createHtmlReport()));
	
	initReportList();
}

void QUReportDialog::initReportList() {
	reportList->addItem(new QUReportItem(new QUSongTagData(ARTIST_TAG), Qt::Checked));
	reportList->addItem(new QUReportItem(new QUSongTagData(TITLE_TAG), Qt::Checked));
	reportList->addItem(new QUReportItem(new QUSongTagData(LANGUAGE_TAG)));
	reportList->addItem(new QUReportItem(new QUSongTagData(EDITION_TAG)));
	reportList->addItem(new QUReportItem(new QUSongTagData(GENRE_TAG)));
	reportList->addItem(new QUReportItem(new QUSongTagData(YEAR_TAG)));
	reportList->addItem(new QUReportItem(new QUSongTagData(CREATOR_TAG)));
}

/*!
 * Extract a resource (e.g. an image/icon) to a subdirectory of the report output
 * path and return a relative path to that resource.
 * \param item The resource to extract (e.g. ":/accept.png")
 * \param dest Directory of the report, NOT the subdirectory for the image
 * \returns Relative local path to the resource.
 */
QString QUReportDialog::useImageFromResource(const QString &item, QDir dest) {
	if(!dest.cd("images")) {
		dest.mkdir("images");
		if(!dest.cd("images")) {
			emit finished(tr("Subdirectory for images could not be created!"), QU::warning);
			return QString();
		}
	}
	
	QPixmap pixmap(item);
	QFileInfo fi(dest, QFileInfo(item).fileName());
	
	if(fi.exists()) {
		dest.cdUp();
		return dest.relativeFilePath(fi.filePath());		
	}
	
	if(!pixmap.save(fi.filePath())) {
		emit finished(QString(tr("The resource file \"%1\" could NOT be saved.")).arg(fi.filePath()), QU::warning);
		return QString();
	}
	
	emit finished(QString(tr("The resource file \"%1\" was extracted successfully.")).arg(fi.filePath()), QU::information);
	
	dest.cdUp();
	return dest.relativeFilePath(fi.filePath());
}

void QUReportDialog::createHtmlReport() {
	QSettings settings;
	QFileInfo fi(QDir(settings.value("reportPath").toString()), "report.html");

	fi.setFile(QFileDialog::getSaveFileName(this, tr("Save Report"),
		fi.filePath(),
		tr("Website (*.htm *.html)")));
	
	if(!fi.fileName().isEmpty()) {
		settings.setValue("reportPath", QVariant(fi.path())); // remember folder
		
		QFile file(fi.filePath());
		
		if(file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
			QTextStream out(&file);
			
			out << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2 Final//EN\">" << endl;
			out << "<html><head>" << endl;
			out << "<title>UltraStar Manager - Song Report</title>" << endl;
			out << "</head><body>" << endl;
			
			out << "<table>" << endl;
			
			// draw table heading
			out << "<tr>" << endl;
			for(int i = 0; i < reportList->count(); i++) {
				QUReportItem *item = dynamic_cast<QUReportItem*>(reportList->item(i));
				
				if(!item || item->checkState() == Qt::Unchecked)
					continue;
				
				out << "<th>" << item->data()->headerData() << "</th>" << endl;
			}
			out << "</tr>" << endl;
			
			// draw table contents
			// TODO: sort items after the first (report) column
			for(int i = 0; i < _songTree->topLevelItemCount(); i++) {
				QUSongItem *songItem = dynamic_cast<QUSongItem*>(_songTree->topLevelItem(i));
				
				if(!songItem)
					continue;
				
				out << "<tr>";
				
				for(int j = 0; j < reportList->count(); j++) {
					QUReportItem *reportItem = dynamic_cast<QUReportItem*>(reportList->item(j));
					
					if(!reportItem || reportItem->checkState() == Qt::Unchecked)
						continue;
					
					out << "<td>" << reportItem->data()->data(songItem->song()) << "</td>";
				}
				
				out << "</tr>" << endl;
			}
			
			// test
			// TODO: implement classes for icon data (e.g. has video)
			out << "<tr><td><img src=\"" << this->useImageFromResource(":/marks/tick.png", fi.dir()) << "\"/></td></tr>" << endl;
			
			out << "</table>" << endl;
			
			out << "</body></html>" << endl;
			
			file.close();
			emit finished(QString(tr("Report created successfully to: \"%1\".")).arg(fi.filePath()), QU::information);
			
			
			QDesktopServices::openUrl(QUrl::fromLocalFile(fi.filePath()));
		} else {
			emit finished(QString(tr("Could not write to: \"%1\".")).arg(fi.filePath()), QU::warning);
		}
	} else {
		emit finished(tr("Report could not be created."), QU::warning);
	}
}
