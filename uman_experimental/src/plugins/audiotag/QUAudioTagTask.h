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
	Q_PROPERTY(QString album READ album)
	Q_PROPERTY(QString genre READ genre)
	Q_PROPERTY(QString year READ year)
	Q_PROPERTY(QString track READ track)
	Q_PROPERTY(QString length READ length)
	Q_PROPERTY(QString bitrate READ bitrate)

	Q_PROPERTY(QString target READ currentContent)
	Q_PROPERTY(QString targetfull READ currentContentAll)

public:
	QUAudioTagTask(QDomDocument *taskConfig, QObject *parent = 0);

	virtual void startOn(QUSongInterface *song);

	virtual bool isConfigurable() const { return true; }
	virtual int configure(QWidget *parent = 0);

	static QStringList availableSources();
	static QStringList availableSpecialSources();
	static QStringList availableCommonSources();
	static QStringList availableSongSources(); // use data from other song tags, not id3 tag

	static QStringList availableTargets();
	static QStringList availableInfoTargets();
	static QStringList availableFileTargets();
	static QStringList availableCustomTargets();

private:
	QUSongInterface *_currentSong;

	TagLib::FileRef ref();

	QString artist();
	QString title();
	QString album();
	QString genre();
	QString year();
	QString track();
	QString length();
	QString bitrate();

	QString currentContent();
	QString currentContentAll();
};

#endif /*QUAUDIOTAGTASK_H_*/
