#include "QUTextDialog.h"

#include <QFile>
#include <QStringList>
#include <QRegExp>
#include <QTextStream>
#include <QTextCodec>

QUTextDialog::QUTextDialog(QUSongFile *song, QWidget *parent, bool showLyrics): QDialog(parent) {
	setupUi(this);
	connect(doneBtn, SIGNAL(clicked()), this, SLOT(accept()));

	if(showLyrics)
		this->showLyrics(song);
	else
		this->showFile(song);
}

QUTextDialog::QUTextDialog(const QString &filePath, QWidget *parent): QDialog(parent) {
	setupUi(this);
	connect(doneBtn, SIGNAL(clicked()), this, SLOT(accept()));

	this->setWindowTitle(tr("Raw File Content"));
	textLbl->setText(filePath);

	QFile f(filePath);

	if(f.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QStringList lines(QString::fromLocal8Bit(f.readAll()).split("\n"));

		for(int i = 0; i < lines.size(); i++) { // format line numbers
			lines[i].prepend(QString("<font color=#808080>%1 </font>").arg(i + 1, 3, 10, QChar('0')));
		}

		QString content(lines.join("<br>"));

		textEdit->insertHtml(content);
		f.close();
	}

	textEdit->moveCursor(QTextCursor::Start);
}

void QUTextDialog::showLyrics(QUSongFile *song) {
	this->setWindowTitle(tr("Lyrics"));
	textLbl->setText(QString("%1 - %2").arg(song->artist()).arg(song->title()));

	this->initLyrics(song);
}

void QUTextDialog::showFile(QUSongFile *song) {
	this->setWindowTitle(song->songFileInfo().filePath());
	textLbl->setText(tr("The content of the song text file is <b>read-only</b>. Line numbers are auto-generated."));

	this->initFile(song);
}

/*!
 * Opens the song file and reads the content.
 */
void QUTextDialog::initFile(QUSongFile *song) {
	QFile file(song->songFileInfo().filePath());

	if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QTextStream in;
		in.setDevice(&file);

		if(song->encoding() == "UTF8") {
			in.setCodec(QTextCodec::codecForName("UTF-8"));
		} else if (song->encoding() == "CP1252") {
			in.setCodec(QTextCodec::codecForName("windows-1252"));
		} else if (song->encoding() == "CP1250") {
			in.setCodec(QTextCodec::codecForName("windows-1250"));
		} else {
			// should not occur
		}

		QStringList lines = in.readAll().split("\n");

		for(int i = 0; i < lines.size(); i++) { // format line numbers
			lines[i].prepend(QString("<font color=#808080>%1 </font>").arg(i + 1, 3, 10, QChar('0')));
		}

		QString content(lines.join("\n"));

		content.replace(" \n", "&nbsp;<br>");
		content.replace("\n", "<br>");
		content.replace(QRegExp("(#[^<]*:)"), "<b>\\1</b>");

		textEdit->insertHtml(content);
		file.close();
	}

	textEdit->moveCursor(QTextCursor::Start);
}

void QUTextDialog::initLyrics(QUSongFile *song) {
	QStringList lyrics = song->lyrics();

	for(int i = 0; i < lyrics.size(); i++) { // format line numbers
		lyrics[i].prepend(QString("<font color=#808080>%1 </font>").arg(i + 1, 3, 10, QChar('0')));
	}

	QString content = lyrics.join("<br>");
	textEdit->insertHtml(content);

	textEdit->moveCursor(QTextCursor::Start);
}
