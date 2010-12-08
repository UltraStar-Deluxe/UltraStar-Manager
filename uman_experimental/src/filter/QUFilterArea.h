#ifndef QUFILTERAREA_H_
#define QUFILTERAREA_H_

#include "QU.h"

#include <QWidget>
#include <QMenu>
#include <QActionGroup>
#include <QList>


#include "ui_QUFilterArea.h"

class QUFilterArea: public QWidget, public Ui::QUFilterArea {
	Q_OBJECT

public:
	QUFilterArea(QWidget *parent);

	QList<QAction*> tagGroupActions() const { return _tagGroup->actions(); }
	bool doInvertedSearch() const { return _invertedSearch->isChecked(); }

private:
	QMenu *_filterMenu;
	QActionGroup *_tagGroup;
	QAction      *_invertedSearch;
};

#endif /* QUFILTERAREA_H_ */
