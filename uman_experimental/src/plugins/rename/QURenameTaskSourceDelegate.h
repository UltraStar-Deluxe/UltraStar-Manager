#ifndef QURENAMETASKSOURCEDELEGATE_H
#define QURENAMETASKSOURCEDELEGATE_H

#include "QUTaskSourceDelegate.h"

class QURenameTaskSourceDelegate: public QUTaskSourceDelegate {
	Q_OBJECT

public:
	QURenameTaskSourceDelegate(QObject *parent = 0);

private:
	virtual void setItems(QComboBox *comboBox) const;
};

#endif // QURENAMETASKSOURCEDELEGATE_H
