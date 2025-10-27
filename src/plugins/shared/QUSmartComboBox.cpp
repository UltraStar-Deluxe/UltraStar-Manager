#include "QUSmartComboBox.h"

#include <QHBoxLayout>
#include <QToolButton>
#include <QLabel>

QUSmartComboBox::QUSmartComboBox(const QString &registryKey, const QString &text, const QStringList &values, int defaultValue):
	QUSmartSetting(registryKey)
{
	_defaultValue = defaultValue;
	saveDefaultValue(_defaultValue);

	_comboBox = new QComboBox();
    _comboBox->addItems(values);
    _comboBox->setCurrentIndex(defaultValue);
	connect(_comboBox, &QComboBox::currentIndexChanged, this, &QUSmartComboBox::updateValue);

	_container = new QWidget();

	QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(new QLabel(text));
	layout->addWidget(_comboBox);
	layout->addSpacerItem(new QSpacerItem(20, 5, QSizePolicy::Expanding, QSizePolicy::Fixed));
	layout->addWidget(createResetButton());
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(2);

	_container->setLayout(layout);
}

void QUSmartComboBox::updateValue(int index) {
	setValue(index);
}

void QUSmartComboBox::resetToDefaults() {
	_comboBox->setCurrentIndex(_defaultValue);
}

bool QUSmartComboBox::hasChanges() const {
	return _defaultValue != _comboBox->currentIndex();
}
