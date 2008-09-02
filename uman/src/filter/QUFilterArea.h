#ifndef QUFILTERAREA_H_
#define QUFILTERAREA_H_

#include "QU.h"

#include <QWidget>

#include "ui_QUFilterArea.h"

class QUFilterArea: public QWidget, public Ui::QUFilterArea {
	Q_OBJECT

public:
	QUFilterArea(QWidget *parent);
};

#endif /* QUFILTERAREA_H_ */
