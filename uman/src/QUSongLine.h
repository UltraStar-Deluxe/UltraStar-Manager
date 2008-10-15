#ifndef QUSONGLINE_H_
#define QUSONGLINE_H_

#include <QObject>
#include <QString>

class QUSongNote: public QObject {
	Q_OBJECT

public:
	enum NoteType {
		normal,
		golden,
		freestyle,
	};
	Q_DECLARE_FLAGS(NoteTypes, NoteType)

	QUSongNote(NoteType ty, int t, int d, int p, const QString &lyric, QObject *parent = 0);

	int timestamp;
	int duration;
	int pitch;

	NoteType type;

	void setLyric(const QString &text);
	QString lyric() { return _lyric; }
	void resetTrailingSpaces(int prefixCount, int suffixCount);

private:
	QString _lyric;
};

// --------------------------------------------------------------------------

class QUSongLine: public QObject {
	Q_OBJECT

public:
	QUSongLine(QObject *parent = 0);
	~QUSongLine();

	void addNote(QUSongNote *newNote);
	QList<QUSongNote*> notes() { return _notes; }

	int outTime() const { return _out; }
	void setOutTime(int out) { _out = out; _useOutTime = true; }
	int inTime() const { return _in; }
	void setInTime(int in) { _in = in; _useInTime = true; }

	bool useOutTime() const { return _useOutTime; }
	bool useInTime() const { return _useInTime; }
	void removeInTime() { _useInTime = false; }

private:
	QList<QUSongNote*> _notes;

	int _out; // timestamp this line disappears
	int _in; // timestamp next line appears

	bool _useOutTime;
	bool _useInTime; // special line break
};

#endif /* QUSONGLINE_H_ */
