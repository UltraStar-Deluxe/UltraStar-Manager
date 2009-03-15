#ifndef QURIBBONBAR_H
#define QURIBBONBAR_H

#include <QTabWidget>

#include "ui_QURibbonBar.h"

class QURibbonBar : public QTabWidget, private Ui::QURibbonBar {
	Q_OBJECT

public:
	QURibbonBar(QWidget *parent = 0);
};

#endif // QURIBBONBAR_H
