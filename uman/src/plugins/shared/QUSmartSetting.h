#ifndef QUSMARTSETTING_H
#define QUSMARTSETTING_H

#include "QUSmartSettingInterface.h"

#include <QObject>
#include <QWidget>
#include <QVariant>
#include <QString>

class QToolButton;

class QUSmartSetting: public QUSmartSettingInterface {
	Q_OBJECT

public:
	QUSmartSetting(const QString &registryKey);

	virtual void resetToDefaults() = 0;
	virtual QVariant value() const;

public slots:
	void reset();

protected:
	void setValue(const QVariant &value);
	QVariant value(const QVariant &defaultValue) const;
	QToolButton* createResetButton();
	QString registryKey() const { return _registryKey; }
	void saveDefaultValue(const QVariant &defaultValue);

private:
	QString _registryKey;
};

#endif // QUSMARTSETTING_H
