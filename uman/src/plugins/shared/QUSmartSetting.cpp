#include "QUSmartSetting.h"

#include <QSettings>

QUSmartSetting::QUSmartSetting(const QString &registryKey): QObject() {
	_registryKey = registryKey;
	_editor = 0;
}

QUSmartSetting::~QUSmartSetting() {
	if(_editor)
		delete _editor;
}


void QUSmartSetting::setEditor(QWidget *editor) {
	if(_editor)
		delete editor;
	_editor = editor;
}

void QUSmartSetting::setValue(const QVariant &value) {
	QSettings s;
	s.setValue(_registryKey, value);
}

QVariant QUSmartSetting::value(const QVariant &defaultValue) const {
	QSettings s;
	return s.value(_registryKey, defaultValue);
}
