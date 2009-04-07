#include "QUScriptableTask.h"
#include "QUSongSupport.h"

#include <QFileInfo>
#include <QVariant>
#include <QMessageBox>
#include <QRegExp>

QUScriptableTask::QUScriptableTask(QDomDocument *taskConfig, QObject *parent): QUSimpleTask(parent) {
	this->_configFileName = taskConfig->firstChildElement("task").attribute("file", tr("unnamed.xml"));

	// setup visual appearance
	QDomElement general(taskConfig->firstChild().firstChildElement("general"));
	if(!general.isNull()) {
		this->setIcon(QIcon(general.firstChildElement("icon").attribute("resource")));
		this->setDescription(general.firstChildElement("description").firstChild().toCDATASection().data().trimmed().toLocal8Bit().data());
		this->setToolTip(general.firstChildElement("tooltip").firstChild().toCDATASection().data().trimmed().toLocal8Bit().data());
		this->setGroup(QVariant(general.attribute("group", "-1")).toInt());

		this->_iconSource = general.firstChildElement("icon").attribute("resource");
	}
}

/*!
 * Just for convenience.
 */
QStringList QUScriptableTask::availableSources() {
	QStringList result;

	// special sources
	result << TEXT_SOURCE;// << KEEP_SUFFIX_SOURCE << UNKNOWN_TAGS_SOURCE;

	return result;
}

QStringList QUScriptableTask::availableCustomSources() {
	return QUSongSupport::availableCustomTags();
}

QStringList QUScriptableTask::availableConditions() {
	return QString("true hasMp3 hasCover hasBackground hasVideo isSongChecked isDuet isKaraoke isSingStar").split(" ");
}
