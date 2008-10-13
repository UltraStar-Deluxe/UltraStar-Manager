#include "QUSongFileData.h"

#include <QVariant>

QUSongFileData::QUSongFileData(const QString &property, QObject *parent): QUAbstractReportData(parent) {
	_property = property;

	if(QString::compare(_property, "path", Qt::CaseInsensitive) == 0) {
		this->setIcon(QIcon(":/types/folder_go.png"));
		this->setDescription(tr("Song Path"));
	} else if(QString::compare(_property, "filePath", Qt::CaseInsensitive) == 0) {
		this->setIcon(QIcon(":/control/folder_page.png"));
		this->setDescription(tr("Song File Path"));
	} else if(QString::compare(_property, "relativeFilePath", Qt::CaseInsensitive) == 0) {
		this->setIcon(QIcon(":/control/folder_realtive.png"));
		this->setDescription(tr("Relative Song File Path"));
	} else if(QString::compare(_property, "lengthTotal", Qt::CaseInsensitive) == 0) {
		this->setIcon(QIcon(":/types/time_eff.png"));
		this->setDescription(tr("Length"));
	} else if(QString::compare(_property, "speed", Qt::CaseInsensitive) == 0) {
		this->setIcon(QIcon(":/marks/speed_slow_turtle.png"));
		this->setDescription(tr("Speed"));
	}
}

QString QUSongFileData::textData(QUSongFile *song) {
	QString result = song->property(_property.toLocal8Bit().data()).toString();

	if(result.isEmpty())
		result = N_A;

	return result;
}

QString QUSongFileData::headerTextData() {
	return this->description();
}

void QUSongFileData::sort(QList<QUSongFile*> &songs) {
	if(this->next())
		this->next()->sort(songs);

	if(QString::compare(_property, "path", Qt::CaseInsensitive) == 0)
		qStableSort(songs.begin(), songs.end(), QUSongFile::pathLessThan);
	else if(QString::compare(_property, "filePath", Qt::CaseInsensitive) == 0)
		qStableSort(songs.begin(), songs.end(), QUSongFile::filePathLessThan);
	else if(QString::compare(_property, "relativeFilePath", Qt::CaseInsensitive) == 0)
		qStableSort(songs.begin(), songs.end(), QUSongFile::relativeFilePathLessThan);
}

