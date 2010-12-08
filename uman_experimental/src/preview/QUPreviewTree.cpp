#include "QUPreviewTree.h"
#include "QUSongFile.h"

#include "audioproperties.h"
#include "fileref.h"
#include "tag.h"
#include "tstring.h"

//extern "C" {
//	#include "avformat.h" /* ffmpeg */
//}

#include <QStringList>
#include <QHeaderView>
#include <QFont>
#include <QMessageBox>
#include <QImage>

#include "QUSongSupport.h"

QUPreviewTree::QUPreviewTree(QWidget *parent): QTreeWidget(parent) {
	this->setColumnCount(2);

	this->header()->hide();
	this->header()->setResizeMode(0, QHeaderView::ResizeToContents);
	this->header()->setResizeMode(1, QHeaderView::Stretch);

	this->setRootIsDecorated(false);
	this->setIndentation(10);

	// set up "general" toplevel item

	general = new QTreeWidgetItem();
	this->addTopLevelItem(general);

	general->setText(0, tr("Figures"));
	general->setFlags(Qt::ItemIsEnabled);
	general->setTextColor(0, Qt::darkGray);
	general->setFirstColumnSpanned(true);

	general->insertChild(SONG_COUNT_CHILD_INDEX, this->createInfoItem(tr("Database"), N_A));
	general->insertChild(HIDDEN_SONG_COUNT_CHILD_INDEX, this->createInfoItem(tr("Hidden"), N_A));
	general->insertChild(VISIBLE_SONG_COUNT_CHILD_INDEX, this->createInfoItem(tr("Visible"), N_A));

	generalSelected = this->createInfoItem(tr("Selection"), N_A);
	general->insertChild(SELECTED_SONG_COUNT_CHILD_INDEX, generalSelected);

	generalSelected->insertChild(SELECTED_SONG_LENGTH_INDEX, this->createInfoItem(tr("Song Length"), N_A));
	generalSelected->insertChild(SELECTED_AUDIO_LENGTH_INDEX, this->createInfoItem(tr("Audio Length"), N_A));
	generalSelected->insertChild(SELECTED_TOTAL_LENGTH_INDEX, this->createInfoItem(tr("Total Length"), N_A));

	general->setExpanded(true);

	// set up "types" toplevel item (totally static)

	types = new QTreeWidgetItem();
	this->addTopLevelItem(types);

	types->setText(0, tr("Supported File Types"));
	types->setFlags(Qt::ItemIsEnabled);
	types->setTextColor(0, Qt::darkGray);
	types->setFirstColumnSpanned(true);

	types->addChild(this->createInfoItem(QIcon(":/types/text.png"), tr("Song"), QUSongSupport::allowedSongFiles().join(" ")));
	types->addChild(this->createInfoItem(QIcon(":/types/music.png"), tr("Audio"), QUSongSupport::allowedAudioFiles().join(" ")));
	types->addChild(this->createInfoItem(QIcon(":/types/picture.png"), tr("Picture"), QUSongSupport::allowedPictureFiles().join(" ")));
	types->addChild(this->createInfoItem(QIcon(":/types/film.png"), tr("Video"), QUSongSupport::allowedVideoFiles().join(" ")));
	types->addChild(this->createInfoItem(QIcon(":/control/playlist.png"), tr("Playlist"), QUSongSupport::allowedPlaylistFiles().join(" ")));
	types->addChild(this->createInfoItem(QIcon(":/types/license.png"), tr("License"), QUSongSupport::allowedLicenseFiles().join(" ")));
	types->addChild(this->createInfoItem(QIcon(":/types/midi.png"), tr("MIDI"), QUSongSupport::allowedMidiFiles().join(" ")));
	types->addChild(this->createInfoItem(QIcon(":/types/midi_kar.png"), tr("Karaoke"),QUSongSupport::allowedKaraokeFiles().join(" ")));
	types->addChild(this->createInfoItem(QIcon(":/types/score.png"), tr("Score"), QUSongSupport::allowedScoreFiles().join(" ")));

	// set up "current" toplevel item

	current = new QTreeWidgetItem();
	this->addTopLevelItem(current);

	current->setText(0, tr("Current File"));
	current->setFlags(Qt::ItemIsEnabled);
	current->setTextColor(0, Qt::darkGray);
	current->setFirstColumnSpanned(true);

	current->setExpanded(true);
	current->setHidden(true);

	// set up "extra" toplevel item

	extra = new QTreeWidgetItem();
	this->addTopLevelItem(extra);

	extra->setText(0, tr("Extra Information"));
	extra->setFlags(Qt::ItemIsEnabled);
	extra->setTextColor(0, Qt::darkGray);
	extra->setFirstColumnSpanned(true);

	extra->setExpanded(true);
	extra->setHidden(true);
}

