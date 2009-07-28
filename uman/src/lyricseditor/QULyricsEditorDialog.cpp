#include "QULyricsEditorDialog.h"
#include "QUSongLineDelegate.h"

#include <QRegExp>

QULyricsEditorDialog::QULyricsEditorDialog(QUSongFile *song, QWidget *parent): QDialog(parent) {
	setupUi(this);
	lyrics->setSong(song);

	connect(acceptBtn, SIGNAL(clicked()), this, SLOT(accept()));
	connect(rejectBtn, SIGNAL(clicked()), this, SLOT(reject()));
	connect(whitespaceChk, SIGNAL(stateChanged(int)), this, SLOT(toggleWhitespace(int)));
	connect(searchEdit, SIGNAL(textEdited(QString)), this, SLOT(search(QString)));

	textLbl->setText(tr("Edit each syllable of each line and <b>fix spelling</b> or <b>wrong whitespaces</b> manually."));
	this->setWindowTitle(QString("Edit Lyrics: \"%1 - %2\"").arg(song->artist()).arg(song->title()));
	songLbl->setText(song->songFileInfo().fileName());
}

/*!
 * Start with editing one specific line.
 */
int QULyricsEditorDialog::execAt(int line) {
	if(line >= 0 && line < lyrics->rowCount()) {
		QTableWidgetItem *item = lyrics->item(line, 1);
		lyrics->scrollToItem(item, QAbstractItemView::PositionAtCenter);
		lyrics->setCurrentItem(item);
		lyrics->editItem(item);
	}

	return exec();
}

void QULyricsEditorDialog::accept() {
	lyrics->song()->saveMelody();

	QDialog::accept();
}

void QULyricsEditorDialog::reject() {
	lyrics->song()->clearMelody();

	QDialog::reject();
}

void QULyricsEditorDialog::toggleWhitespace(int state) {
	lyrics->setShowWhitespace(state == Qt::Checked);
}

void QULyricsEditorDialog::search(const QString &keyword) {
	for(int row = 0; row < lyrics->rowCount(); row++) {
		if( !keyword.isEmpty() && lyrics->item(row, 1)->text().contains(QRegExp(keyword, Qt::CaseInsensitive))) {
			lyrics->item(row, 1)->setBackgroundColor(Qt::yellow);
		} else {
			lyrics->item(row, 1)->setBackgroundColor(Qt::white);
		}
	}
}
