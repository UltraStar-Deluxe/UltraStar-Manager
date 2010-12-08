#ifndef QUCOLUMNACTION_H_
#define QUCOLUMNACTION_H_

#include <QAction>
#include <QVariant>

#include "QUSongItem.h"

class QUColumnAction: public QAction {
	Q_OBJECT

public:
	QUColumnAction(const QString &text, const QVariant &userData = QVariant(), QObject *parent = 0);

private slots:
	void toggleColumn(bool checked);

signals:
	void columnToggled(bool checked, int index);

};

#endif /* QUCOLUMNACTION_H_ */
