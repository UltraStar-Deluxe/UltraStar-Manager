#include "QURibbonBar.h"

#include <QToolButton>
#include <QIcon>
#include <QHBoxLayout>

QURibbonBar::QURibbonBar(QWidget *parent) {
	setupUi(this);

	QStringList styles;

	styles << "QTabWidget::pane {border-bottom: 1px solid #C2C7CB; border-top: 1px solid #C2C7CB; margin-bottom: 3px; background-color: white}";
	styles << "QTabBar::tab {background: transparent; min-width: 4em; color: #333333}";
	styles << "QTabWidget::tab-bar {left: 5px}";
	styles << "QTabBar::tab:selected {color: black; background: white; border-top-left-radius: 4px; border-top-right-radius: 4px; border: 1px solid #C4C4C3; border-bottom-style: none}";
	styles << "QGroupBox {padding: 2px 3px 6px 3px; margin: 0 2px 4px 2px; border: 0; font-size: 7pt; border-bottom: 1px solid #C2C7CB; border-left: 1px solid #C2C7CB; border-right: 1px solid #C2C7CB; border-bottom-left-radius: 4px; border-bottom-right-radius: 4px; color: #C2C7CB}";
	styles << "QGroupBox::title {subcontrol-position: bottom center; subcontrol-origin: margin}";
//	styles << "QToolButton {border: 2px solid red; padding: 0 10px 0 10px}";

	setStyleSheet(styles.join(" "));

	helpBtn = new QToolButton(this);
	helpBtn->setAutoRaise(true);
	helpBtn->setIcon(QIcon(":/marks/help.png"));
	helpBtn->setToolTip(tr("Show Monty now to help you."));

	montyBtn = new QToolButton(this);
	montyBtn->setAutoRaise(true);
	montyBtn->setCheckable(true);
	montyBtn->setToolTip(tr("Allow Monty to help you on its own."));

	montyBtn->setIcon(QIcon(":/monty/normal.png"));

	QWidget *corner = new QWidget(this);
	corner->setLayout(new QHBoxLayout(this));
	corner->layout()->addWidget(montyBtn);
	corner->layout()->addWidget(helpBtn);
	corner->layout()->setContentsMargins(0, 0, 0, 0);
	corner->layout()->setSpacing(0);

	setCornerWidget(corner, Qt::TopRightCorner);
}

void QURibbonBar::setShortcut(QToolButton *w, const QKeySequence &key) {
	w->setShortcut(key);
	w->setToolTip(QString("%1 (%2)").arg(w->toolTip()).arg(key.toString()));
}
