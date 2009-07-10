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
	../shared/QUSmartSetting.cpp
HEADERS = QURenameTaskFactory.h \
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
	../shared/QUSmartSetting.h
FORMS += ../shared/QUTaskDialog.ui
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
