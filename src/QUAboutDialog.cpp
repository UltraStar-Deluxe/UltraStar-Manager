#include "QUAboutDialog.h"
#include "main.h"
#include "version.h"
#include <QTimer>
#include <QScrollBar>
#include <QFile>
#include <QDateTime>

#define RESET_GAP   4000
#define SCROLL_STEP 1
#define SCROLL_GAP  100

QUAboutDialog::QUAboutDialog(QWidget *parent): QDialog(parent) {
	setupUi(this);

	this->setWindowFlags(Qt::SplashScreen);

	connect(acceptBtn, SIGNAL(clicked()), this, SLOT(accept()));

	QString creditsText;
	creditsText += "<style>a { color: #4479D4; text-decoration: none;}</style>";
	creditsText += "<center><br><br><br><br><br>";
	creditsText += QObject::tr("<b>Thank you</b> for using UltraStar Manager!");
	creditsText += "<br><br>***<br><br>";
	creditsText += QObject::tr("<b>Chief Programmer</b>");
	creditsText += "<br>Marcel Taeumel<br><br>";
	creditsText += QObject::tr("<b>Assistant Programmer</b>");
	creditsText += "<br>Markus Böhning<br><br>";
	creditsText += QObject::tr("<b>NSIS Setup Script</b>");
	creditsText += "<br>Michael Grünewald<br>Marcel Taeumel<br>Markus Böhning<br><br>";
	creditsText += QObject::tr("<b>German Translation</b>");
	creditsText += "<br>Marcel Taeumel<br>Markus Böhning<br><br>";
	creditsText += QObject::tr("<b>French Translation</b>");
	creditsText += "<br>Fred (Pinky)<br><br>";
	creditsText += QObject::tr("<b>Spanish Translation</b>");
	creditsText += "<br>Francisco Montero<br>";
	creditsText += QObject::tr("e-mail:");
	creditsText += " <a href=\"mailto:panchomillenium@gmail.com\">panchomillenium@gmail.com</a><br><br>";
	creditsText += QObject::tr("<b>Portuguese Translation</b>");
	creditsText += "<br>zup3r_vock<br><br>";
	creditsText += QObject::tr("<b>Polish Translation</b>");
	creditsText += "<br>Robert de Clair<br>";
	creditsText += QObject::tr("Website:");
	creditsText += " <a href=\"http://www.krokmania.com\">www.krokmania.com</a>";
	creditsText += "<br>Michal \"michalek^\" Janowski<br><br>";

	creditsText += QObject::tr("<b>CSS Styles (HTML Report)</b>");
	creditsText += "<br>Fabian Lindenberg<br>Marcel Taeumel";
	creditsText += "<br><br>***<br><br>";
	creditsText += QObject::tr("<b>Silk Icons</b>");
	creditsText += "<br>© 2005-2006 Mark James<br>";
	creditsText += QObject::tr("Website:");
	creditsText += " <a href=\"http://www.famfamfam.com\">www.famfamfam.com</a><br>";
	creditsText += QObject::tr("License:");
	creditsText += "<a href=\"http://creativecommons.org/licenses/by/2.5\">CC Attribution 2.5 Generic</a><br><br>";
	creditsText += QObject::tr("<b>Diagona Icons</b>");
	creditsText += "<br>© 2007 Yusuke Kamiyamane<br>";
	creditsText += QObject::tr("Website:");
	creditsText += " <a href=\"http://www.pinvoke.com\">www.pinvoke.com</a><br>";
	creditsText += QObject::tr("License:");
	creditsText += " <a href=\"http://creativecommons.org/licenses/by/3.0\">CC Attribution 3.0 Unported</a><br><br>";
	creditsText += QObject::tr("<b>Fugue Icons</b>");
	creditsText += "<br>© 2008 Yusuke Kamiyamane<br>";
	creditsText += QObject::tr("Website:");
	creditsText += " <a href=\"http://www.pinvoke.com\">www.pinvoke.com</a><br>";
	creditsText += QObject::tr("License:");
	creditsText += " <a href=\"http://creativecommons.org/licenses/by/3.0\">CC Attribution 3.0 Unported</a><br><br>";
	creditsText += QObject::tr("<b>Monty Icons</b>");
	creditsText += "<br>© 2008 Dirceu Veiga<br>";
	creditsText += QObject::tr("Website:");
	creditsText += " <a href=\"http://dirceuveiga.com.br/eg/\">dirceuveiga.com.br</a><br><br>";
	creditsText += QObject::tr("<b>Boomy Icons</b>");
	creditsText += "<br>© 2006-2008 Milosz Wlazlo<br>";
	creditsText += QObject::tr("Website:");
	creditsText += " <a href=\"http://miloszwl.deviantart.com\">miloszwl.deviantart.com</a><br><br>";
	creditsText += QObject::tr("<b>Tiny Animals Icons</b>");
	creditsText += "<br>© 2008 Iconshock<br>";
	creditsText += QObject::tr("Website:");
	creditsText += " <a href=\"http://www.iconshock.com\">www.iconshock.com</a><br>";
	creditsText += QObject::tr("License:");
	creditsText += " <a href=\"http://en.wikipedia.org/wiki/Linkware\">Linkware</a><br><br>";
	creditsText += QObject::tr("<b>Milky Icons</b>");
	creditsText += "<br>© 2008 IconEden (Frexy)<br>";
	creditsText += QObject::tr("Website:");
	creditsText += " <a href=\"http://iconeden.com/icon/milky-a-free-vector-iconset.html\">www.iconeden.com</a><br><br>";
	creditsText += QObject::tr("<b>Graphical Compositions</b>");
	creditsText += "<br>Marcel Taeumel";
	creditsText += "<br><br>***<br><br>";
	creditsText += QObject::tr("<b>Picture Flow</b>");
	creditsText += "<br>© 2007 - 2008 Ariya Hidayat<br>";
	creditsText += QObject::tr("Website:");
	creditsText += " <a href=\"http://pictureflow.googlecode.com\">pictureflow.googlecode.com</a><br>";
	creditsText += QObject::tr("License:");
	creditsText += " <a href=\"http://www.opensource.org/licenses/mit-license.php\">MIT-License</a><br><br>";
	creditsText += QObject::tr("<b>HMAC-SHA2 Implementation</b>");
	creditsText += "<br>© 2005, 2007 Olivier Gay<br>";
	creditsText += QObject::tr("e-mail:");
	creditsText += " <a href=\"mailto:olivier.gay@a3.epfl.ch\">olivier.gay@a3.epfl.ch</a><br>";
	creditsText += QObject::tr("Website:");
	creditsText += " <a href=\"http://www.ouah.org/ogay/hmac\">www.ouah.org</a>";
	creditsText += "<br><br>***<br><br>";
	creditsText += QObject::tr("<b>Tester</b>");
	creditsText += "<br>Michael Grünewald<br>Matthias Jacob<br>BlindGuard<br>MasterPhW<br>hanzmole<br>Tokutai<br>Robert de Clair<br>escaped<br>DeathStar<br>gabro<br><br>";
	creditsText += QObject::tr("<b>Special Thanks</b>");
	creditsText += "<br>escaped<br>BlindGuard<br><br><br><br><br>";
	creditsText += "<b>Copyright © 2008-2017</b></center>";

	credits->setHtml(creditsText);

	QDateTime dateTime = QDateTime::fromString(QString(date_time), "dd.MM.yyyy HH:mm");

	versionLbl->setText(QString(tr("Version: <b>%1.%2.%3</b><br>Commit: %4, %5"))
			.arg(MAJOR_VERSION)
			.arg(MINOR_VERSION)
			.arg(PATCH_VERSION)
			.arg(QString(revision).remove(QRegExp("(.*:)|\\D")))
			.arg(dateTime.toString("yyyy-MM-dd, HH:mm")));

	resetText();
	credits->document()->setDefaultStyleSheet("a {color: white}");
}

void QUAboutDialog::scrollDown() {
	QScrollBar *bar = credits->verticalScrollBar();

	if(bar->value() >= bar->maximum())
		QTimer::singleShot(RESET_GAP, this, SLOT(resetText()));
	else {
		bar->setValue(bar->value() + SCROLL_STEP);
		QTimer::singleShot(SCROLL_GAP, this, SLOT(scrollDown()));
	}
}

void QUAboutDialog::resetText() {
	QScrollBar *bar = credits->verticalScrollBar();
	bar->setValue(bar->minimum());
	QTimer::singleShot(SCROLL_GAP, this, SLOT(scrollDown()));
}
