#ifndef QURIBBONBAR_H
#define QURIBBONBAR_H

#include <QTabWidget>
#include <QSize>
#include <QList>

#include "ui_QURibbonBar.h"

class QEvent;

class QURibbonBar : public QTabWidget, public Ui::QURibbonBar {
	Q_OBJECT

public:
	QURibbonBar(QWidget *parent = 0);

	QToolButton *helpBtn;
	QToolButton *montyBtn;
	QToolButton *hideBtn;

	void setShortcut(QToolButton *w, const QKeySequence &key);

	virtual QSize sizeHint() const;
	bool menuHidden() const { return _menuHidden; }

public slots:
	void setMenuHidden(bool);
	void toggleMenuHidden();
	void changeCurrentTab(int);

protected:
	virtual bool eventFilter(QObject *target, QEvent *event);

	void useVisibleStyle();
	void useHiddenStyle();

private:
	bool _menuHidden;
};

#endif // QURIBBONBAR_H
