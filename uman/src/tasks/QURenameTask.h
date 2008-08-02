#ifndef QURENAMETASK_H_
#define QURENAMETASK_H_

#include "QU.h"
#include "QUAbstractTask.h"

#include <QDomDocument>
#include <QStringList>

class QURenameData: public QObject {
	Q_OBJECT

public:
	QURenameData(QObject *parent = 0): QObject(parent), _keepSuffix(false), _keepUnknownTags(false) {}

	QString _source;
	QString _text;
	QString _if;
	QString _default;
	bool _keepSuffix;
	bool _keepUnknownTags;
};

class QURenameTask: public QUAbstractTask {
	Q_OBJECT

public:
	QURenameTask(QDomDocument *taskConfig, QObject *parent = 0);
	~QURenameTask() { qDeleteAll(_data); _data.clear(); }

	virtual void startOn(QUSongFile *song);

	int group() const { return _group; }

private:
	int _group; // used for exclusive tasks
	QString _target;
	QString _schema;
	QList<QURenameData*> _data;

	QStringList filterUnknownTags(const QString &text);
};

#endif /*QURENAMETASK_H_*/
