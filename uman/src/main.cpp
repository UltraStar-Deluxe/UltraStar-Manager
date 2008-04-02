#include "QUMainWindow.h"

#include <QtGui>
#include <QApplication>
#include <QDateTime>

int main(int argc, char *argv[]) {
	Q_INIT_RESOURCE(uman);
	
	qsrand(QDateTime::currentDateTime().toTime_t());
	
    QApplication a(argc, argv);
    QUMainWindow w;
    w.show();
    a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
    return a.exec();
}
