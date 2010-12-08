#include "QUSmartInputField.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpacerItem>
#include <QFontMetrics>
#include <QValidator>
#include <QToolButton>

QUSmartInputField::QUSmartInputField(const QString &registryKey, const QString &defaultValue, QValidator *validator, const QString &prefix, const QString &suffix):
	QUSmartSetting(registryKey)
{
	_defaultValue = defaultValue;
	saveDefaultValue(_defaultValue);

	initContainer(validator, prefix, suffix);
}

void QUSmartInputField::initContainer(QValidator *validator, const QString &prefix, const QString &suffix) {
	_container = new QWidget();

	QHBoxLayout *layout = new QHBoxLayout;

	_edit = new QLineEdit(value(_defaultValue).toString());
	connect(_edit, SIGNAL(textChanged(QString)), this, SLOT(updateValue(QString)));

	_edit->setValidator(validator);
	_edit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	_edit->setFixedHeight(_edit->fontMetrics().height() + 1);
	_edit->setFixedWidth(_edit->fontMetrics().width('x') * 10);

	layout->addWidget(new QLabel(prefix));
	layout->addWidget(_edit);
	if(!suffix.isEmpty())
		layout->addWidget(new QLabel(suffix));
	layout->addSpacerItem(new QSpacerItem(20, 5, QSizePolicy::Expanding, QSizePolicy::Fixed));
	layout->addWidget(createResetButton());
	layout->setMargin(0);
	layout->setSpacing(2);

	_container->setLayout(layout);
}

void QUSmartInputField::updateValue(QString newValue) {
	setValue(newValue);
}

void QUSmartInputField::resetToDefaults() {
	_edit->setText(_defaultValue);
}

bool QUSmartInputField::hasChanges() const {
	return QString::compare(_edit->text(), _defaultValue) != 0;
}
