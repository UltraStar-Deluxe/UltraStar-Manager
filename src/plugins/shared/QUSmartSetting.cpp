#include "QUSmartSetting.h"

#include <QSettings>
#include <QToolButton>
#include <QIcon>

QUSmartSetting::QUSmartSetting(const QString &registryKey): QUSmartSettingInterface(0) {
	_registryKey = registryKey;
}

void QUSmartSetting::setValue(const QVariant &value) {
	QSettings s;
	s.setValue(_registryKey, value);
	emit changed();
}

QVariant QUSmartSetting::value(const QVariant &defaultValue) const {
	QSettings s;
	return s.value(_registryKey, defaultValue);
}

QVariant QUSmartSetting::value() const {
	return value(QVariant());
}

QToolButton* QUSmartSetting::createResetButton() {
	QToolButton *btn = new QToolButton(0);
	btn->setAutoRaise(true);
	btn->setIcon(QIcon(":/control/arrow_undo.png"));
	btn->setToolTip(tr("Reset to default value."));
	connect(btn, SIGNAL(clicked()), this, SLOT(reset()));
	return btn;
}

void QUSmartSetting::reset() {
	resetToDefaults();
}

/*!
 * Write the default value into the registry if no key exists.
 */
void QUSmartSetting::saveDefaultValue(const QVariant &defaultValue) {
	QSettings s;
	if(!s.contains(registryKey()))
		s.setValue(registryKey(), defaultValue);
}
