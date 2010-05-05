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
	../shared/QUSmartSetting.cpp
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
	../QUSmartSettingInterface.h
FORMS += ../shared/QUTaskDialog.ui
TRANSLATIONS = audiotag.de.ts \
	audiotag.pl.ts \
	audiotag.es.ts
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
QT += xml
win32 { 
    INCLUDEPATH += ../../../include/taglib
    LIBS += -L"../../../lib" \
        -ltag
}
unix { 
    INCLUDEPATH += /usr/include/taglib
    LIBS += -L"/usr/lib" \
        -ltag
}
QMAKE_EXTRA_TARGETS += langtarget
PRE_TARGETDEPS += language.h
langtarget.target = language.h
langtarget.commands = python getTaskText.py

CONFIG(release, debug|release) {
	TARGET = 3-audiotag
	DESTDIR = ../../../bin/wip/plugins
	OBJECTS_DIR = ../tmp/audiotag/release
	MOC_DIR = ../tmp/audiotag/release
}

CONFIG(debug, debug|release) {
	TARGET = 3-audiotag_debug
	DESTDIR = ../../../bin/wip_debug/plugins
	OBJECTS_DIR = ../tmp/audiotag/debug
	MOC_DIR = ../tmp/audiotag/debug
}
