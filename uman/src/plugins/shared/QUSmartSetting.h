#ifndef QUSMARTSETTING_H
#define QUSMARTSETTING_H

#include <QWidget>
#include <QVariant>
#include <QString>

class QUSmartSetting: public QObject {
	Q_OBJECT

public:
	QUSmartSetting(const QString &registryKey);
	virtual ~QUSmartSetting();

	QWidget* editor() const { return _editor; }

//	virtual void resetToDefaults() = 0;
//	virtual bool hasChanges() const = 0;

protected:
	void setEditor(QWidget *editor);
	void setValue(const QVariant &value);
	QVariant value(const QVariant &defaultValue) const;

private:
	QWidget *_editor;
	QString _registryKey;
};

#endif // QUSMARTSETTING_H