QTreeWidgetItem* QUPreviewTree::createInfoItem(const QString &tag, const QString &value) {
	QTreeWidgetItem *infoItem = new QTreeWidgetItem();

	infoItem->setText(0, tag);
	infoItem->setText(1, value);
	if(value != N_A) infoItem->setToolTip(1, value);

	QFont f(infoItem->font(0));
	f.setBold(true);
	infoItem->setFont(0, f);

	infoItem->setFlags(Qt::ItemIsEnabled);

	return infoItem;
}

QTreeWidgetItem* QUPreviewTree::createInfoItem(const QIcon &icon, const QString &tag, const QString &value) {
	QTreeWidgetItem *result = createInfoItem(tag, value);

	result->setIcon(0, icon);

	return result;
}

void QUPreviewTree::setSongCount(int songCount, int friendCount) {
	QTreeWidgetItem *child = general->child(SONG_COUNT_CHILD_INDEX);

	if(child)
		child->setText(1, tr("%1 (%2 songs, %3 friends)").arg(songCount + friendCount).arg(songCount).arg(friendCount));
}

void QUPreviewTree::setSelectedSongCount(int songCount, int friendCount) {
	QTreeWidgetItem *child = general->child(SELECTED_SONG_COUNT_CHILD_INDEX);

	if(child)
		child->setText(1, tr("%1 (%2 songs, %3 friends)").arg(songCount + friendCount).arg(songCount).arg(friendCount));
}

void QUPreviewTree::setHiddenSongCount(int songCount, int friendCount) {
	QTreeWidgetItem *child = general->child(HIDDEN_SONG_COUNT_CHILD_INDEX);

	if(child)
		child->setText(1, tr("%1 (%2 songs, %3 friends)").arg(songCount + friendCount).arg(songCount).arg(friendCount));
}

void QUPreviewTree::setVisibleSongCount(int songCount, int friendCount) {
	QTreeWidgetItem *child = general->child(VISIBLE_SONG_COUNT_CHILD_INDEX);

	if(child)
		child->setText(1, tr("%1 (%2 songs, %3 friends)").arg(songCount + friendCount).arg(songCount).arg(friendCount));
}

void QUPreviewTree::setSelectedSongLength(int seconds) {
	showSelectedLength( generalSelected->child(SELECTED_SONG_LENGTH_INDEX), seconds );
}

void QUPreviewTree::setSelectedAudioLength(int seconds) {
	showSelectedLength( generalSelected->child(SELECTED_AUDIO_LENGTH_INDEX), seconds );
}

void QUPreviewTree::setSelectedTotalLength(int seconds) {
	showSelectedLength( generalSelected->child(SELECTED_TOTAL_LENGTH_INDEX), seconds );
}

/*!
 * Show audio file information for now.
 */
void QUPreviewTree::showFileInformation(const QFileInfo &fi) {
	qDeleteAll(current->takeChildren());
	qDeleteAll(extra->takeChildren());

	current->setHidden(true);
	extra->setHidden(true);

	QString fileScheme("*." + fi.suffix());

	if(QUSongSupport::allowedAudioFiles().contains(fileScheme, Qt::CaseInsensitive))
		showAudioFileInformation(fi);
	else if(QUSongSupport::allowedPictureFiles().contains(fileScheme, Qt::CaseInsensitive))
		showPictureFileInformation(fi);
	else if(QUSongSupport::allowedVideoFiles().contains(fileScheme, Qt::CaseInsensitive))
		showVideoFileInformation(fi);
}

void QUPreviewTree::showAudioFileInformation(const QFileInfo &fi) {
	current->addChild(this->createInfoItem(tr("Filename"), fi.fileName()));
	current->addChild(this->createInfoItem(tr("Path"), fi.absolutePath()));
	current->addChild(this->createInfoItem(tr("Type"), tr("audio file")));
	current->addChild(this->createInfoItem(tr("Size"), QString("%1 MiB").arg(fi.size() / 1024. / 1024., 0, 'f', 2)));

	TagLib::FileRef ref(fi.absoluteFilePath().toLocal8Bit().data());

	if(!ref.isNull()) {
		QString artist(TStringToQString(ref.tag()->artist())); if(artist == "") artist = N_A;
		QString title(TStringToQString(ref.tag()->title())); if(title == "") title = N_A;
		QString album(TStringToQString(ref.tag()->album())); if(album == "") album = N_A;
		QString genre(TStringToQString(ref.tag()->genre())); if(genre == "") genre = N_A;
		QString year(QVariant(ref.tag()->year()).toString()); if(year == "0") year = N_A;
		QString track(QVariant(ref.tag()->track()).toString()); if(track == "0") track = N_A;

		extra->addChild(this->createInfoItem(tr("Artist"), artist));
		extra->addChild(this->createInfoItem(tr("Title"), title));
		extra->addChild(this->createInfoItem(tr("Album"), album));
		extra->addChild(this->createInfoItem(tr("Genre"), genre));
		extra->addChild(this->createInfoItem(tr("Year"), year));
		extra->addChild(this->createInfoItem(tr("Track"), track));

		TagLib::AudioProperties *prop = ref.audioProperties();

		if(prop) {
			extra->addChild(this->createInfoItem(tr("Length"), QString("%1:%2 min").arg(prop->length() / 60).arg(prop->length() % 60, 2, 10, QChar('0'))));
			extra->addChild(this->createInfoItem(tr("Bitrate"), QString("%1 kbit/s").arg(prop->bitrate())));
		}


		extra->setText(0, tr("Audio Properties"));
		extra->setHidden(false);
	}

	current->setHidden(false);
}

