#include "QUTextDialog.h"

#include <QFile>
#include <QStringList>
#include <QRegExp>

QUTextDialog::QUTextDialog(QUSongFile *song, QWidget *parent): QDialog(parent) {
	setupUi(this);
	connect(doneBtn, SIGNAL(clicked()), this, SLOT(accept()));
	
	this->setWindowTitle(song->songFileInfo().filePath());
	this->initContent(song);
	
	textLbl->setText(tr("The content of the song text file is <b>read-only</b>. Line numbers auto-generated."));
}

/*!
 * Opens the song file and reads the content.
 */
void QUTextDialog::initContent(QUSongFile *song) {
	QFile f(song->songFileInfo().filePath());
	
	if(f.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QStringList lines(QString(f.readAll()).split("\n"));
		
		for(int i = 0; i < lines.size(); i++) { // format line numbers
			lines[i].prepend(QString("<font color=#808080>%1 </font>").arg(i + 1, 3, 10, QChar('0')));
		}
		
		QString content(lines.join("\n"));
		
		content.replace("\n", "<br>");
		content.replace(QRegExp("(#[^<]*:)"), "<b>\\1</b>");
						
		textEdit->insertHtml(content);
		f.close();
	}	
	
	textEdit->moveCursor(QTextCursor::Start);
}
