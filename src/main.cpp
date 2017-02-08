#include "main.h"
#include "QUMainWindow.h"
#include "QUMonty.h"
#include "QUMessageBox.h"

#include <QApplication>
#include <QDateTime>
#include <QTranslator>
#include <QSettings>
#include <QSplashScreen>
#include <QPixmap>
#include <QString>
#include <QFile>
#include <QLocale>
#include <QFileInfo>

void initApplication();
void initLanguage(QApplication&, QTranslator&, QTranslator&, QSplashScreen&);
void handlePreviousAppCrash();
void handleWipWarning();
void handleReleaseCandidateInformation();
void handleArguments();

int main(int argc, char *argv[]) {
	initApplication();

	QApplication app(argc, argv);
	QTranslator  trContent;
	QTranslator  trQt;

	QSplashScreen splash(QPixmap(":/icons/splash2.png"));
	splash.show();

	initLanguage(app, trContent, trQt, splash);

#ifdef WIP_VERSION
	handleWipWarning();
#endif // WIP_VERSION

#ifdef RC_VERSION
	handleReleaseCandidateInformation();
#endif // RC_VERSION

	handlePreviousAppCrash();
	handleArguments();

	QUMainWindow mainWindow;
	app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));

	mainWindow.show();
	splash.finish(&mainWindow);

	return app.exec();
}

/*!
 * Initializes general aspects of this application like resources,
 * random timers and registry setups.
 */
void initApplication() {
	Q_INIT_RESOURCE(uman);

	qsrand(QDateTime::currentDateTime().toTime_t());

	QCoreApplication::setOrganizationName("HPI");
	QCoreApplication::setApplicationName("UltraStar Manager");
}

/*!
 * Installs a proper translator according to the registry settings. That's why you
 * have to restart this application if you want to change its language. Uses the system
 * language if no registry setting is found.
 *
 * Note that there is no special translation file for English present. That's why all
 * strings in the application source code are in English.
 */
void initLanguage(QApplication &app, QTranslator &trContent, QTranslator &trQt, QSplashScreen &s) {
	QSettings settings;
	bool settingFound;
	QLocale lang;

	if(!settings.contains("language")) {
		settingFound = false;
		lang = QLocale::system();
		settings.setValue("language", lang.name()); // remember this language (system local)
	} else {
		settingFound = true;
		lang = QLocale(settings.value("language").toString());
	}

	if(lang.language() == QLocale::Spanish) {
		if(trContent.load(":/lang/uman.es.qm")) {
			app.installTranslator(&trContent);
		}
		if(trQt.load(":/lang/qt_es.qm")) {
			app.installTranslator(&trQt);
		}
	} else if(lang.language() == QLocale::German) {
		if(trContent.load(":/lang/uman.de.qm")) {
			app.installTranslator(&trContent);
		}
		if(trQt.load(":/lang/qt_de.qm")) {
			app.installTranslator(&trQt);
		}
	} else if(lang.language() == QLocale::French) {
		if(trContent.load(":/lang/uman.fr.qm")) {
			app.installTranslator(&trContent);
		}
		if(trQt.load(":/lang/qt_fr.qm")) {
			app.installTranslator(&trQt);
		}
	} else if(lang.language() == QLocale::Polish) {
		if(trContent.load(":/lang/uman.pl.qm")) {
			app.installTranslator(&trContent);
		}
		if(trQt.load(":/lang/qt_pl.qm")) {
			app.installTranslator(&trQt);
		}
	} else if(lang.language() == QLocale::Portuguese) {
		if(trContent.load(":/lang/uman.pt.qm")) {
			app.installTranslator(&trContent);
		}
		if(trQt.load(":/lang/qt_pt.qm")) {
			app.installTranslator(&trQt);
		}
	}

	monty->initMessages();

	s.showMessage(QString(QObject::tr("%1.%2.%3 is loading...")).arg(MAJOR_VERSION).arg(MINOR_VERSION).arg(PATCH_VERSION), Qt::AlignBottom | Qt::AlignRight, Qt::white);

	// message needs to be here because it can be translated only after installing the translator
	if(!settingFound)
		QUMessageBox::information(0,
				QObject::tr("Init Language"),
				QObject::tr("There was no configuration setting about your language. Your <b>system language</b> will be used if possible.<br>"
						"<br>"
						"See <b>Options > Language</b> to change this."),
				BTN << ":/marks/accept.png" << QObject::tr("Continue."),
				270);
}

void handleWipWarning() {
	QUMessageBox::warning(0,
				QObject::tr("WIP version detected!"),
				QObject::tr("This version is still under development <b>and may not be very stable</b>. You might loose all your songs if you use it.<br><br><b>Please backup</b> the songs you are working with and do not distribute this version."),
				BTN << ":/marks/accept.png" << QObject::tr("I will be careful."),
				385);
}

void handleReleaseCandidateInformation() {
	QUMessageBox::information(0,
				QObject::tr("Release Candidate Information"),
				QObject::tr("<b>Dear testing person,</b><br><br>this version is meant to be <b>feature-complete and almost bug-free</b>. Nevertheless, it might be possible that some errors may corrupt your song database. Please backup your song database first.<br>While testing the release candidate, especially consider the following:<br><br>- support for CP1252, CP1250 and UTF8 encodings<br>- support for medley and preview tags<br>- mp3/cover/background quality assessment in songtree<br>- shiny new French, Spanish, and Portuguese translation<br>- updated and completed Polish translation<br>- duet bugs fixed<br>- check for update feature<br>- new task to increase low BPMs<br>- new task to normalize note pitches<br>- new task to capitalize the first word of each line<br><br>Send feedback and bug reports to: <a href=\"mailto:bohning@users.sourceforge.net\">bohning@users.sourceforge.net</a>"),
				BTN << ":/marks/accept.png" << QObject::tr("Okay!"),
				400);
}

void handlePreviousAppCrash() {
	if(!QFile::exists("running.app")) {
		QFile f("running.app");
		f.open(QIODevice::WriteOnly);
		f.close();

		return; // everything is ok
	}

	int result = QUMessageBox::critical(0,
			QObject::tr("Application Crash Detected"),
			QObject::tr("The UltraStar Manager did not exit successfully last time. Maybe you've chosen a <b>bad song folder</b>.<br>"
					"<br>"
					"Please report this problem <a href=\"http://uman.sf.net\">here</a>."),
			BTN	<< ":/marks/accept.png" << QObject::tr("Try again.")
				<< ":/control/folder_note.png" << QObject::tr("Select another song folder."));
	if(result == 1) {
		QSettings settings;
		settings.remove("songPath");
		settings.remove("playlistFilePath");
	}
}

/*!
 * Read arguments as a song path to be loaded.
 */
void handleArguments() {
	QSettings s;
	foreach(QString arg, qApp->arguments()) {
		QFileInfo fi(arg);

		if(fi.isFile())
			continue; // application file

		if(fi.isDir() && fi.exists()) {
//			QStringList paths(s.value("songPaths").toStringList());
//			paths.append(arg);
//			paths.removeDuplicates();
//			s.setValue("songPaths", paths);
			s.setValue("songPath", arg);
			break;
		}
	}
}
