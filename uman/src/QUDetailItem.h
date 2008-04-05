#ifndef QUDETAILITEM_H_
#define QUDETAILITEM_H_

#include <QString>
#include <QTableWidgetItem>
#include <QList>
#include "QUSongFile.h"

/*!
 * This class encapsulates a pointer to a QUSongFile object for a QTableWidget
 * instance which is used to present details of a song file to the user.
 */
class QUDetailItem: public QTableWidgetItem {	
public:
	QUDetailItem(const QString &tag, const QList<QUSongFile*> &songs);
	
	
	QString tag() const {return _tag;}
	
	const QList<QUSongFile*>& songs() const {return _songs;}
	
private:
	QString _tag;
	QList<QUSongFile*> _songs;
	
	void updateDefaultData(const QString &tag, QUSongFile *song);
	void updateText(const QString &tag, QUSongFile *song);
	
	void updateItemForMultipleSongs();
};

#endif /*QUDETAILITEM_H_*/
