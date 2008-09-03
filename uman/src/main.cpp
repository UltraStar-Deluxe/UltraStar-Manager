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

void initApplication();
void initLanguage(QApplication&, QTranslator&);

int main(int argc, char *argv[]) {
	initApplication();

	QApplication app(argc, argv);
	QTranslator  tr;

	QSplashScreen splash(QPixmap(":/icons/splash.png"));
	splash.showMessage(QString(QObject::tr("Version %1.%2.%3 is loading...")).arg(MAJOR_VERSION).arg(MINOR_VERSION).arg(PATCH_VERSION), Qt::AlignBottom | Qt::AlignRight, Qt::white);
	splash.show();

	initLanguage(app, tr);

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
 */
void initLanguage(QApplication &app, QTranslator &t) {
	QSettings settings;
	bool settingFound = true;
	QString lang = settings.value("language").toString();

	if(lang.isEmpty()) {
		lang = QLocale::system().name();
		settingFound = false;

		settings.setValue("language", QVariant(lang)); // remember this language (system local)
	}

	if(QString::compare(lang, "de_DE", Qt::CaseInsensitive) == 0) {
		if(t.load(":/lang/uman_de.qm")) {
			app.installTranslator(&t);
			monty->initMessages(":/txt/hints_de");
		}
	}

	// message needs to be here because it can be translated only after installing
	// the translator
	if(!settingFound)
		QUMessageBox::ask(0,
				QObject::tr("Init Language"),
				QObject::tr("There was no configuration setting about your language.Your <b>system language</b> will be used if possible.<br>"
						"<br>"
						"See <b>Options > Language</b> to change this."),
				":/marks/accept.png", QObject::tr("Continue."),
				"", "",
				"", "",
				50);
}
