#ifndef QUSCOREFILE_H
#define QUSCOREFILE_H

#include <QObject>
#include <QString>
#include <QSettings>
#include <QMap>

class QUScoreFile: public QObject {
	Q_OBJECT

public:
	enum Difficulty {
		Easy,
		Normal,
		Hard
	};
	Q_DECLARE_FLAGS(Difficulties, Difficulty)

	QUScoreFile(const QString &filePath, QObject *parent = 0);
	~QUScoreFile();

	QString filePath() const { if(_score) return _score->fileName(); else return ""; }

	QMap<int, QString> scores(Difficulty);

private:
	QSettings *_score;
};

#endif // QUSCOREFILE_H
