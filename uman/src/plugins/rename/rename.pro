SOURCES = ../shared/QUMultilingualTaskFactory.cpp \
	QURenameTaskFactory.cpp \
    QURenameTask.cpp \
    ../../QUStringSupport.cpp \
    ../../QUSongSupport.cpp \
    ../shared/QUSimpleTask.cpp \
    ../shared/QUScriptableTask.cpp \
    ../shared/QUScriptableTaskFactory.cpp \
    QURenameTaskDialog.cpp \
    QURenameTaskSourceDelegate.cpp \
    ../shared/QUTaskDialog.cpp \
    ../shared/QUTaskDataTable.cpp \
    ../shared/QUTaskConditionDelegate.cpp \
    ../shared/QUTaskSourceDelegate.cpp \
	../shared/QUTaskTextDelegate.cpp \
	../shared/QUSmartSetting.cpp
HEADERS = language.h \
	../shared/QUMultilingualTaskFactory.h \
	QURenameTaskFactory.h \
    QURenameTask.h \
    ../../QUStringSupport.h \
    ../../QUSongSupport.h \
    ../shared/QUSimpleTask.h \
    ../shared/QUScriptableTask.h \
    ../shared/QUScriptableTaskFactory.h \
    QURenameTaskDialog.h \
    QURenameTaskSourceDelegate.h \
    ../shared/QUTaskDialog.h \
    ../shared/QUTaskDataTable.h \
    ../shared/QUTaskConditionDelegate.h \
    ../shared/QUTaskSourceDelegate.h \
	../shared/QUTaskTextDelegate.h \
	../shared/QUSmartSetting.h \
	../QUSmartSettingInterface.h
FORMS += ../shared/QUTaskDialog.ui
TRANSLATIONS = rename.de.ts \
	rename.pl.ts \
	rename.es.ts
INCLUDEPATH += . \
    ui \
    .. \
    ../.. \
    ../../song \
    ../shared
TEMPLATE = lib
CONFIG += plugin
UI_DIR = ../ui
QT += xml
QMAKE_EXTRA_TARGETS += langtarget
PRE_TARGETDEPS += language.h
langtarget.target = language.h
langtarget.commands = python getTaskText.py

CONFIG(release, debug|release) {
	TARGET = 4-rename
	DESTDIR = ../../../bin/wip/plugins
	OBJECTS_DIR = ../tmp/rename/release
	MOC_DIR = ../tmp/rename/release
}

CONFIG(debug, debug|release) {
	TARGET = 4-rename_debug
	DESTDIR = ../../../bin/wip_debug/plugins
	OBJECTS_DIR = ../tmp/rename/debug
	MOC_DIR = ../tmp/rename/debug
}
