#include "QUSongLine.h"

QUSongNote::QUSongNote(NoteType ty, int t, int d, int p, const QString &lyric, QObject *parent): QObject(parent) {
	type = ty;
	timestamp = t;
	duration = d;
	pitch = p;

	setLyric(lyric);
}

void QUSongNote::setLyric(const QString &text) {
	_lyric = text;
}

/*!
 * Reset the number of leading and following spacse.
 */
void QUSongNote::resetTrailingSpaces(int prefixCount, int suffixCount) {
	// the simple way
	if(prefixCount != -1 and suffixCount != -1) {
		_lyric = _lyric.trimmed();

		for(int i = 0; i < prefixCount; i++)
			_lyric.prepend(" ");
		for(int i = 0; i < suffixCount; i++)
			_lyric.append(" ");

		return;
	}

	// the hard way
	if(prefixCount == -1) {
		// prefix unchanged

		for(int i = 0; i < _lyric.length(); i++) {
			if(!_lyric.endsWith(" "))
				break;

			_lyric.remove(_lyric.length() - 1, 1);
		}

		for(int i = 0; i < suffixCount; i++)
			_lyric.append(" ");
	}

	if(suffixCount == -1) {
		// suffix unchanged

		for(int i = 0; i < _lyric.length(); i++) {
			if(!_lyric.startsWith(" "))
				break;

			_lyric.remove(0, 1);
		}

		for(int i = 0; i < prefixCount; i++)
			_lyric.prepend(" ");
	}
}

// --------------------------------------------------------------------------------------

QUSongLine::QUSongLine(QObject *parent): QObject(parent) {
	_out = 0;
	_in = 0;

	_useOutTime = false;
	_useInTime = false;
}

QUSongLine::~QUSongLine() {
	qDeleteAll(_notes);
	_notes.clear();
}

void QUSongLine::addNote(QUSongNote *newNote) {
	if(!newNote)
		return;

	_notes.append(newNote);
}
