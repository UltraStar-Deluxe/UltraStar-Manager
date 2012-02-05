#include "QUMonty.h"
#include "QULogService.h"
#include "QUSongDatabase.h"

#include <QFile>
#include <QSettings>
#include <QPixmap>
#include <QFileInfo>
#include <QRegExp>

QUMonty::QUMonty() {
	initMessages();

	_watcher = new QFileSystemWatcher();
}

QUMonty::~QUMonty() {
	delete _watcher;
}

void QUMonty::initMessages() {
	messages << QObject::tr("If you double-click the cell where the row of an <i>audio file</i> and the <i>audio column</i> cross, you can immediately set your song to use this audio file in UltraStar.<br><br>This also works for cover and background images as well as video files.");
	messages << QObject::tr("You can edit the value of most tags in the <i>Song Details</i> area. Just double-click the right cell there to enter the edit mode of a specific value.");
	messages << QObject::tr("You can modify the order in which tags are saved to a song file by dragging &amp; dropping these tags under <i>Options &rarr; Tag Save Order...</i>.");
	messages << QObject::tr("Did you remark the <i>Tasks</i> area? Just check the tasks you want to be carried out, select some songs and then click on the button labeled <i>Run Tasks</i>.<br><br>You can also hit <i>Ctrl+R</i>.");
	messages << QObject::tr("You can hide the event log by unchecking <i>View &rarr; Event Log</i>.<br><br>This also works for <i>Song Details</i> and the <i>Tasks</i> area.");
	messages << QObject::tr("Do you want me to say something else? Tell everybody about your requests in our <a href=\"http://forum.ultra-star.de/viewforum.php?f=62\">project forum</a>.");
	messages << QObject::tr("The BPM tag is not editable? Yeah, that's right. This is not a song creation tool like the <a href=\"http://sourceforge.net/projects/usc\">UltraStar Creator</a>. Nevertheless, you can modify all lyrics and correct spelling errors.");
	messages << QObject::tr("Did I hear you correctly? A missing feature?! Just click <a href=\"http://forum.ultra-star.de/viewforum.php?f=62\">here</a> and let everybody know &ndash; especially the developers.");
	messages << QObject::tr("You can expand or collapse the song tree in the <i>Extras</i> menu. That means you will either see only directories of your songs or all files you can use.");
	messages << QObject::tr("You are missing a video for your favourite song? Check out <a href=\"http://www.youtube.com\">Youtube</a>, <a href=\"http://www.dailymotion.com\">Dailymotion</a>, or <a href=\"http://vimeo.com\">Vimeo</a>!");
	messages << QObject::tr("Whenever a task could not be applied, you can find a corresponding entry in the event log. Mostly a file could not be found or invalid characters (e.g. '?') are used to rename something.<br><br>Be aware that UltraStar can display more characters than you can use in Windows for a file.");
	messages << QObject::tr("Did you notice that there are green, yellow, and red smileys in the song tree? Green ones tell you that your song directory is named correctly after <i>Artist - Title</i> case-sensitive while yellow ones only match case-insensitive. The red smileys indicate an entirely wrong combination.");
	messages << QObject::tr("The <i>chain</i> next to an image, audio or video file indicates that it can be found by UltraStar correctly.<br><br>You can reset chains by double-clicking the correct column.");
	messages << QObject::tr("Files are unused by UltraStar whenever they are displayed in <font color=\"gray\">gray</font>.");
	messages << QObject::tr("You want to know how many songs you have in your collection? Well, there are <b>%1 songs</b>. Not bad at all.<br><br>Do you have a cover, a background or a video for each one? No? Then you've got a new task.");
	messages << QObject::tr("Hmmm... just in case you haven't noticed: The last events are located at the top of the event log.");
	messages << QObject::tr("Did you remark these nice tooltips at the header of each column in the song tree? There are more tooltips for nearly every button here.");
	messages << QObject::tr("Do you wonder why some tasks want to rename your file into <i>something.*</i>?<br><br>Well, your file suffix is preserved whatever it may be. So don't bother.");
	messages << QObject::tr("The first *.txt or *.txd file in any subdirectory of your song folder will be recognized as a song and listed in the song tree. Other song files will be so-called <i>friends</i> to this primary song. You can switch the primary song in the context-menu of a song file.");
	messages << QObject::tr("There is a song missing which is found by UltraStar instead? That's impossible!<br><br>Well... please report it on the <a href=\"http://forum.ultra-star.de/viewforum.php?f=62\">project forum</a> and include details where that song is located.");
	messages << QObject::tr("You can turn off the relative paths in the folder column by unchecking <i>View &rarr; Show Relative Song Paths</i>.");
	messages << QObject::tr("What are <b><font color=\"royalblue\">blue folders</font></b>? If a tag is found in a song text file that will not be recognized by UltraStar then this song is marked with a blue folder.<br><br>That's not bad at all. These tags will stay untouched by default. However, you can either add them using <i>Options &rarr; Custom Tags...</i> or use a dedicated task to remove all unknown tags, if you want.");
	messages << QObject::tr("The tooltip of a blue folder shows all unsupported tags found in the corresponding song file.");
	messages << QObject::tr("Try to click on an audio file and see what appears in the preview window...");
	messages << QObject::tr("Did you notice the large number of default values for a genre in the <i>Song Details</i>? These genres originate from the good old ID3v1 specification as well as some custom Winamp extensions.");
	messages << QObject::tr("I shall be quiet? Then uncheck me <i>in the top right corner of the main menu</i>.");
	messages << QObject::tr("The default values for an artist, a title and the year in the <i>Song Details</i> are taken from the ID3 tag, if available.");
	messages << QObject::tr("There is a task that enables you to set up missing files for a song automatically. That task looks for the first proper files (audio, image, video) and uses that file if the present entry is invalid or missing.");
	messages << QObject::tr("Some details can be edited simultaneously for a selection of songs. Simply select some songs and use the <i>Song Details</i> table.");
	messages << QObject::tr("If you drag &amp; drop a dockable widget (song details, tasks, event log) over another dockable widget, these two widgets will merge into a single widget with tabs at the bottom.");
	messages << QObject::tr("You can make a dockable widget (song details, tasks, event log) floatable if you double-click on its title bar.");
	messages << QObject::tr("Did you remark that the state of the application window as well as all dockable widgets is saved on exit?");
	messages << QObject::tr("Unsupported tags will be saved after all other tags in the song file. You can add these tags as custom tags via <i>Options &rarr; Custom Tags...</i> and then specify their save order in <i>Options &rarr; Tag Save Order...</i>.");
	messages << QObject::tr("You have a problem with changing the artist tag because autocomplete wants to use the ID3 tag value? (e.g. \"abba\" &rarr; \"Abba\")<br><br>Try to check <i>Use Case-sensitive Autocomplete</i> in the <i>Song Details</i> table.");
	messages << QObject::tr("If you double-click a text file, you'll see its contents.");
	messages << QObject::tr("Did you know that you can drag one or more files from your explorer onto a song in the tree to <i>copy</i> and use/assign this or these files automatically for that song?");
	messages << QObject::tr("Files that appear <font color=\"gray\"><s>striked-out and gray</s></font> cannot be used anywhere and are useless to UltraStar.");
	messages << QObject::tr("Did you know that there is a context-menu in the song tree? Try a right-click on a file or a song directory and see what happens...");
	messages << QObject::tr("You can delete a single file by pushing the delete key on your keyboard or by choosing <i>Delete</i> in the context menu of that file.<br><br><b>Attention! You can delete song text files!</b>");
	messages << QObject::tr("If you drag &amp; drop a song text file from your explorer into the song tree (not at an item), a new folder will be created and your file will be copied to that new location.<br><br>This way you can easily add new songs to your collection.");
	messages << QObject::tr("Whenever a folder appears <b>bold</b>, there are unsaved changes. Don't forget to save them or turn on <i>Options &rarr; Auto-Save</i>!");
	messages << QObject::tr("You can filter the song tree. Just hit <i>Ctrl+F</i> and type some regular expression. Then press the filter button.");
	messages << QObject::tr("Have you ever created a <i>report</i> of your songs? No?<br><br>Try <i>Extras &rarr; Create Report...</i> or simply press <i>F8</i> to export your song collection to PDF, HTML, CSV, or plain text.");
	messages << QObject::tr("If you double-click an image file you'll get a preview of it.");
	messages << QObject::tr("You can delete a rename task by deleting its XML configuration file in the <tt>plugins/config/rename</tt> subdirectory.");
	messages << QObject::tr("If you make a double-click on a rename task in the task list you can modify it.");
	messages << QObject::tr("There is a context menu in the task list which enables you to add and edit rename tasks.");
	messages << QObject::tr("Some operations cannot be cancelled because that would lead to an inconsistent state, e.g. applying a filter.");
	messages << QObject::tr("You can expand and collapse the categories in the <i>File Information</i> window by double-clicking their names.");
	messages << QObject::tr("A cross with an exclamation mark in the song tree indicates that the value for a file-related tag (e.g. #COVER) was set but the file does not exist.");
	messages << QObject::tr("A song file is <b>invalid</b> if one of the artist, title, or BPM tag is missing, or if is has a length of zero. That song will have a <font style=\"BACKGROUND-COLOR: #FFD6D6\">&nbsp;light red or pink background&nbsp;</font> in the song tree.");
	messages << QObject::tr("If you want to integrate new songs via drag &amp; drop in the song tree, it may be helpful to hide all other songs first. Simply select <i>Hide &rarr; All</i> from the song tree context menu.");
	messages << QObject::tr("There are two types of context menus in the song tree. One for files and one for folders (entire songs).");
	messages << QObject::tr("You can negate the regular expression for a filter if you click the magnifier button in front of the input line.");
	messages << QObject::tr("If you filter \"abba|akon|dido\", you will get all items that contain \"abba\", \"akon\" or \"dido\".");
	messages << QObject::tr("Nearly all icons in the song tree have a tooltip. Just let the mouse cursor hover and wait a couple of seconds. A hint will appear.");
	messages << QObject::tr("You can see duplicate songs if you press <i>Find Duplicates</i> in the filter/search area (<i>Ctrl+F</i>). Duplicate songs will be listed together.");
	messages << QObject::tr("Playlists will be connected to the current song database. Whenever a song is changed, playlists will be updated automatically.<br><br>Changed playlists need to be saved.");
	messages << QObject::tr("The <i>Songs &rarr; Get Covers...</i> feature allows you to download covers from remote image source. They will only be downloaded for songs that are checked. That also applies to <i>Copy &amp; Set</i>.");
	messages << QObject::tr("You can expand a list of all supported file types if you double-click the corresponding headline in <i>File Information</i>.");
	messages << QObject::tr("Editable tasks can be modified via double-click. Additionally, a context menu is available via right-click.");
	messages << QObject::tr("The event log will be saved on exit in the application path.");
	messages << QObject::tr("The header of the song tree has a context menu. Here you can show and hide columns. Presets offer a more convenient way to do this.");
	messages << QObject::tr("You can display the lyrics of the currently selected song via <i>Ctrl+L</i>.");
	//messages << QObject::tr("The new-style menu can be collapsed with <i>Ctrl+F1</i>.");
	messages << QObject::tr("You can preview whole songs with the new <i>Media Player</i>! It's so cool. Selected songs, the whole song database or the current playlist can be played. You can sing and check the quality even without having to start UltraStar.");
	messages << QObject::tr("If you right-click in the lyrics of the Media Player while a song is playing, you can jump to the lyrics editor and correct spelling errors and typos.");
	messages << QObject::tr("You can jump to any position in a song in the Media Player.");
	messages << QObject::tr("Having trouble spotting bad song covers? Try the image review via <i>Songs &rarr; Review Images</i>. You get a cool preview for all covers or backgrounds and can unlink them from a song with your spacebar.");
	messages << QObject::tr("You can calculate the speed of a song via <i>Songs &rarr; Calculate Song Speed</i>. It is an indicator for the average number of syllables per second.");
	messages << QObject::tr("All tasks and cover downloads are implemented as <i>plugins</i>. You can write your own plugins with the <b>UltarStar Manager Plugin SDK</b>.");
	messages << QObject::tr("You can edit lyrics with <i>Ctrl+E</i>.");
	messages << QObject::tr("Three different encodings are supported for song files: CP1252 (Western European), CP1250 (Central/Eastern European), and UTF8.<br><br>You can set the default input encoding for song files whose encodings cannot be determined automatically in <i>Options &rarr; Default Encodings...</i>.");
	messages << QObject::tr("By default, song files are saved with CP1252 encoding to keep compatibility with older versions of UltraStar. If this encoding is insufficient for encoding your song file due to some special characters, it will automatically be saved using the UTF8 encoding.<br><br>You can change the default output encoding for song files in <i>Options &rarr; Default Encodings...</i>.");
	messages << QObject::tr("UltraStar Manager is not available in your language? Well, if you are interested in translating, just get in touch with one of the developers at the <a href=\"http://sourceforge.net/projects/uman\">project site</a>.");
	messages << QObject::tr("You enjoy high quality audio files, covers and backgrounds? Just set your personal quality preferences in <i>Options</i> under <i>Quality Thresholds</i> and sort your song collection by the corresponding column.<br><br>In addition to the colored ticks, non-square (1:1) covers and non-widescreen (16:9) backgrounds will be indicated by a small exclamation mark next to quality tick.");
	messages << QObject::tr("You can open image files externally for cropping and/or resizing using the file context menu.");
	messages << QObject::tr("Are you using the latest version of UltraStar Manager? Well, why don't you check by clicking <i>About &rarr; Check for Update</i>? You can enable an automatic update check at startup in the resulting dialog.");
	messages << QObject::tr("You can refresh a single song or a selection of songs by choosing <i>Reload</i> from the context menu or by simply pressing <i>F5</i>.");
}

