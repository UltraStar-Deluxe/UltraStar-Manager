#include "QUMultilingualTaskFactory.h"

#include <QTranslator>

QUMultilingualTaskFactory::QUMultilingualTaskFactory(QObject *parent): QObject(parent) {}

QUMultilingualTaskFactory::~QUMultilingualTaskFactory() {
	qDeleteAll(_translations);
	_translations.clear();
}

QMap<QString, QTranslator*> QUMultilingualTaskFactory::translations() const {
	if(_translations.isEmpty()) {
		QMapIterator<QString, QString> i(translationLocations());
		while(i.hasNext()) {
			i.next();
			QTranslator *translator = new QTranslator();
			if(translator->load(i.value()))
				_translations.insert(i.key(), translator);
		}
	}

	return _translations;
}
