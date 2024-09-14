#include "QUPreviewTree.h"
#include "QUSongFile.h"

#include "audioproperties.h"
#include "fileref.h"
#include "tag.h"
#include "tstring.h"
#include "MediaInfo/MediaInfo.h"
using namespace MediaInfoLib;

#include <QStringList>
#include <QHeaderView>
#include <QFont>
#include <QMessageBox>
#include <QImage>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>

#include <QDebug>
#include <codecvt>
#include <locale>

#include "QUSongSupport.h"
#include "QULogService.h"

QUPreviewTree::QUPreviewTree(QWidget *parent): QTreeWidget(parent) {
	this->setColumnCount(2);

	this->header()->hide();
	this->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
	this->header()->setSectionResizeMode(1, QHeaderView::Stretch);

	this->setRootIsDecorated(false);
	this->setIndentation(10);

	// set up "general" toplevel item

	general = new QTreeWidgetItem();
	this->addTopLevelItem(general);

	general->setText(0, tr("Figures"));
	general->setFlags(Qt::ItemIsEnabled);
	general->setForeground(0, Qt::darkGray);
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
	types->setForeground(0, Qt::darkGray);
	types->setFirstColumnSpanned(true);
	types->setExpanded(false);

	types->addChild(this->createInfoItem(QIcon(":/types/text.png"), tr("Song"), QUSongSupport::allowedSongFiles().join(" ")));
	types->addChild(this->createInfoItem(QIcon(":/types/music.png"), tr("Audio"), QUSongSupport::allowedAudioFiles().join(" ")));
	types->addChild(this->createInfoItem(QIcon(":/types/image.png"), tr("Image"), QUSongSupport::allowedImageFiles().join(" ")));
	types->addChild(this->createInfoItem(QIcon(":/types/video.png"), tr("Video"), QUSongSupport::allowedVideoFiles().join(" ")));
	types->addChild(this->createInfoItem(QIcon(":/control/playlist.png"), tr("Playlist"), QUSongSupport::allowedPlaylistFiles().join(" ")));
	types->addChild(this->createInfoItem(QIcon(":/types/license.png"), tr("License"), QUSongSupport::allowedLicenseFiles().join(" ")));
	types->addChild(this->createInfoItem(QIcon(":/types/midi.png"), tr("MIDI"), QUSongSupport::allowedMidiFiles().join(" ")));
	types->addChild(this->createInfoItem(QIcon(":/types/midi_kar.png"), tr("Karaoke"),QUSongSupport::allowedKaraokeFiles().join(" ")));
	types->addChild(this->createInfoItem(QIcon(":/types/score.png"), tr("Score"), QUSongSupport::allowedScoreFiles().join(" ")));
	types->addChild(this->createInfoItem(QIcon(":/types/sync.png"), tr("Sync"), QUSongSupport::allowedSyncFiles().join(" ")));
	types->addChild(this->createInfoItem("", ""));

	// set up "dir" toplevel item

	dir = new QTreeWidgetItem();
	this->addTopLevelItem(dir);

	dir->setText(0, tr("Directory Information"));
	dir->setFlags(Qt::ItemIsEnabled);
	dir->setForeground(0, Qt::darkGray);
	dir->setFirstColumnSpanned(true);

	dir->setExpanded(true);
	dir->setHidden(true);

	// set up "text" toplevel item

	text = new QTreeWidgetItem();
	this->addTopLevelItem(text);

	text->setText(0, tr("Text File Information"));
	text->setFlags(Qt::ItemIsEnabled);
	text->setForeground(0, Qt::darkGray);
	text->setFirstColumnSpanned(true);

	text->setExpanded(true);
	text->setHidden(true);

	// set up "audio" toplevel item

	audio = new QTreeWidgetItem();
	this->addTopLevelItem(audio);

	audio->setText(0, tr("Audio File Information"));
	audio->setFlags(Qt::ItemIsEnabled);
	audio->setForeground(0, Qt::darkGray);
	audio->setFirstColumnSpanned(true);

	audio->setExpanded(true);
	audio->setHidden(true);

	// set up "image" toplevel item

	image = new QTreeWidgetItem();
	this->addTopLevelItem(image);

	image->setText(0, tr("Image File Information"));
	image->setFlags(Qt::ItemIsEnabled);
	image->setForeground(0, Qt::darkGray);
	image->setFirstColumnSpanned(true);

	image->setExpanded(true);
	image->setHidden(true);

	// set up "video" toplevel item

	video = new QTreeWidgetItem();
	this->addTopLevelItem(video);

	video->setText(0, tr("Video File Information"));
	video->setFlags(Qt::ItemIsEnabled);
	video->setForeground(0, Qt::darkGray);
	video->setFirstColumnSpanned(true);

	video->setExpanded(true);
	video->setHidden(true);

	// set up "sync" toplevel item

	sync = new QTreeWidgetItem();
	this->addTopLevelItem(sync);

	sync->setText(0, tr("Sync File Information"));
	sync->setFlags(Qt::ItemIsEnabled);
	sync->setForeground(0, Qt::darkGray);
	sync->setFirstColumnSpanned(true);

	sync->setExpanded(true);
	sync->setHidden(true);

	// set up "file" toplevel item

	file = new QTreeWidgetItem();
	this->addTopLevelItem(file);

	file->setText(0, tr("Other File Information"));
	file->setFlags(Qt::ItemIsEnabled);
	file->setForeground(0, Qt::darkGray);
	file->setFirstColumnSpanned(true);

	file->setExpanded(true);
	file->setHidden(true);
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
void QUPreviewTree::showFileInformation(const QFileInfo &fi, bool deleteAndHide) {
	if(deleteAndHide) {
		qDeleteAll(file->takeChildren());
		qDeleteAll(image->takeChildren());
		qDeleteAll(audio->takeChildren());
		qDeleteAll(video->takeChildren());
		qDeleteAll(text->takeChildren());
		qDeleteAll(sync->takeChildren());
		qDeleteAll(dir->takeChildren());

		file->setHidden(true);
		image->setHidden(true);
		audio->setHidden(true);
		video->setHidden(true);
		text->setHidden(true);
		sync->setHidden(true);
		dir->setHidden(true);
	}

	if(fi.isFile()) {
		QString fileScheme("*." + fi.suffix());

		if(QUSongSupport::allowedImageFiles().contains(fileScheme, Qt::CaseInsensitive))
			showPictureFileInformation(fi);
		else if(QUSongSupport::allowedVideoFiles().contains(fileScheme, Qt::CaseInsensitive))
			showVideoFileInformation(fi);
		else if(QUSongSupport::allowedAudioFiles().contains(fileScheme, Qt::CaseInsensitive))
			showAudioFileInformation(fi);
		else if(QUSongSupport::allowedSongFiles().contains(fileScheme, Qt::CaseInsensitive))
			showTextFileInformation(fi);
		else if(QUSongSupport::allowedKaraokeFiles().contains(fileScheme, Qt::CaseInsensitive))
			showSimpleFileInformation(fi, tr("karaoke file"));
		else if(QUSongSupport::allowedLicenseFiles().contains(fileScheme, Qt::CaseInsensitive))
			showSimpleFileInformation(fi, tr("license file"));
		else if(QUSongSupport::allowedMidiFiles().contains(fileScheme, Qt::CaseInsensitive))
			showSimpleFileInformation(fi, tr("midi file"));
		else if(QUSongSupport::allowedPlaylistFiles().contains(fileScheme, Qt::CaseInsensitive))
			showSimpleFileInformation(fi, tr("playlist file"));
		else if(QUSongSupport::allowedScoreFiles().contains(fileScheme, Qt::CaseInsensitive))
			showSimpleFileInformation(fi, tr("score file"));
		else if(QUSongSupport::allowedSyncFiles().contains(fileScheme, Qt::CaseInsensitive))
			showSyncFileInformation(fi);
	} else {
		showDirectoryFileInformation(fi);
	}
}

void QUPreviewTree::showAudioFileInformation(const QFileInfo &fi) {
	audio->addChild(this->createInfoItem(tr("Filename"), fi.fileName()));
	audio->addChild(this->createInfoItem(tr("Size"), QString("%1 MiB").arg(fi.size() / 1024. / 1024., 0, 'f', 2)));

	/*
	TagLib::FileRef ref(fi.absoluteFilePath().toLocal8Bit().data());

	if(!ref.isNull()) {
		QString artist(TStringToQString(ref.tag()->artist())); if(artist == "") artist = N_A;
		QString title(TStringToQString(ref.tag()->title())); if(title == "") title = N_A;
		QString album(TStringToQString(ref.tag()->album())); if(album == "") album = N_A;
		QString genre(TStringToQString(ref.tag()->genre())); if(genre == "") genre = N_A;
		QString year(QVariant(ref.tag()->year()).toString()); if(year == "0") year = N_A;
		QString track(QVariant(ref.tag()->track()).toString()); if(track == "0") track = N_A;

		extra->addChild(this->createInfoItem("ID3 tag", ""));
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
	}
	*/

	setlocale(LC_ALL, "");
	MediaInfo MI;
	MI.Option(L"CharSet", L"UTF-8");
	std::wstring filePath = fi.absoluteFilePath().toStdWString();
	//filePath = L"/Users/markus/Downloads/JDownloader/!cleaned_foreign/Louane - Si T’étais Là [VIDEO]/Louane - Si T’étais Là.mp3";
	//std::wcout << L"Si T’étais Là" << std::endl << std::flush;
	//std::wcout << L'à' << std::endl << std::flush;
	//qDebug() << QString::fromStdWString(filePath);
	//std::cout << "cout (utf8().data): " << fi.absoluteFilePath().toUtf8().data() << std::flush;
	
	//std::cout << fi.absoluteFilePath().toUtf8().toStdString() << std::endl << std::flush;
	
	if(MI.Open(filePath) == 0) {
		audio->addChild(this->createInfoItem(tr("Error"), tr("Could not open file.")));
		audio->setHidden(false);
		//logSrv->add(QString("absoluteFilePath() = " + fi.absoluteFilePath()), QU::Error);
		return;
	}
	QString artist(QString::fromStdWString(MI.Get(Stream_General, 0, __T("Performer"), Info_Text, Info_Name))); if(artist == "") artist = N_A;
	audio->addChild(this->createInfoItem(tr("Artist"), artist));
	QString title(QString::fromStdWString(MI.Get(Stream_General, 0, __T("Title"), Info_Text, Info_Name))); if(title == "") title = N_A;
	audio->addChild(this->createInfoItem(tr("Title"), title));
	QString album(QString::fromStdWString(MI.Get(Stream_General, 0, __T("Album"), Info_Text, Info_Name))); if(album == "") album = N_A;
	audio->addChild(this->createInfoItem(tr("Album"), album));
	QString genre(QString::fromStdWString(MI.Get(Stream_General, 0, __T("Genre"), Info_Text, Info_Name))); if(genre == "") genre = N_A;
	audio->addChild(this->createInfoItem(tr("Genre"), genre));
	QString year(QString::fromStdWString(MI.Get(Stream_General, 0, __T("Recorded_Date"), Info_Text, Info_Name))); if(year == "") year = N_A;
	audio->addChild(this->createInfoItem(tr("Year"), year));
	QString track(QString::fromStdWString(MI.Get(Stream_General, 0, __T("Track/Position"), Info_Text, Info_Name))); if(track == "") track = N_A;
	audio->addChild(this->createInfoItem(tr("Track"), track));

	QString lengthAudio(QString::fromStdWString(MI.Get(Stream_Audio, 0, __T("PlayTime/String3"), Info_Text, Info_Name)).mid(3));
	if(lengthAudio != "") audio->addChild(this->createInfoItem(tr("Length"), lengthAudio));
	QString formatAudio(QString::fromStdWString(MI.Get(Stream_Audio, 0, __T("Format"), Info_Text, Info_Name)));
	QString codecIDAudio(QString::fromStdWString(MI.Get(Stream_Video, 0, __T("CodecID"), Info_Text, Info_Name)));
	if(formatAudio != "") {
		if(codecIDAudio != "") audio->addChild(this->createInfoItem(tr("Format"), QString("%1 (%2)").arg(formatAudio, codecIDAudio)));
		else audio->addChild(this->createInfoItem(tr("Format"), QString("%1").arg(formatAudio)));
	}
	QString profileAudio(QString::fromStdWString(MI.Get(Stream_Audio, 0, __T("Format_Profile"), Info_Text, Info_Name)));
	if(profileAudio != "") audio->addChild(this->createInfoItem(tr("Profile"), profileAudio));
	QString bitRateAudio(QString::fromStdWString(MI.Get(Stream_Audio, 0, __T("BitRate"), Info_Text, Info_Name)));
	QString bitRateModeAudio(QString::fromStdWString(MI.Get(Stream_Audio, 0, __T("BitRate_Mode"), Info_Text, Info_Name)).toUpper());
	if(bitRateAudio != "") {
		if(QString::compare(bitRateModeAudio, "CBR") == 0)
			audio->addChild(this->createInfoItem(tr("Bitrate"), QString("%1 kbit/s (%2)").arg(bitRateAudio.toInt()/1000).arg(bitRateModeAudio)));
		else {
			QString bitRateMaxAudio(QString::fromStdWString(MI.Get(Stream_Audio, 0, __T("BitRate_Maximum"), Info_Text, Info_Name))); if(bitRateMaxAudio != "")
			audio->addChild(this->createInfoItem(tr("Bitrate"), QString("%1 kbit/s (%2, max. %3 kbit/s)").arg(bitRateAudio.toInt()/1000).arg(bitRateModeAudio.toLower()).arg(bitRateMaxAudio.toInt()/1000)));
		}
	}
	//QString channels(QString::fromStdWString(MI.Get(Stream_Audio, 0, __T("Channel_s_"), Info_Text, Info_Name)));
	//if(channels != "") audio->addChild(this->createInfoItem(tr("Channels"), channels));
	QString samplingRate(QString::fromStdWString(MI.Get(Stream_Audio, 0, __T("SamplingRate"), Info_Text, Info_Name)));
	if(samplingRate != "") audio->addChild(this->createInfoItem(tr("Sampling rate"), QString("%1 kHz").arg(samplingRate.toInt() / 1000.)));
	QString bitDepthAudio(QString::fromStdWString(MI.Get(Stream_Video, 0, __T("BitDepth"), Info_Text, Info_Name)));
	if(bitDepthAudio != "") audio->addChild(this->createInfoItem(tr("Bit depth"), bitDepthAudio));

	// raw MediaInfoLib information (as reference)
	//audio->addChild(this->createInfoItem("", ""));
	//QString raw(QString::fromStdWString(MI.Inform()));
	//audio->addChild(this->createInfoItem(tr("Raw info"), raw));
	MI.Close();

	audio->addChild(this->createInfoItem("", ""));
	audio->setHidden(false);
}

void QUPreviewTree::showPictureFileInformation(const QFileInfo &fi) {
	image->addChild(this->createInfoItem(tr("Filename"), fi.fileName()));
	image->addChild(this->createInfoItem(tr("Size"), QString("%1 KiB").arg(fi.size() / 1024., 0, 'f', 2)));

	QImage img(fi.filePath());
	image->addChild(this->createInfoItem(tr("Dimensions"), QString("%1 x %2 px").arg(img.width()).arg(img.height())));
	image->addChild(this->createInfoItem(tr("Depth"), QVariant(img.depth()).toString()));

	image->addChild(this->createInfoItem("", ""));
	image->setHidden(false);
}

void QUPreviewTree::showVideoFileInformation(const QFileInfo &fi) {
	video->addChild(this->createInfoItem(tr("Filename"), fi.fileName()));
	video->addChild(this->createInfoItem(tr("Size"), QString("%1 MiB").arg(fi.size() / 1024. / 1024., 0, 'f', 2)));


	MediaInfo MI;
	if(MI.Open(fi.absoluteFilePath().toStdWString()) == 0) {
		video->addChild(this->createInfoItem(tr("Error"), "Could not open file."));
		video->setHidden(false);
		return;
	}

	// general information
	QString artist(QString::fromStdWString(MI.Get(Stream_General, 0, __T("Performer"), Info_Text, Info_Name)));
	if(artist != "") video->addChild(this->createInfoItem(tr("Artist"), artist));
	QString title(QString::fromStdWString(MI.Get(Stream_General, 0, __T("Title"), Info_Text, Info_Name)));
	if(title != "") video->addChild(this->createInfoItem(tr("Title"), title));
	QString album(QString::fromStdWString(MI.Get(Stream_General, 0, __T("Album"), Info_Text, Info_Name)));
	if(album != "") video->addChild(this->createInfoItem(tr("Album"), album));
	QString length(QString::fromStdWString(MI.Get(Stream_General, 0, __T("PlayTime/String3"), Info_Text, Info_Name)).mid(3));
	if(length != "") video->addChild(this->createInfoItem(tr("Length"), length));
	QString bitRate(QString::fromStdWString(MI.Get(Stream_General, 0, __T("BitRate"), Info_Text, Info_Name)));
	QString bitRateMode(QString::fromStdWString(MI.Get(Stream_General, 0, __T("BitRate_Mode"), Info_Text, Info_Name)).toUpper());
	if(bitRate != "") {
		if(bitRateMode != "") video->addChild(this->createInfoItem(tr("Bit rate"), QString("%1 kbit/s (%2)").arg(bitRate.toInt() / 1000.).arg(bitRateMode)));
		else video->addChild(this->createInfoItem(tr("Bit rate"), QString("%1 kbit/s").arg(bitRate.toInt() / 1000.)));
	}
	QString format(QString::fromStdWString(MI.Get(Stream_General, 0, __T("Format"), Info_Text, Info_Name)));
	QString codecID(QString::fromStdWString(MI.Get(Stream_General, 0, __T("CodecID"), Info_Text, Info_Name)));
	if(format != "") {
		if(codecID != "") video->addChild(this->createInfoItem(tr("Format"), QString("%1 (%2)").arg(format, codecID)));
		else video->addChild(this->createInfoItem(tr("Format"), QString("%1").arg(format)));
	}
	QString profile(QString::fromStdWString(MI.Get(Stream_General, 0, __T("Format_Profile"), Info_Text, Info_Name)));
	if(profile != "") video->addChild(this->createInfoItem(tr("Profile"), profile));
	QString countVideo(QString::fromStdWString(MI.Get(Stream_Video, 0, __T("StreamCount"), Info_Text, Info_Name)));
	QString countAudio(QString::fromStdWString(MI.Get(Stream_Audio, 0, __T("StreamCount"), Info_Text, Info_Name)));
	if(countAudio == "") countAudio = "no";
	video->addChild(this->createInfoItem(tr("Streams"), QString("%1 (%2 video, %3 audio)").arg(countVideo.toInt()+countAudio.toInt()).arg(countVideo, countAudio)));

	// video stream information
	video->addChild(this->createInfoItem("", ""));
	video->addChild(this->createInfoItem("Video track", ""));
	QString lengthVideo(QString::fromStdWString(MI.Get(Stream_Video, 0, __T("PlayTime/String3"), Info_Text, Info_Name)).mid(3));
	if(lengthVideo != "") video->addChild(this->createInfoItem(tr("Length"), lengthVideo));
	QString width(QString::fromStdWString(MI.Get(Stream_Video, 0, __T("Width"), Info_Text, Info_Name)));
	QString height(QString::fromStdWString(MI.Get(Stream_Video, 0, __T("Height"), Info_Text, Info_Name)));
	QString aspectRatio(QString::fromStdWString(MI.Get(Stream_Video, 0, __T("DisplayAspectRatio/String"), Info_Text, Info_Name)));
	if(width != "" || height != "" || aspectRatio != "") video->addChild(this->createInfoItem(tr("Dimensions"), QString("%1 x %2 px (%3)").arg(width, height, aspectRatio)));
	QString formatVideo(QString::fromStdWString(MI.Get(Stream_Video, 0, __T("Format"), Info_Text, Info_Name)));
	QString codecIDVideo(QString::fromStdWString(MI.Get(Stream_Video, 0, __T("CodecID"), Info_Text, Info_Name)));
	if(formatVideo != "") {
		if(codecIDVideo != "") video->addChild(this->createInfoItem(tr("Format"), QString("%1 (%2)").arg(formatVideo, codecIDVideo)));
		else video->addChild(this->createInfoItem(tr("Format"), QString("%1").arg(formatVideo)));
	}
	QString profileVideo(QString::fromStdWString(MI.Get(Stream_Video, 0, __T("Format_Profile"), Info_Text, Info_Name)));
	if(profileVideo != "") video->addChild(this->createInfoItem(tr("Profile"), profileVideo));
	QString bitRateVideo(QString::fromStdWString(MI.Get(Stream_Video, 0, __T("BitRate"), Info_Text, Info_Name)));
	QString bitRateModeVideo(QString::fromStdWString(MI.Get(Stream_Video, 0, __T("BitRate_Mode"), Info_Text, Info_Name)).toUpper());
	if(bitRateVideo != "") {
		if(QString::compare(bitRateModeVideo, "CBR") == 0)
			video->addChild(this->createInfoItem(tr("Bitrate"), QString("%1 kbit/s (%2)").arg(bitRateVideo.toInt()/1000).arg(bitRateModeVideo)));
		else {
			QString bitRateMaxVideo(QString::fromStdWString(MI.Get(Stream_Video, 0, __T("BitRate_Maximum"), Info_Text, Info_Name)));
			if(bitRateMaxVideo != "") video->addChild(this->createInfoItem(tr("Bitrate"), QString("%1 kbit/s (max. %2 kbit/s)").arg(bitRateVideo.toInt()/1000).arg(bitRateMaxVideo.toInt()/1000)));
			else video->addChild(this->createInfoItem(tr("Bitrate"), QString("%1 kbit/s").arg(bitRateVideo.toInt() / 1000.)));
		}
	}
	QString frameRate(QString::fromStdWString(MI.Get(Stream_Video, 0, __T("FrameRate"), Info_Text, Info_Name)));
	QString frameRateMode(QString::fromStdWString(MI.Get(Stream_Video, 0, __T("FrameRate_Mode/String"), Info_Text, Info_Name)));
	if(frameRate != "") {
		if(frameRateMode != "") video->addChild(this->createInfoItem(tr("Framerate"), QString("%1 fps (%2)").arg(frameRate, frameRateMode.toLower())));
		else video->addChild(this->createInfoItem(tr("Framerate"), QString("%1 fps").arg(frameRate)));
	}
	QString colorSpace(QString::fromStdWString(MI.Get(Stream_Video, 0, __T("ColorSpace"), Info_Text, Info_Name)));
	if(colorSpace != "") video->addChild(this->createInfoItem(tr("Color space"), colorSpace));
	QString bitDepthVideo(QString::fromStdWString(MI.Get(Stream_Video, 0, __T("BitDepth"), Info_Text, Info_Name)));
	if(bitDepthVideo != "") video->addChild(this->createInfoItem(tr("Bit depth"), bitDepthVideo));
	QString chromaSubsampling(QString::fromStdWString(MI.Get(Stream_Video, 0, __T("ChromaSubsampling/String"), Info_Text, Info_Name)));
	if(chromaSubsampling != "") video->addChild(this->createInfoItem(tr("Chroma"), chromaSubsampling));
	QString scanType(QString::fromStdWString(MI.Get(Stream_Video, 0, __T("ScanType"), Info_Text, Info_Name)));
	if(scanType != "") video->addChild(this->createInfoItem(tr("Scan type"), scanType));
	QString streamSizeVideo(QString::fromStdWString(MI.Get(Stream_Video, 0, __T("StreamSize"), Info_Text, Info_Name)));
	if(streamSizeVideo != "") video->addChild(this->createInfoItem(tr("Stream size"), QString("%1 MiB").arg(QString::number(streamSizeVideo.toInt() / 1024. / 1024., 'f', 2))));

	// audio stream information
	video->addChild(this->createInfoItem("", ""));
	video->addChild(this->createInfoItem("Audio track", ""));
	QString lengthAudio(QString::fromStdWString(MI.Get(Stream_Audio, 0, __T("PlayTime/String3"), Info_Text, Info_Name)).mid(3));
	if(lengthAudio != "") video->addChild(this->createInfoItem(tr("Length"), lengthAudio));
	QString formatAudio(QString::fromStdWString(MI.Get(Stream_Audio, 0, __T("Format"), Info_Text, Info_Name)));
	QString codecIDAudio(QString::fromStdWString(MI.Get(Stream_Video, 0, __T("CodecID"), Info_Text, Info_Name)));
	if(formatAudio != "") {
		if(codecIDAudio != "") video->addChild(this->createInfoItem(tr("Format"), QString("%1 (%2)").arg(formatAudio, codecIDAudio)));
		else video->addChild(this->createInfoItem(tr("Format"), QString("%1").arg(formatAudio)));
	}
	QString profileAudio(QString::fromStdWString(MI.Get(Stream_Audio, 0, __T("Format_Profile"), Info_Text, Info_Name)));
	if(profileAudio != "") video->addChild(this->createInfoItem(tr("Profile"), profileAudio));
	QString bitRateAudio(QString::fromStdWString(MI.Get(Stream_Audio, 0, __T("BitRate"), Info_Text, Info_Name)));
	QString bitRateModeAudio(QString::fromStdWString(MI.Get(Stream_Audio, 0, __T("BitRate_Mode"), Info_Text, Info_Name)).toUpper());
	if(bitRateAudio != "") {
		if(QString::compare(bitRateModeAudio, "CBR") == 0)
			video->addChild(this->createInfoItem(tr("Bitrate"), QString("%1 kbit/s (%2)").arg(bitRateAudio.toInt()/1000).arg(bitRateModeAudio)));
		else {
			QString bitRateMaxAudio(QString::fromStdWString(MI.Get(Stream_Audio, 0, __T("BitRate_Maximum"), Info_Text, Info_Name))); if(bitRateMaxAudio != "")
			video->addChild(this->createInfoItem(tr("Bitrate"), QString("%1 kbit/s (%2, max. %3 kbit/s)").arg(bitRateAudio.toInt()/1000).arg(bitRateModeAudio).arg(bitRateMaxAudio.toInt()/1000)));
		}
	}
	QString streamSizeAudio(QString::fromStdWString(MI.Get(Stream_Audio, 0, __T("StreamSize"), Info_Text, Info_Name)));
	if(streamSizeAudio != "") video->addChild(this->createInfoItem(tr("Stream size"), QString("%1 MiB").arg(QString::number(streamSizeAudio.toInt() / 1024. / 1024., 'f', 2))));
	//QString channels(QString::fromStdWString(MI.Get(Stream_Audio, 0, __T("Channel_s_"), Info_Text, Info_Name)));
	//if(channels != "") video->addChild(this->createInfoItem(tr("Channels"), channels));
	QString samplingRate(QString::fromStdWString(MI.Get(Stream_Audio, 0, __T("SamplingRate"), Info_Text, Info_Name)));
	if(samplingRate != "") video->addChild(this->createInfoItem(tr("Sampling rate"), QString("%1 kHz").arg(samplingRate.toInt() / 1000.)));
	QString bitDepthAudio(QString::fromStdWString(MI.Get(Stream_Video, 0, __T("BitDepth"), Info_Text, Info_Name)));
	if(bitDepthAudio != "") video->addChild(this->createInfoItem(tr("Bit depth"), bitDepthAudio));

	// raw MediaInfoLib information (as reference)
	//video->addChild(this->createInfoItem("", ""));
	//video->addChild(this->createInfoItem("Raw", ""));
	//QString raw(QString::fromStdWString(MI.Inform()));
	//video->addChild(this->createInfoItem(tr("Raw"), raw));
	MI.Close();

	video->addChild(this->createInfoItem("", ""));
	video->setHidden(false);
}

void QUPreviewTree::showTextFileInformation(const QFileInfo &fi) {
	text->addChild(this->createInfoItem(tr("Filename"), fi.fileName()));
	text->addChild(this->createInfoItem(tr("Size"), QString("%1 KiB").arg(fi.size() / 1024., 0, 'f', 2)));
	QFile file(fi.filePath());
	if(file.open(QIODevice::ReadOnly)) {
		QByteArray content = file.readAll();
		int CR = content.count(QByteArray("\x0D"));
		int LF = content.count(QByteArray("\x0A"));
		if(CR == 0 && LF != 0)
			text->addChild(this->createInfoItem(tr("Line endings"), tr("Unix/Linux/Mac OS X (LF)")));
		else if(CR == LF)
			text->addChild(this->createInfoItem(tr("Line endings"), tr("Windows (CRLF)")));
		else if(CR != 0 && LF == 0)
			text->addChild(this->createInfoItem(tr("Line endings"), tr("Mac < OS X (CR)")));
		else
			text->addChild(this->createInfoItem(tr("Line endings"), QString("inconsistent (%1 CR, %2 LF)").arg(CR).arg(LF)));

		file.close();
	}
	//TODO: add file encoding?

	text->addChild(this->createInfoItem("", ""));
	text->setHidden(false);
}

void QUPreviewTree::showSyncFileInformation(const QFileInfo &fi) {
	sync->addChild(this->createInfoItem(tr("Filename"), fi.fileName()));
	sync->addChild(this->createInfoItem(tr("Size"), QString("%1 KiB").arg(fi.size() / 1024., 0, 'f', 2)));
	QFile file(fi.filePath());
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		logSrv->add(QString("Failed to open sync file: " + fi.absoluteFilePath()), QU::Error);
		return;
	}

	QByteArray jsonData = file.readAll();
	file.close();

	// Parse JSON data
	QJsonParseError error;
	QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &error);
	if (jsonDoc.isNull())
	{
		logSrv->add(QString("Failed to parse JSON: " + error.errorString()), QU::Error);
		return;
	}

	// Get the root object
	QJsonObject jsonObj = jsonDoc.object();

	// Access values from the JSON object
	int songId = jsonObj["song_id"].toInt();
	QStringList metaTags = jsonObj["meta_tags"].toString().split(",");
	bool pinned = jsonObj["pinned"].toBool();
	int version = jsonObj["version"].toInt();

	// Access nested objects
	//QJsonObject txtObj = jsonObj["txt"].toObject();
	//QString txtFileName = txtObj["fname"].toString();
	//QString txtResource = txtObj["resource"].toString();

	sync->addChild(this->createInfoItem("Song ID", QString::number(songId)));
	sync->addChild(this->createInfoItem("Metatags", metaTags.first()));
	for (int i = 1; i < metaTags.size(); ++i) {
		sync->addChild(createInfoItem("", metaTags[i]));
	}
	sync->addChild(this->createInfoItem("Pinned", QVariant(pinned).toString()));
	sync->addChild(this->createInfoItem("Version", QString::number(version)));

	sync->addChild(this->createInfoItem("", ""));
	sync->setHidden(false);
}

