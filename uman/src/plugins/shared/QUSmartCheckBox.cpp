#include "QUSmartCheckBox.h"

#include <QHBoxLayout>
#include <QToolButton>

QUSmartCheckBox::QUSmartCheckBox(const QString &registryKey, const QString &text, bool isChecked):
	QUSmartSetting(registryKey)
{
	_defaultValue = isChecked;
	saveDefaultValue(_defaultValue);

	_checkBox = new QCheckBox(text);
	_checkBox->setChecked(value(isChecked).toBool());
	connect(_checkBox, SIGNAL(stateChanged(int)), this, SLOT(updateValue(int)));

	_container = new QWidget();

	QHBoxLayout *layout = new QHBoxLayout;

	layout->addWidget(_checkBox);
	layout->addSpacerItem(new QSpacerItem(20, 5, QSizePolicy::Expanding, QSizePolicy::Fixed));
	layout->addWidget(createResetButton());
	layout->setMargin(0);
	layout->setSpacing(2);

	_container->setLayout(layout);
}

void QUSmartCheckBox::updateValue(int state) {
	setValue(state == Qt::Checked ? true : false);
}

void QUSmartCheckBox::resetToDefaults() {
	_checkBox->setChecked(_defaultValue);
}

bool QUSmartCheckBox::hasChanges() const {
	return _defaultValue != _checkBox->isChecked();
}
