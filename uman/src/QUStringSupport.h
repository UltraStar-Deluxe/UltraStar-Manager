#ifndef QUSTRINGSUPPORT_H_
#define QUSTRINGSUPPORT_H_

#include <QObject>
#include <QString>

class QUStringSupport: QObject {
    Q_OBJECT
    
public:
    QUStringSupport(QObject *parent = 0);

    static QString withoutUnsupportedCharacters (const QString &text);
	static QString withoutFolderTags(const QString &text);
	static QString withoutLeadingBlanks(const QString &text);
	static QString withoutAnyUmlaut(const QString &text);
	static QString withoutAnyUmlautEx(const QString &text);
};

#endif // QUSTRINGSUPPORT_H_
