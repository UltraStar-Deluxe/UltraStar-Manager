#ifndef QUABSTRACTTASK_H_
#define QUABSTRACTTASK_H_

#include <QObject>
#include <QIcon>

#include "QU.h"
#include "QUSongFile.h"

class QUAbstractTask: public QObject {
	Q_OBJECT
	
public:
	QUAbstractTask(QObject *parent = 0);
	
	virtual void startOn(QUSongFile *song) = 0;
	
	const QIcon& icon() const { return _icon; }
	const QString& description() const { return _description; }
	const QString& toolTip() const { return _toolTip; }
	
private:
	QIcon   _icon;
	QString _description;
	QString _toolTip;

protected:
	void setIcon(const QIcon &icon) { _icon = icon; }
	void setDescription(const QString &description) { _description = description; }
	void setToolTip(const QString &toolTip) { _toolTip = toolTip; }
};

#endif /*QUABSTRACTTASK_H_*/
