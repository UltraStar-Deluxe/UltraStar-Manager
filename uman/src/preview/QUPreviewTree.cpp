#include "QUPreviewTree.h"
#include "QUSongFile.h"

#include "fileref.h"
#include "tag.h"
#include "tstring.h"

#include <QStringList>
#include <QHeaderView>
#include <QFont>
#include <QMessageBox>

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

	general->insertChild(SONG_COUNT_CHILD_INDEX, this->createInfoItem("Songs", N_A));
	general->insertChild(SELECTED_SONG_COUNT_CHILD_INDEX, this->createInfoItem("Selected Songs", N_A));
	general->insertChild(HIDDEN_SONG_COUNT_CHILD_INDEX, this->createInfoItem("Hidden Songs", N_A));
	general->insertChild(VISIBLE_SONG_COUNT_CHILD_INDEX, this->createInfoItem("Visible Songs", N_A));

	general->setExpanded(true);

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

	QString fileScheme("*." + fi.suffix());

	if(!QUSongFile::allowedAudioFiles().contains(fileScheme, Qt::CaseInsensitive)) {
		current->setHidden(true);
		extra->setHidden(true);
		return; // read only audio files for ID3 tag
	}

	current->addChild(this->createInfoItem(tr("Filename"), fi.fileName()));
	current->addChild(this->createInfoItem(tr("Path"), fi.absolutePath()));
	current->addChild(this->createInfoItem(tr("Type"), tr("audio file")));

	TagLib::FileRef ref(fi.absoluteFilePath().toLocal8Bit().data());

	if(!ref.isNull()) {
		QString artist(TStringToQString(ref.tag()->artist())); if(artist == "") artist = N_A;
		QString title(TStringToQString(ref.tag()->title())); if(title == "") title = N_A;
		QString genre(TStringToQString(ref.tag()->genre())); if(genre == "") genre = N_A;
		QString year(QVariant(ref.tag()->year()).toString()); if(year == "0") year = N_A;

		extra->addChild(this->createInfoItem(tr("Artist"), artist));
		extra->addChild(this->createInfoItem(tr("Title"), title));
		extra->addChild(this->createInfoItem(tr("Genre"), genre));
		extra->addChild(this->createInfoItem(tr("Year"), year));

		extra->setText(0, "ID3 Tags");
		extra->setHidden(false);
	}

	current->setHidden(false);
}
