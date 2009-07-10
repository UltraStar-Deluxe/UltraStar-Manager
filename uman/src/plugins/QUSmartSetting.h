#ifndef QUSMARTSETTING_H
#define QUSMARTSETTING_H

#include <QWidget>
#include <QVariant>
#include <QString>

class QUSmartSetting: public QObject {
	Q_OBJECT

public:
	QUSmartSetting(const QString &registryKey);

	virtual QWidget* editor() const = 0;

//	virtual void resetToDefaults() = 0;
//	virtual bool hasChanges() const = 0;

protected:
	void setValue(const QVariant &value);
	QVariant value(const QVariant &defaultValue) const;

private:
	QString _registryKey;
};

#endif // QUSMARTSETTING_H
