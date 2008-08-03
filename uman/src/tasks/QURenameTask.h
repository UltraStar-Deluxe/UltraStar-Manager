#ifndef QURENAMETASK_H_
#define QURENAMETASK_H_

#include "QU.h"
#include "QUAbstractTask.h"

#include <QDomDocument>
#include <QDomDocumentType>
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
	virtual QString description() const { return tr(QUAbstractTask::description().toLocal8Bit().data()); }
	virtual QString toolTip() const { return tr(QUAbstractTask::toolTip().toLocal8Bit().data()); }

	QString configFileName() const { return _configFileName; }
	QString iconSource() const { return _iconSource; }
	int group() const { return _group; }
	QString target() const { return _target; }
	QString schema() const { return _schema; }
	QList<QURenameData*> data() const { return _data; }

private:
	QString _configFileName;
	QString _iconSource; // used to determine the source path of the tasks icon
	int _group; // used for exclusive tasks
	QString _target;
	QString _schema;
	QList<QURenameData*> _data;

	QStringList filterUnknownTags(const QString &text);
};

#endif /*QURENAMETASK_H_*/
