#include "QUSmartInputCheck.h"

#include <QHBoxLayout>
#include <QToolButton>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QSettings>

QUSmartInputCheck::QUSmartInputCheck(const QString &registryKey, const QString &defaultValue, QValidator *validator, const QString &prefix):
	QUSmartSetting(registryKey)
{
	_defaultValue = defaultValue;
	saveDefaultValue(_defaultValue);

	_container = new QWidget();
	_group = new QWidget();

	_checkBox = new QCheckBox("");
	QSettings s;
	_checkBox->setChecked(s.value(this->registryKey() + "_check", false).toBool());
	_group->setEnabled(_checkBox->isChecked());
	connect(_checkBox, SIGNAL(stateChanged(int)), this, SLOT(updateCheckValue(int)));

	_edit = new QLineEdit(QUSmartSetting::value(_defaultValue).toString());
	connect(_edit, SIGNAL(textChanged(QString)), this, SLOT(updateTextValue(QString)));

	_edit->setValidator(validator);
	_edit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	_edit->setFixedHeight(_edit->fontMetrics().height() + 1);
	_edit->setFixedWidth(_edit->fontMetrics().width('x') * 10);

	QHBoxLayout *containerLayout = new QHBoxLayout;
	QHBoxLayout *groupLayout = new QHBoxLayout;

	groupLayout->addWidget(new QLabel(prefix));
	groupLayout->addWidget(_edit);
	groupLayout->addSpacerItem(new QSpacerItem(20, 5, QSizePolicy::Expanding, QSizePolicy::Fixed));
	groupLayout->addWidget(createResetButton());
	groupLayout->setMargin(0);
	groupLayout->setSpacing(2);

	containerLayout->addWidget(_checkBox);
	containerLayout->addWidget(_group);
	containerLayout->setMargin(0);
	containerLayout->setSpacing(0);

	_group->setLayout(groupLayout);
	_container->setLayout(containerLayout);
}

void QUSmartInputCheck::updateTextValue(QString newValue) {
	setValue(newValue);
}

void QUSmartInputCheck::updateCheckValue(int state) {
	QSettings s;
	s.setValue(registryKey() + "_check", state == Qt::Checked);
	_group->setEnabled(state == Qt::Checked);

	emit changed();
}

void QUSmartInputCheck::resetToDefaults() {
	_edit->setText(_defaultValue);
}

bool QUSmartInputCheck::hasChanges() const {
	return _checkBox->isChecked() || QString::compare(_edit->text(), _defaultValue) != 0;
}

/*!
 * Override to use checkbox for hiding the value.
 */
QVariant QUSmartInputCheck::value() const {
	if(_checkBox->isChecked())
		return QUSmartSetting::value();
	else
		return QVariant();
}
