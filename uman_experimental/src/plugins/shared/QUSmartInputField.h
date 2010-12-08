#ifndef QUSMARTINPUTFIELD_H
#define QUSMARTINPUTFIELD_H

#include "QUSmartSetting.h"

class QValidator;
class QLineEdit;

class QUSmartInputField: public QUSmartSetting {
	Q_OBJECT

public:
	QUSmartInputField(const QString &registryKey, const QString &defaultValue, QValidator *validator, const QString &prefix, const QString &suffix = QString());

	virtual QWidget* editor() const { return _container; }

	virtual void resetToDefaults();
	virtual bool hasChanges() const;

public slots:
	void updateValue(QString);

private:
	QWidget *_container;
	QLineEdit *_edit;
	QString _defaultValue;

	void initContainer(QValidator *validator, const QString &prefix, const QString &suffix);
};

#endif // QUSMARTINPUTFIELD_H
