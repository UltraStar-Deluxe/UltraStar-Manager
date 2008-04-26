#include "QUAudioTagTask.h"

QUAudioTagTask::QUAudioTagTask(QU::AudioTagTaskModes mode, QObject *parent): 
	QUAbstractTask(parent),
	_mode(mode)
{
	switch(_mode) {
	case QU::useArtist:
		this->setIcon(QIcon(":/types/user.png"));
		this->setDescription(tr("Use ID3 tag for artist"));
		break;
	case QU::useTitle:
		this->setIcon(QIcon(":/types/font.png"));
		this->setDescription(tr("Use ID3 tag for title"));
		break;
	case QU::useGenre:
		this->setIcon(QIcon(":/types/genre.png"));
		this->setDescription(tr("Use ID3 tag for genre"));
		break;
	case QU::useYear:
		this->setIcon(QIcon(":/types/date.png"));
		this->setDescription(tr("Use ID3 tag for year"));
		break;
	}
}

void QUAudioTagTask::startOn(QUSongFile *song) {
	switch(_mode) {
	case QU::useArtist:
		song->useID3TagForArtist();
		break;
	case QU::useTitle:
		song->useID3TagForTitle();
		break;
	case QU::useGenre:
		song->useID3TagForGenre();
		break;
	case QU::useYear:
		song->useID3TagForYear();
		break;
	}
}
