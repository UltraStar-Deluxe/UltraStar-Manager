#include "QUScriptableTask.h"

#include <QFileInfo>
#include <QVariant>
#include <QMessageBox>
#include <QRegExp>

QUScriptableTask::QUScriptableTask(QDomDocument *taskConfig, QObject *parent): QUAbstractTask(parent) {
	this->_configFileName = taskConfig->firstChildElement("task").attribute("file", tr("unnamed.xml"));

	// setup visual appearance
	QDomElement general(taskConfig->firstChild().firstChildElement("general"));
	if(!general.isNull()) {
		this->setIcon(QIcon(general.firstChildElement("icon").attribute("resource")));
		this->setDescription(general.firstChildElement("description").firstChild().toCDATASection().data().trimmed().toLocal8Bit().data());
		this->setToolTip(general.firstChildElement("tooltip").firstChild().toCDATASection().data().trimmed().toLocal8Bit().data());

		this->_group = QVariant(general.attribute("group", "-1")).toInt();
		this->_iconSource = general.firstChildElement("icon").attribute("resource");
	}
}
