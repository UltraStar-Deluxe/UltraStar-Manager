#ifndef QUSMARTCHECKBOX_H
#define QUSMARTCHECKBOX_H

#include "QUSmartSetting.h"

class QUSmartCheckBox: public QUSmartSetting {
	Q_OBJECT

public:
	QUSmartCheckBox(const QString &registryKey, const QString &text, bool isChecked = true);

private:
	QString _text;
	bool _isChecked;
};

#endif // QUSMARTCHECKBOX_H
