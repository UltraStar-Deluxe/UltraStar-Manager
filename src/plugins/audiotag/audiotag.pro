SOURCES = ../shared/QUMultilingualTaskFactory.cpp \
	QUAudioTagTaskFactory.cpp \
	QUAudioTagTask.cpp \
	../../QUStringSupport.cpp \
	../../QUSongSupport.cpp \
	../shared/QUSimpleTask.cpp \
	../shared/QUScriptableTask.cpp \
	../shared/QUScriptableTaskFactory.cpp \
	QUAudioTagTaskDialog.cpp \
	QUAudioTagTaskSourceDelegate.cpp \
	../shared/QUTaskDialog.cpp \
	../shared/QUTaskDataTable.cpp \
	../shared/QUTaskConditionDelegate.cpp \
	../shared/QUTaskSourceDelegate.cpp \
	QUDefaultDelegate.cpp \
	../shared/QUSmartSetting.cpp \
	../shared/QUTaskModifierDelegate.cpp
HEADERS = language.h \
	../shared/QUMultilingualTaskFactory.h \
	QUAudioTagTaskFactory.h \
	QUAudioTagTask.h \
	../../QUStringSupport.h \
	../../QUSongSupport.h \
	../shared/QUSimpleTask.h \
	../shared/QUScriptableTask.h \
	../shared/QUScriptableTaskFactory.h \
	QUAudioTagTaskDialog.h \
	QUAudioTagTaskSourceDelegate.h \
	 ../shared/QUTaskDialog.h \
	../shared/QUTaskDataTable.h \
	../shared/QUTaskConditionDelegate.h \
	../shared/QUTaskSourceDelegate.h \
	QUDefaultDelegate.h \
	../shared/QUSmartSetting.h \
	../QUSmartSettingInterface.h \
	../shared/QUTaskModifierDelegate.h
FORMS += ../shared/QUTaskDialog.ui
OTHER_FILES += audiotag.json
TRANSLATIONS = audiotag.de.ts \
	audiotag.pl.ts \
	audiotag.es.ts \
	audiotag.fr.ts \
	audiotag.pt.ts
INCLUDEPATH += . \
	ui \
	.. \
	../.. \
	../../song \
	../shared \
	../../../include/taglib
TEMPLATE = lib
CONFIG += plugin
UI_DIR = ../ui
QT += xml \
	widgets

win32 {
	INCLUDEPATH += "C:/Program Files/taglib/include/taglib"
	LIBS += -L"C:/Program Files/taglib/lib" \
		-ltag
}
unix {
	CONFIG += link_pkgconfig
	PKGCONFIG += taglib
}
QMAKE_EXTRA_TARGETS += langtarget
PRE_TARGETDEPS += language.h
langtarget.target = language.h
langtarget.commands = python $${PWD}/getTaskText.py

CONFIG(release, debug|release) {
	TARGET = 3-audiotag
	DESTDIR = ../../../bin/release/plugins
	OBJECTS_DIR = ../tmp/audiotag/release
	MOC_DIR = ../tmp/audiotag/release
}

CONFIG(debug, debug|release) {
	TARGET = 3-audiotag_debug
	DESTDIR = ../../../bin/debug/plugins
	OBJECTS_DIR = ../tmp/audiotag/debug
	MOC_DIR = ../tmp/audiotag/debug
}

CONFIG_SRC_DIR = $$shell_path(config/)
CONFIG_DEST_DIR = $$shell_path($${DESTDIR}/config/audiotag/)
LANG_SRC_DIR = $$shell_path(./)
LANG_DEST_DIR = $$shell_path($${DESTDIR}/languages/)

QMAKE_POST_LINK += $$sprintf($${QMAKE_MKDIR_CMD}, "$${CONFIG_DEST_DIR}") $$escape_expand(\\n\\t)
QMAKE_POST_LINK += $${QMAKE_COPY} $${CONFIG_SRC_DIR}*.xml $${CONFIG_DEST_DIR} $$escape_expand(\\n\\t)
QMAKE_POST_LINK += $$sprintf($${QMAKE_MKDIR_CMD}, "$${LANG_DEST_DIR}") $$escape_expand(\\n\\t)
QMAKE_POST_LINK += $${QMAKE_COPY} $${LANG_SRC_DIR}*.qm $${LANG_DEST_DIR}
