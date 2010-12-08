#ifndef QUBOOLEANSONGDATA_H_
#define QUBOOLEANSONGDATA_H_

#include "QUAbstractReportData.h"

class QUBooleanSongData : public QUAbstractReportData {
	Q_OBJECT
	
public:
	QUBooleanSongData(const QString &tag, QObject *parent = 0);
	
	virtual QString textData(QUSongFile *song);
	virtual QString iconData(QUSongFile *song);
	
	virtual QString headerTextData();
	virtual QString headerIconData();
	
	virtual void sort(QList<QUSongFile*> &songs);
	
private:
	QString _tag;
};

#endif /*QUBOOLEANSONGDATA_H_*/