QUMonty* QUMonty::_instance = 0;
QUMonty* QUMonty::instance() {
	if(_instance == 0)
		_instance = new QUMonty();

	return _instance;
}

QPixmap QUMonty::pic(QUMonty::Status status) {
	switch(status) {
	case QUMonty::normal:
		return QPixmap(QString(":/monty/normal.png"));
	case QUMonty::back:
		return QPixmap(QString(":/monty/back.png"));
	case QUMonty::happy:
		return QPixmap(QString(":/monty/happy.png"));
	case QUMonty::seated:
		return QPixmap(QString(":/monty/seated.png"));
	default:
		return QPixmap();
	}
}

QString QUMonty::welcomeMsg() {
	QString welcomeStr(QObject::tr("Hello! I am Monty the Mammoth. I will tell you some hints from time to time. Just press the <i>Hide</i> button below and I will disappear for now."));

	if (songDB->songCount() == 0) {
		welcomeStr += QObject::tr("<br><br>You have <b>no songs</b> in your collection. Try choosing a different song directory using <i>Options &rarr; Song Directory</i> or add a new song path via <i>Options &rarr; Paths...</i>.");
	} else if (songDB->songCount() == 1) {
		welcomeStr += QObject::tr("<br><br>You have <b>1 song</b> in your collection. Is it managed well yet?");
	} else {
		welcomeStr += QObject::tr("<br><br>You have <b>%1 songs</b> in your collection. Are they managed well yet?").arg(songDB->songCount());
	}

	return welcomeStr;
}

