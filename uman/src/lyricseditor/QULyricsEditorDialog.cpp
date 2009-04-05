#include "QULyricsEditorDialog.h"

#include "QUSongLineDelegate.h"

QULyricsEditorDialog::QULyricsEditorDialog(QUSongFile *song, QWidget *parent): QDialog(parent) {
	setupUi(this);
	lyrics->setSong(song);

	connect(acceptBtn, SIGNAL(clicked()), this, SLOT(accept()));
	connect(rejectBtn, SIGNAL(clicked()), this, SLOT(reject()));
	connect(whitespaceChk, SIGNAL(stateChanged(int)), this, SLOT(toggleWhitespace(int)));

	textLbl->setText(tr("Edit each syllable of each line and <b>fix spelling</b> or <b>wrong whitespaces</b> manually."));
	songLbl->setText(QString("%1 - %2").arg(song->artist()).arg(song->title()));
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
