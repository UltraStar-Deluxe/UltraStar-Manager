#include "QUPreviewTree.h"
#include "QUSongFile.h"

#include "audioproperties.h"
#include "fileref.h"
#include "tag.h"
#include "tstring.h"

#include <QStringList>
#include <QHeaderView>
#include <QFont>
#include <QMessageBox>
#include <QImage>

QUPreviewTree::QUPreviewTree(QWidget *parent): QTreeWidget(parent) {
	this->setColumnCount(2);

	this->header()->hide();
	this->header()->setResizeMode(0, QHeaderView::ResizeToContents);
	this->header()->setResizeMode(1, QHeaderView::Stretch);

	this->setRootIsDecorated(false);

	// set up "general" toplevel item

	general = new QTreeWidgetItem();
	this->addTopLevelItem(general);

	general->setText(0, tr("General Information"));
	general->setFlags(Qt::ItemIsEnabled);
	general->setTextColor(0, Qt::darkGray);
	general->setFirstColumnSpanned(true);

	general->insertChild(SONG_COUNT_CHILD_INDEX, this->createInfoItem(tr("Songs"), N_A));
	general->insertChild(SELECTED_SONG_COUNT_CHILD_INDEX, this->createInfoItem(tr("Selected Songs"), N_A));
	general->insertChild(HIDDEN_SONG_COUNT_CHILD_INDEX, this->createInfoItem(tr("Hidden Songs"), N_A));
	general->insertChild(VISIBLE_SONG_COUNT_CHILD_INDEX, this->createInfoItem(tr("Visible Songs"), N_A));

	general->setExpanded(true);

	// set up "types" toplevel item (totally static)

	types = new QTreeWidgetItem();
	this->addTopLevelItem(types);

	types->setText(0, tr("Supported File Types"));
	types->setFlags(Qt::ItemIsEnabled);
	types->setTextColor(0, Qt::darkGray);
	types->setFirstColumnSpanned(true);

	types->addChild(this->createInfoItem(tr("Song"), QU::allowedSongFiles().join(" ")));
	types->addChild(this->createInfoItem(tr("Audio"), QU::allowedAudioFiles().join(" ")));
	types->addChild(this->createInfoItem(tr("Picture"), QU::allowedPictureFiles().join(" ")));
	types->addChild(this->createInfoItem(tr("Video"), QU::allowedVideoFiles().join(" ")));

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

	QFont f(infoItem->font(0));
	f.setBold(true);
	infoItem->setFont(0, f);

	infoItem->setFlags(Qt::ItemIsEnabled);

	return infoItem;
}

void QUPreviewTree::setSongCount(int count) {
	QTreeWidgetItem *child = general->child(SONG_COUNT_CHILD_INDEX);

	if(child)
		child->setText(1, QVariant(count).toString());
}

void QUPreviewTree::setSelectedSongCount(int count) {
	QTreeWidgetItem *child = general->child(SELECTED_SONG_COUNT_CHILD_INDEX);

	if(child)
		child->setText(1, QVariant(count).toString());
}

void QUPreviewTree::setHiddenSongCount(int count) {
	QTreeWidgetItem *child = general->child(HIDDEN_SONG_COUNT_CHILD_INDEX);

	if(child)
		child->setText(1, QVariant(count).toString());
}

void QUPreviewTree::setVisibleSongCount(int count) {
	QTreeWidgetItem *child = general->child(VISIBLE_SONG_COUNT_CHILD_INDEX);

	if(child)
		child->setText(1, QVariant(count).toString());
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

	if(QU::allowedAudioFiles().contains(fileScheme, Qt::CaseInsensitive))
		showAudioFileInformation(fi);
	else if(QU::allowedPictureFiles().contains(fileScheme, Qt::CaseInsensitive))
		showPictureFileInformation(fi);
	else if(QU::allowedVideoFiles().contains(fileScheme, Qt::CaseInsensitive))
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

//	extra->setText(0, tr("Video Properties"));
//	extra->setHidden(false);

	current->setHidden(false);
}
