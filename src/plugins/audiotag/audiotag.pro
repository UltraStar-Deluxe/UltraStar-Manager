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
	LIBS += -L"../../../lib/Windows" \
		-ltag
}
mac {
	LIBS += -L"/usr/local/lib" \
		-ltag
}
unix:!macx {
	LIBS += -ltag
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

CONFIG_SRC_DIR = $$IN_PWD/config/
CONFIG_DEST_DIR = $${DESTDIR}/config/audiotag/
LANG_SRC_DIR = $$IN_PWD/
LANG_DEST_DIR = $${DESTDIR}/languages/
win32 {
    CONFIG_SRC_DIR ~= s,/,\\,g
    CONFIG_DEST_DIR ~= s,/,\\,g
    LANG_SRC_DIR ~= s,/,\\,g
    LANG_DEST_DIR ~= s,/,\\,g
}

QMAKE_POST_LINK += $$sprintf($${QMAKE_MKDIR_CMD}, "$${CONFIG_DEST_DIR}") $$escape_expand(\n\t)
QMAKE_POST_LINK += $${QMAKE_COPY} $${CONFIG_SRC_DIR}* $${CONFIG_DEST_DIR} $$escape_expand(\n\t)
QMAKE_POST_LINK += $$sprintf($${QMAKE_MKDIR_CMD}, "$${LANG_DEST_DIR}") $$escape_expand(\n\t)
QMAKE_POST_LINK += $${QMAKE_COPY} $${LANG_SRC_DIR}*.qm $${LANG_DEST_DIR}
