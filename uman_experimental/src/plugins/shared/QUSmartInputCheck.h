#ifndef QUSMARTINPUTCHECK_H
#define QUSMARTINPUTCHECK_H

#include "QUSmartSetting.h"

class QWidget;
class QCheckBox;
class QLineEdit;
class QValidator;

class QUSmartInputCheck: public QUSmartSetting {
	Q_OBJECT

public:
	QUSmartInputCheck(const QString &registryKey, const QString &defaultValue, QValidator *validator, const QString &prefix);

	virtual QWidget* editor() const { return _container; }

	virtual void resetToDefaults();
	virtual bool hasChanges() const;

	virtual QVariant value() const;

public slots:
	void updateTextValue(QString);
	void updateCheckValue(int);

private:
	QString _defaultValue;
	QWidget *_container;
	QWidget *_group;
	QCheckBox *_checkBox;
	QLineEdit *_edit;
};

#endif // QUSMARTINPUTCHECK_H
