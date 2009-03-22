#include "QUAutoCue.h"
#include "QULogService.h"

#include <QColor>
#include <QScrollBar>

QUAutoCue::QUAutoCue(QWidget *parent):
		QTextBrowser(parent),
		_waitedMilliseconds(0),
		_stopRequested(false),
		_pauseRequested(false),
		_cueListIndex(-1)
{
	setFontFamily("Tahoma");
	setFontPointSize(10);
}

/*!
 * Prepare autocue for new song.
 */
void QUAutoCue::reset(const QList<QUSongLineInterface*> &lines, double bpm, double gap, double isRelative) {
	clear();
	_cueList.clear();
	_cueListIndex = -1;

	int time = 0; // absolute time after beginning (milliseconds)
	int pos = 0; // absolute cursor position in text/lyrics

	int relTime = qRound(gap);

	foreach(QUSongLineInterface *line, lines) {
		int last = 0;
		foreach(QUSongNoteInterface *note, line->notes()) {
			if(note != line->notes().first()) {
				if(last == note->timestamp()) // no space
					_cueList.pop_back();
			}
			last = note->timestamp() + note->duration();

			if(isRelative)
				time = relTime + qRound((note->timestamp() / bpm) * 60000);
			else
				time = qRound(gap) + qRound((note->timestamp() / bpm) * 60000);

			if(note->type() == QUSongNoteInterface::golden)
				setFontWeight(QFont::Bold);
			else if(note->type() == QUSongNoteInterface::freestyle)
				setFontItalic(true);

			setAlignment(Qt::AlignHCenter);
			insertPlainText(note->syllable());

			setFontWeight(QFont::Normal);
			setFontItalic(false);

			_cueList << QUCueInfo(time, pos, note->syllable().size());

			pos += note->syllable().size();

			time += qRound((note->duration() / bpm) * 60000);
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
	_startTime.start();
	_waitedMilliseconds = 0;
	_cueListIndex = 0;
	_stopRequested = false;
	_pauseRequested = false;
	update();
}

void QUAutoCue::stop() {
	_stopRequested = true;
	_pauseRequested = false;
	clear();
}

void QUAutoCue::pause() {
	_pauseTime = QTime::currentTime();
	_pauseRequested = true;
	_stopRequested = false;
}

void QUAutoCue::resume(double position) {
	qDebug(QVariant(position).toByteArray().data());

	if(_cueList.isEmpty())
		return;

	_cueListIndex = 0;
	int targetTime = (int)(position * 1000);

	while(_cueListIndex < _cueList.size() && _cueList.at(_cueListIndex).time < targetTime)
		_cueListIndex++;

	if(_cueListIndex >= _cueList.size())
		return;

	_waitedMilliseconds += _pauseTime.elapsed();

	_pauseRequested = false;
	_stopRequested = false;
	update();
}

/*!
 * Highlight the correct syllable. Handle scrolling and so on.
 */
void QUAutoCue::update() {
	int time = _startTime.elapsed() - _waitedMilliseconds;
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

	if(!_stopRequested && !_pauseRequested)
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
