#ifndef QUSONGTAGDATA_H_
#define QUSONGTAGDATA_H_

#include "QUAbstractReportData.h"

/*!
 * This report data is used to retrieve normal song tag values like from
 * #ARTIST or #TITLE. All data is text.
 */
class QUSongTagData: public QUAbstractReportData {
	Q_OBJECT
	
public:
	QUSongTagData(const QString &tag, QObject *parent = 0);
	
	virtual QString textData(QUSongFile *song);
	virtual QString iconData(QUSongFile *song) { return QString(); }
	
	virtual QString headerTextData();
	virtual QString headerIconData() { return QString(); }
	
	virtual void sort(QList<QUSongFile*> &songs);	
private:
	QString _tag;
};

#endif /*QUSONGTAGDATA_H_*/
