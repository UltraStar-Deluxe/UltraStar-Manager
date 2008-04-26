#include "QUMainWindow.h"

#include <QtGui>
#include <QApplication>
#include <QDateTime>
#include <QTranslator>
#include <QSettings>

int main(int argc, char *argv[]) {
	Q_INIT_RESOURCE(uman);
	
	qsrand(QDateTime::currentDateTime().toTime_t());
	
	QCoreApplication::setOrganizationName("HPI");
	QCoreApplication::setApplicationName("UltraStar Manager");
	
	QTranslator umanTranslator;
	QApplication app(argc, argv);
	
	// language
	QSettings settings;
	QString lang = settings.value("language", QVariant(QLocale::system().name())).toString();

	if(QString::compare(lang, "de_DE", Qt::CaseInsensitive) == 0) {	
		if(umanTranslator.load(":/lang/uman_de.qm"))
			app.installTranslator(&umanTranslator);
	}	
	
    QUMainWindow w;
    w.show();
    app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
    return app.exec();
}
