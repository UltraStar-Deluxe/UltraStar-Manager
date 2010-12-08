#ifndef QUSMARTSETTINGINTERFACE_H
#define QUSMARTSETTINGINTERFACE_H

#include <QObject>
#include <QVariant>

class QWidget;

class QUSmartSettingInterface: public QObject {
	Q_OBJECT

public:
	QUSmartSettingInterface(QObject *parent = 0): QObject(parent) {}

	virtual QWidget* editor() const = 0;
	virtual bool hasChanges() const = 0;
	virtual QVariant value() const = 0;

signals:
	void changed();
};


#endif // QUSMARTSETTINGINTERFACE_H
