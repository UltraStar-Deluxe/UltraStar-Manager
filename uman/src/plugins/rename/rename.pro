SOURCES = QURenameTaskFactory.cpp \
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
	../QUSmartSetting.cpp
HEADERS = language.h \
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
	../QUSmartSetting.h
FORMS += ../shared/QUTaskDialog.ui
TRANSLATIONS = rename.de.ts \
	rename.pl.ts
INCLUDEPATH += . \
    ui \
    .. \
    ../.. \
    ../../song \
    ../shared
TARGET = 4-rename
TEMPLATE = lib
CONFIG += plugin
CONFIG -= debug
DESTDIR = ../../../bin/wip/plugins
OBJECTS_DIR = ../tmp/rename
MOC_DIR = ../tmp/rename
UI_DIR = ../ui
QT += xml
QMAKE_EXTRA_TARGETS += langtarget
PRE_TARGETDEPS += language.h
langtarget.target = language.h
langtarget.commands = python getTaskText.py
