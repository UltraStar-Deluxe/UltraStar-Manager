#ifndef QURIBBONBAR_H
#define QURIBBONBAR_H

#include <QTabWidget>
#include <QKeySequence>

#include "ui_QURibbonBar.h"

class QURibbonBar : public QTabWidget, public Ui::QURibbonBar {
	Q_OBJECT

public:
	QURibbonBar(QWidget *parent = 0);

	QToolButton *helpBtn;
	QToolButton *montyBtn;

	void setShortcut(QToolButton *w, const QKeySequence &key);
};

#endif // QURIBBONBAR_H
