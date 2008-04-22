#ifndef QUABSTRACTREPORTDATA_H_
#define QUABSTRACTREPORTDATA_H_

#include <QObject>
#include <QIcon>

#include "QU.h"
#include "QUSongFile.h"

class QUAbstractReportData: public QObject {
	Q_OBJECT
	
public:
	QUAbstractReportData(QObject *parent = 0);
	
	virtual QString textData(QUSongFile *song) = 0;
	virtual QString iconData(QUSongFile *song) = 0;
	
	virtual QString headerTextData() = 0;
	virtual QString headerIconData() = 0;
	
	const QIcon& icon() const { return _icon; }
	const QString& description() const { return _description; }
	const QString& toolTip() const { return _toolTip; }
	
	virtual void sort(QList<QUSongFile*> &songs) = 0;
	
	QUAbstractReportData* next() const { return _next; }
	void setNext(QUAbstractReportData *next) { _next = next; }
	
private:
	QIcon   _icon;
	QString _description;
	QString _toolTip;
	QUAbstractReportData *_next;

protected:
	void setIcon(const QIcon &icon) { _icon = icon; }
	void setDescription(const QString &description) { _description = description; }
	void setToolTip(const QString &toolTip) { _toolTip = toolTip; }	
};

#endif /*QUABSTRACTREPORTDATA_H_*/
