#ifndef QUCOMMUNICATORINTERFACE_H
#define QUCOMMUNICATORINTERFACE_H

#include <QObject>
#include <QString>

/*!
 * This interface enables simple communication between
 * the main app and a plugin due to the lack of signals
 * in plugin interfaces.
 */
class QUCommunicatorInterface: public QObject {
	Q_OBJECT

public:
	enum SimpleMessage {
		Done,
		Failed
	};

	QUCommunicatorInterface(QObject *parent = 0): QObject(parent) {}

	virtual void send(SimpleMessage) = 0;
	virtual void send(const QString &) = 0;
};

#endif // QUCOMMUNICATORINTERFACE_H
