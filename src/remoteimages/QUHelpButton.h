#ifndef QUHELPBUTTON_H
#define QUHELPBUTTON_H

#include <QToolButton>
#include <QString>

class QUHelpButton: public QToolButton {
	Q_OBJECT

public:
	QUHelpButton(QWidget *parent = 0);

	void setText(const QString &text) { _text = text; }
	void setTopic(const QString &text) { _topic = text; }

protected slots:
	void showHelp();

protected:
	QString text() const { return _text; }
	QString topic() const { return _topic; }

private:
	QString _text;
	QString _topic;
};

#endif // QUHELPBUTTON_H
