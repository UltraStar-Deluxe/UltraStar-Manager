#ifndef QUSMARTCHECKBOX_H
#define QUSMARTCHECKBOX_H

#include "QUSmartSetting.h"

#include <QCheckBox>
#include <QWidget>

class QUSmartCheckBox: public QUSmartSetting {
	Q_OBJECT

public:
	QUSmartCheckBox(const QString &registryKey, const QString &text, bool isChecked = true);

	virtual QWidget* editor() const { return _container; }

	virtual void resetToDefaults();
	virtual bool hasChanges() const;

public slots:
	void updateValue(int state);

private:
	bool _defaultValue;
	QCheckBox *_checkBox;
	QWidget *_container;
};

#endif // QUSMARTCHECKBOX_H
