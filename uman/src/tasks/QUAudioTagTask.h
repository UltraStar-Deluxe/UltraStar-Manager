#ifndef QUAUDIOTAGTASK_H_
#define QUAUDIOTAGTASK_H_

#include "QU.h"
#include "QUScriptableTask.h"

#include "audioproperties.h"
#include "fileref.h"
#include "tag.h"
#include "tstring.h"

#include <QStringList>

/*!
 * Use the ID3 tag of an audio file for the change
 * of several song data (e.g. #ARTIST, #TITLE, #GENRE, ...).
 */
class QUAudioTagTask: public QUScriptableTask {
	Q_OBJECT

	Q_PROPERTY(QString artist READ artist)
	Q_PROPERTY(QString title READ title)
	Q_PROPERTY(QString genre READ genre)
	Q_PROPERTY(QString year READ year)

	Q_PROPERTY(QString target READ currentContent)
	Q_PROPERTY(QString targetfull READ currentContentAll)

public:
	QUAudioTagTask(QDomDocument *taskConfig, QObject *parent = 0);

	virtual void startOn(QUSongFile *song);

	static QStringList availableID3Sources();

	static QStringList availableTargets();
	static QStringList availableInfoTargets();
	static QStringList availableCustomTargets();

private:
	QUSongFile *_currentSong;

	TagLib::FileRef ref();

	QString artist();
	QString title();
	QString genre();
	QString year();

	QString currentContent();
	QString currentContentAll();
};

#endif /*QUAUDIOTAGTASK_H_*/
