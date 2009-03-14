#ifndef QUAUDIOTAGTASKSOURCEDELEGATE_H
#define QUAUDIOTAGTASKSOURCEDELEGATE_H

#include "QUTaskSourceDelegate.h"

class QUAudioTagTaskSourceDelegate: public QUTaskSourceDelegate {
	Q_OBJECT

public:
	QUAudioTagTaskSourceDelegate(QObject *parent = 0);

private:
	virtual void setItems(QComboBox *comboBox) const;
};

#endif // QUAUDIOTAGTASKSOURCEDELEGATE_H
