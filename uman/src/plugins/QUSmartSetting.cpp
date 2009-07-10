#include "QUSmartSetting.h"

#include <QSettings>

QUSmartSetting::QUSmartSetting(const QString &registryKey): QObject() {
	_registryKey = registryKey;
}

void QUSmartSetting::setValue(const QVariant &value) {
	QSettings s;
	s.setValue(_registryKey, value);
}

QVariant QUSmartSetting::value(const QVariant &defaultValue) const {
	QSettings s;
	return s.value(_registryKey, defaultValue);
}
