#ifndef QUAUTOCUE_H_
#define QUAUTOCUE_H_

#include "QU.h"
#include "QUSongFile.h"

#include <QTextBrowser>
#include <QTimer>
#include <QTime>

class QUCueInfo {
public:
	QUCueInfo(int t, int p, int w): time(t), pos(p), width(w) {}
	int time;
	int pos;
	int width;
};

class QUAutoCue: public QTextBrowser {
	Q_OBJECT

public:
	QUAutoCue(QWidget *parent = 0);

	void reset(const QList<QUSongLineInterface*> &lines, double bpm, double gap, double isRelative);

public slots:
	void play();
	void stop();

	void pause();
	void resume(double position);
	void seek(double position);

private slots:
	void update();

private:
	QTime _startTime;
	QTime _pauseTime;
	int _waitedMilliseconds;
	bool  _stopRequested;
	bool  _pauseRequested;

	QList<QUCueInfo> _cueList;
	int _cueListIndex;

	void mark(int pos, int width);
};

#endif /* QUAUTOCUE_H_ */
