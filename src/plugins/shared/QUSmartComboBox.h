#ifndef QUSMARTCOMBOBOX_H_H
#define QUSMARTCOMBOBOX_H_H

#include "QUSmartSetting.h"

#include <QComboBox>
#include <QStringList>

class QUSmartComboBox: public QUSmartSetting {
	Q_OBJECT

public:
	QUSmartComboBox(const QString &registryKey, const QString &text, const QStringList &values, int defaultValue = 0);
    ~QUSmartComboBox() override = default;

	virtual QWidget* editor() const { return _container; }

	virtual void resetToDefaults();
	virtual bool hasChanges() const;

public slots:
	void updateValue(int index);

private:
	int _defaultValue;
	QComboBox *_comboBox;
	QWidget *_container;
};

#endif // QUSMARTCOMBOBOX_H_H