void QUMonty::talk(QLabel *montyLbl, QLabel *msgLbl) {
	montyLbl->setPixmap(pic((QUMonty::Status)(qrand() % 4)));
	QString message = messages[qrand() % messages.size()];
	if (!message.contains("%1")) {
		msgLbl->setText(message);
	} else {
		msgLbl->setText(message.arg(songDB->songCount()));
	}
}

/*!
 * Searches for hints to the given question.
 */
void QUMonty::answer(QLabel *montyLbl, QLabel *msgLbl, const QString &question, bool prev) {
	static QStringList answers;

	if(!question.isEmpty()) {
		answers.clear(); // process new question

		QRegExp rx(question, Qt::CaseInsensitive);
		foreach(QString msg, messages) {
			if(msg.contains(rx))
				answers.append(msg);
		}
	}

	if(answers.isEmpty()) {
		montyLbl->setPixmap(pic((QUMonty::Status)(qrand() % 4)));
		msgLbl->setText(QObject::tr("Sorry. I don't understand you."));
		return;
	}

	// show next answer
	QString out = prev ? answers.last() : answers.first();
	if(!prev) {
		answers.removeFirst();
		answers.append(out);
	} else {
		answers.removeLast();
		answers.prepend(out);
	}

	montyLbl->setPixmap(pic((QUMonty::Status)(qrand() % 4)));
	if (!answers.first().contains('1')) {
		msgLbl->setText(answers.first());
	} else {
		msgLbl->setText(answers.first().arg(songDB->songCount()));
	}
}

