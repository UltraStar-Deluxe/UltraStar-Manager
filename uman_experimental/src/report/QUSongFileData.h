#ifndef QUSONGFILEDATA_H_
#define QUSONGFILEDATA_H_

#include "QUAbstractReportData.h"

/*!
 * Retrieve song file data through properties.
 */
class QUSongFileData: public QUAbstractReportData {
	Q_OBJECT

public:
	QUSongFileData(const QString &property, QObject *parent = 0);

	virtual QString textData(QUSongFile *song);
	virtual QString iconData(QUSongFile *song) { return QString(); }

	virtual QString headerTextData();
	virtual QString headerIconData() { return QString(); }

	virtual void sort(QList<QUSongFile*> &songs);

private:
	QString _property;
};

#endif /* QUSONGFILEDATA_H_ */
