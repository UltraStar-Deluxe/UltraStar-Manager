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
	
	virtual QString data(QUSongFile *song);
	virtual QString headerData();
	
	// sorting
	virtual void sort(QList<QUSongFile*> &songs);
	
	static bool artistLessThan (QUSongFile *s1, QUSongFile *s2) { return QString::compare(s1->artist(), s2->artist(), Qt::CaseInsensitive) < 0; }
	static bool titleLessThan (QUSongFile *s1, QUSongFile *s2) { return QString::compare(s1->title(), s2->title(), Qt::CaseInsensitive) < 0; }
	
private:
	QString _tag;
};

#endif /*QUSONGTAGDATA_H_*/