void QUPreviewTree::showPictureFileInformation(const QFileInfo &fi) {
	current->addChild(this->createInfoItem(tr("Filename"), fi.fileName()));
	current->addChild(this->createInfoItem(tr("Path"), fi.absolutePath()));
	current->addChild(this->createInfoItem(tr("Type"), tr("picture file")));
	current->addChild(this->createInfoItem(tr("Size"), QString("%1 KiB").arg(fi.size() / 1024., 0, 'f', 2)));

	QImage img(fi.filePath());
	extra->addChild(this->createInfoItem(tr("Dimensions"), QString("%1 x %2").arg(img.width()).arg(img.height())));
	extra->addChild(this->createInfoItem(tr("Depth"), QVariant(img.depth()).toString()));

	extra->setText(0, tr("Picture Properties"));
	extra->setHidden(false);

	current->setHidden(false);
}

void QUPreviewTree::showVideoFileInformation(const QFileInfo &fi) {
	current->addChild(this->createInfoItem(tr("Filename"), fi.fileName()));
	current->addChild(this->createInfoItem(tr("Path"), fi.absolutePath()));
	current->addChild(this->createInfoItem(tr("Type"), tr("video file")));
	current->addChild(this->createInfoItem(tr("Size"), QString("%1 MiB").arg(fi.size() / 1024. / 1024., 0, 'f', 2)));

	current->setHidden(false);

//	AVFormatContext *pFormatCtx;
//	const char      *filename=fi.filePath().toLocal8Bit().data();
//
//	// Open video file
//	if(av_open_input_file(&pFormatCtx, filename, NULL, 0, NULL)!=0) {
//		current->addChild(this->createInfoItem(tr("Error"), "Could not open file."));
//		return;
//	}
//
//	// Retrieve stream information
//	if(av_find_stream_info(pFormatCtx)<0) {
//		current->addChild(this->createInfoItem(tr("Error"), "Couldn't find stream information."));
//		return;
//	}
//
//	int            i, videoStream;
//	AVCodecContext *pCodecCtx;
//
//	// Find the first video stream
//	videoStream = -1;
//	for(i = 0; i < (int)pFormatCtx->nb_streams; i++)
//	    if(pFormatCtx->streams[i]->codec->codec_type == CODEC_TYPE_VIDEO) {
//	        videoStream = i;
//	        break;
//	    }
//
//	if(videoStream == -1) {
//		current->addChild(this->createInfoItem(tr("Error"), "Didn't find a video stream."));
//		return;
//	}
//
//	// Get a pointer to the codec context for the video stream
//	pCodecCtx = pFormatCtx->streams[videoStream]->codec;
//
//	AVCodec *pCodec;
//
//	// Find the decoder for the video stream
//	pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
//	if(pCodec == 0) {
//		current->addChild(this->createInfoItem(tr("Error"), "Codec not found."));
//		return;
//	}
//
//	// Inform the codec that we can handle truncated bitstreams -- i.e.,
//	// bitstreams where frame boundaries can fall in the middle of packets
//	if(pCodec->capabilities & CODEC_CAP_TRUNCATED)
//	    pCodecCtx->flags |= CODEC_FLAG_TRUNCATED;
//
//	// Open codec
//	if(avcodec_open(pCodecCtx, pCodec) < 0) {
//		current->addChild(this->createInfoItem(tr("Error"), "Could not open codec."));
//		return;
//	}
//
//	// Hack to correct wrong frame rates that seem to be generated by some
//	// codecs
////	if(pCodecCtx->frame_rate > 1000 && pCodecCtx->frame_rate_base == 1)
////	    pCodecCtx->frame_rate_base = 1000;
//
//	// Close the codec
//	avcodec_close(pCodecCtx);
//
//	// Close the video file
//	av_close_input_file(pFormatCtx);

	//	extra->setText(0, tr("Video Properties"));
	//	extra->setHidden(false);

}

void QUPreviewTree::showSelectedLength(QTreeWidgetItem *child, int seconds) {
	if(!child) return;

	generalSelected->setExpanded(seconds != -1);
	child->setText(1, seconds == -1 ? N_A : QString("%1:%2:%3").arg(seconds / 3600, 2, 10, QChar('0')).arg((seconds % 3600) / 60, 2, 10, QChar('0')).arg(seconds % 60, 2, 10, QChar('0')));
}
