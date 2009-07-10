#include "QUSmartCheckBox.h"

#include <QCheckBox>

QUSmartCheckBox::QUSmartCheckBox(const QString &registryKey, const QString &text, bool isChecked):
	QUSmartSetting(registryKey)
{
	_text = text;
	_isChecked = value(isChecked).toBool(); // registry value is more important
	setEditor(new QCheckBox(_text));
}
