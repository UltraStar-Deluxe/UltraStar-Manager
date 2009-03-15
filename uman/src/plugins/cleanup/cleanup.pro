SOURCES = QUCleanUpTaskFactory.cpp \
    QUCleanUpTask.cpp \
    ../shared/QUSimpleTask.cpp \
    ../shared/QUSimpleTaskFactory.cpp
HEADERS = QUCleanUpTaskFactory.h \
    QUCleanUpTask.h \
    ../shared/QUSimpleTask.h \
    ../shared/QUSimpleTaskFactory.h
INCLUDEPATH += . \
    .. \
    ../.. \
    ../../song \
    ../shared
TARGET = 5-cleanup
TEMPLATE = lib
CONFIG += plugin
CONFIG -= debug
DESTDIR = ../../../bin/wip/plugins
OBJECTS_DIR = tmp
MOC_DIR = tmp