void QUPreviewTree::showSimpleFileInformation(const QFileInfo &fi, const QString type) {
	file->addChild(this->createInfoItem(tr("Filename"), fi.fileName()));
	file->addChild(this->createInfoItem(tr("Type"), type));
	file->addChild(this->createInfoItem(tr("Size"), QString("%1 KiB").arg(fi.size() / 1024., 0, 'f', 2)));

	file->addChild(this->createInfoItem("", ""));
	file->setHidden(false);
}

void QUPreviewTree::showDirectoryFileInformation(const QFileInfo &di) {
	dir->addChild(this->createInfoItem(tr("Path"), QDir::toNativeSeparators(di.absolutePath())));
	QFileInfoList fil(di.dir().entryInfoList(QDir::Files | QDir::NoDotAndDotDot));
	dir->addChild(this->createInfoItem(tr("Files"), QString::number(fil.size())));
	dir->addChild(this->createInfoItem("", ""));
	dir->setHidden(false);

	foreach(QFileInfo fi, fil) {
		showFileInformation(fi, false);
	}
}

void QUPreviewTree::showSelectedLength(QTreeWidgetItem *child, int seconds) {
	if(!child) return;

	generalSelected->setExpanded(seconds != -1);
	child->setText(1, seconds == -1 ? N_A : QString("%1:%2:%3").arg(seconds / 3600, 2, 10, QChar('0')).arg((seconds % 3600) / 60, 2, 10, QChar('0')).arg(seconds % 60, 2, 10, QChar('0')));
}
