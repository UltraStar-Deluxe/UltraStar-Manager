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
	QURenameTask(QU::RenameTaskModes mode, QObject *parent = 0);
	QURenameTask(QDomDocument *taskConfig, QObject *parent = 0);
	~QURenameTask() { qDeleteAll(_data); _data.clear(); }

	virtual void startOn(QUSongFile *song);

	QU::RenameTaskModes mode() const { return _mode; }

private:
	QU::RenameTaskModes _mode;

	void renameSongDirSpecial(QUSongFile *song);
	void renameSongVideoSpecial(QUSongFile *song);

	QStringList filterUnknownTags(const QString &text);

	QString _target;
	QString _schema;
	QList<QURenameData*> _data;
};

#endif /*QURENAMETASK_H_*/
