#ifndef QUBOOLEANSONGDATA_H_
#define QUBOOLEANSONGDATA_H_

#include "QUAbstractReportData.h"

class QUBooleanSongData : public QUAbstractReportData {
	Q_OBJECT
	
public:
	QUBooleanSongData(const QString &tag, QObject *parent = 0);
	
	virtual QString data(QUSongFile *song);
	virtual QString headerData();
	
	virtual void sort(QList<QUSongFile*> &songs) {} // cannot sort right now
	
private:
	QString _tag;
};

#endif /*QUBOOLEANSONGDATA_H_*/
