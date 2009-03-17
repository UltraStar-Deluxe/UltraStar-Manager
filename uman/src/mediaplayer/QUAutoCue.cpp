#include "QUAutoCue.h"
#include "QULogService.h"

#include <QColor>
#include <QScrollBar>

QUAutoCue::QUAutoCue(QWidget *parent): QTextBrowser(parent), _stopRequested(false), _cueListIndex(-1) {
	setFontFamily("Tahoma");
	setFontPointSize(10);
}

/*!
 * Prepare autocue for new song.
 */
void QUAutoCue::reset(const QList<QUSongLine*> &lines, double bpm, double gap, double isRelative) {
	clear();
	_cueList.clear();
	_cueListIndex = -1;

	int time = 0; // absolute time after beginning (milliseconds)
	int pos = 0; // absolute cursor position in text/lyrics

	int relTime = qRound(gap);

	foreach(QUSongLine *line, lines) {
		int last = 0;
		foreach(QUSongNote *note, line->notes()) {
			if(note != line->notes().first()) {
				if(last == note->timestamp) // no space
					_cueList.pop_back();
			}
			last = note->timestamp + note->duration;

			if(isRelative)
				time = relTime + qRound((note->timestamp / bpm) * 60000);
			else
				time = qRound(gap) + qRound((note->timestamp / bpm) * 60000);

			if(note->type == QUSongNote::golden)
				setFontWeight(QFont::Bold);
			else if(note->type == QUSongNote::freestyle)
				setFontItalic(true);

			setAlignment(Qt::AlignHCenter);
			insertPlainText(note->lyric());

			setFontWeight(QFont::Normal);
			setFontItalic(false);

			_cueList << QUCueInfo(time, pos, note->lyric().size());

			pos += note->lyric().size();

			time += qRound((note->duration / bpm) * 60000);
			_cueList << QUCueInfo(time, pos, 0);
		}
		insertPlainText("\n"); pos++;

		if(isRelative) {
			relTime += qRound((line->inTime() / bpm) * 60000);
		}
	}

	QTextCursor tc = textCursor();
	tc.setPosition(0);
	setTextCursor(tc);
}

void QUAutoCue::play() {
	_startTime = QTime::currentTime();
	_cueListIndex = 0;
	_stopRequested = false;
	update();
}

void QUAutoCue::stop() {
	_stopRequested = true;
	clear();
}

/*!
 * Highlight the correct syllable. Handle scrolling and so on.
 */
void QUAutoCue::update() {
	int time = _startTime.msecsTo(QTime::currentTime());
	int maxDelay = 50; // milliseconds

	forever{
		if(_cueListIndex >= _cueList.size())
			return;

		if(_cueList.at(_cueListIndex).time < time) { // you are too late
			_cueListIndex++;
			continue;
		}

		int delay = _cueList.at(_cueListIndex).time - time;
		if(delay >= maxDelay) { // you are too early, come back later
			if(!_stopRequested)
				QTimer::singleShot(10, this, SLOT(update()));
			return;
		} else {
			break;
		}
	}

	mark(_cueList.at(_cueListIndex).pos, _cueList.at(_cueListIndex).width);
	_cueListIndex++;

	if(!_stopRequested)
		QTimer::singleShot(10, this, SLOT(update()));
}

void QUAutoCue::mark(int pos, int width) {
	QScrollBar *bar = verticalScrollBar();
	bar->setValue(bar->maximum());

	QTextCursor tc = textCursor();
	tc.setPosition(pos);
	tc.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, width); // make selection
	setTextCursor(tc);

//	setTextColor(Qt::blue); // apply color to selection

//	tc.setPosition(pos); // remove selection
//	setTextCursor(tc);
}
