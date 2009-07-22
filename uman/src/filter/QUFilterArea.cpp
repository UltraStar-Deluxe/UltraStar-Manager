#include "QUFilterArea.h"

QUFilterArea::QUFilterArea(QWidget *parent): QWidget(parent) {
	setupUi(this);

	_filterMenu = new QMenu(this);

	_invertedSearch = _filterMenu->addAction(tr("Inverted search"));
	_invertedSearch->setCheckable(true);
	_filterMenu->addSeparator();

	_tagGroup = new QActionGroup(this);
	_tagGroup->addAction(_filterMenu->addAction(tr("All tags")));
	_tagGroup->addAction(_filterMenu->addAction(tr("Information tags")));
	_tagGroup->addAction(_filterMenu->addAction(tr("File tags")));
	_tagGroup->addAction(_filterMenu->addAction(tr("Control tags")));
	_tagGroup->addAction(_filterMenu->addAction(tr("Custom tags")));

	foreach(QAction *a, tagGroupActions())
		a->setCheckable(true);

	this->tagGroupActions().at(0)->setChecked(true);

	filterBtn->setMenu(_filterMenu);

	connect(filterDuplicatesBtn, SIGNAL(clicked()), filterClearBtn, SLOT(show()));

	filterClearBtn->hide();
}