bool QUMonty::autoSaveEnabled() const {
	QSettings settings;
	return settings.value("autoSave", QVariant(true)).toBool();
}

/*!
 * Extract a resource (e.g. an image/icon) to a subdirectory of the report output
 * path and return a relative path to that resource.
 * \param item The resource to extract (e.g. ":/accept.png")
 * \param dest Directory of the report, NOT the subdirectory for the image
 * \returns Relative local path to the resource.
 */
QString QUMonty::useImageFromResource(const QString &item, QDir dest) {
	if(!dest.cd("images")) {
		dest.mkdir("images");
		if(!dest.cd("images")) {
//			logSrv->add(tr("Subdirectory for images could not be created!"), QU::Warning);
			return QString();
		}
	}

	QPixmap pixmap(item);
	QFileInfo fi(dest, QFileInfo(item).fileName());

	if(fi.exists()) {
		dest.cdUp();
		return dest.relativeFilePath(fi.filePath());
	}

	if(!pixmap.save(fi.filePath())) {
//		logSrv->add(QString(tr("The resource file \"%1\" could NOT be saved.")).arg(fi.filePath()), QU::Warning);
		return QString();
	}

//	logSrv->add(QString(tr("The resource file \"%1\" was extracted successfully.")).arg(fi.filePath()), QU::Information);

	dest.cdUp();
	return dest.relativeFilePath(fi.filePath());
}
