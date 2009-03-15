SOURCES = QULyricTaskFactory.cpp \
    QULyricTask.cpp \
    ../shared/QUSimpleTask.cpp \
    ../shared/QUSimpleTaskFactory.cpp
HEADERS = QULyricTaskFactory.h \
    QULyricTask.h \
    ../shared/QUSimpleTask.h \
    ../shared/QUSimpleTaskFactory.h
INCLUDEPATH += . \
    .. \
    ../.. \
    ../../song \
    ../shared
TARGET = 2-lyric
TEMPLATE = lib
CONFIG += plugin
CONFIG -= debug
DESTDIR = ../../../bin/wip/plugins
OBJECTS_DIR = tmp
MOC_DIR = tmp
