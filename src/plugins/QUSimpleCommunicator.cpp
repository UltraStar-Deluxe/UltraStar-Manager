#include "QUSimpleCommunicator.h"

QUSimpleCommunicator::QUSimpleCommunicator(QObject *parent): QUCommunicatorInterface(parent) {}

void QUSimpleCommunicator::send(SimpleMessage msg) {
	switch(msg) {
	case Done:
		emit done(); break;
	case Failed:
		emit failed(); break;
	}
}

void QUSimpleCommunicator::send(const QString &msg) {
	emit messageSent(msg);
}
