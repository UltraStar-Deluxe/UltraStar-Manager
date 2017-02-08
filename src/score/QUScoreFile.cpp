#include "QUScoreFile.h"

#include <QStringList>

QUScoreFile::QUScoreFile(const QString &filePath, QObject *parent): QObject(0) {
	_score = new QSettings(filePath, QSettings::IniFormat);
}

QUScoreFile::~QUScoreFile() {
	if(_score)
		delete _score;
}

QMap<int, QString> QUScoreFile::scores(Difficulty diff) {
	QMap<int, QString> result;
	foreach(QString group, _score->childGroups()) {
		_score->beginGroup(group);
		if((diff == Easy && group.startsWith("Easy", Qt::CaseInsensitive))
			|| (diff == Normal && group.startsWith("Normal", Qt::CaseInsensitive))
			|| (diff == Hard && group.startsWith("Hard", Qt::CaseInsensitive)))
			result.insert(_score->value("Score", 0).toInt(), _score->value("Player", "").toString());
	}
	return result;
}
