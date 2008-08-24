#ifndef QUSCRIPTABLETASK_H_
#define QUSCRIPTABLETASK_H_

#include "QU.h"
#include "QUAbstractTask.h"

#include <QDomDocument>
#include <QDomDocumentType>
#include <QStringList>

class QUScriptData: public QObject {
	Q_OBJECT

public:
	QUScriptData(QObject *parent = 0):
		QObject(parent),
		_keepSuffix(false),
		_keepUnknownTags(false) {}

	QString _source;
	QString _text;
	QString _if;
	QString _default;

	// for rename tasks reserved
	bool _keepSuffix;
	bool _keepUnknownTags;

	// for id3 tasks reserved
	QString _ignoreEmpty;
};

class QUScriptableTask: public QUAbstractTask {
	Q_OBJECT

public:
	QUScriptableTask(QDomDocument *taskConfig, QObject *parent = 0);
	~QUScriptableTask() { qDeleteAll(_data); _data.clear(); }

	virtual QString description() const { return tr(QUAbstractTask::description().toLocal8Bit().data()); }
	virtual QString toolTip() const { return tr(QUAbstractTask::toolTip().toLocal8Bit().data()); }

	QString configFileName() const { return _configFileName; }
	QString iconSource() const { return _iconSource; }
	int group() const { return _group; }
	QString target() const { return _target; }
	QString schema() const { return _schema; }
	QList<QUScriptData*> data() const { return _data; }

protected:
	QString _configFileName;
	QString _iconSource; // used to determine the source path of the tasks icon
	int _group; // used for exclusive tasks
	QString _target;
	QString _schema;
	QList<QUScriptData*> _data;
};

#endif /* QUSCRIPTABLETASK_H_ */
